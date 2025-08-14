// Minimal UTF-8 Korean text rendering on DigiPen AlphaEngine using FreeType
// - Single dynamic atlas (RGBA)
// - Shelf packing (row-based)
// - UTF-8 decoder
// - Draw via AEGfxMeshStart / AEGfxTriAdd / AEGfxMeshEnd / AEGfxMeshDraw
//
// Build notes:
//   - Include FreeType2 (ft2build.h) and link freetype.
//   - Include AlphaEngine headers/libraries as in your project template.
//   - Coordinate system toggleable: SetWorldYAxisUp(true/false)
//   - Texture V flip toggleable:   SetUVFlipV(true/false)
#pragma once
#define NOMINMAX

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cassert>

#include "AEEngine.h"
#include "AEGraphics.h" // AEGfxTextureLoadFromMemory, AEGfxTriAdd 등

struct Glyph
{
    float u0, v0, u1, v1; // UV [0..1]
    int   w, h;           // bitmap size
    int   bearingX;       // left
    int   bearingY;       // top
    int   advance;        // in pixels (26.6 >> 6)
};

static inline std::vector<uint32_t> DecodeUTF8(const std::string& s)
{
    std::vector<uint32_t> cps;
    size_t i = 0;
    while (i < s.size())
    {
        uint8_t c = (uint8_t)s[i];
        if ((c & 0x80) == 0)
        {
            cps.push_back(c);
            i += 1;
        }
        else if ((c & 0xE0) == 0xC0 && i + 1 < s.size())
        {
            uint32_t cp = ((c & 0x1F) << 6) | ((uint8_t)s[i + 1] & 0x3F);
            cps.push_back(cp);
            i += 2;
        }
        else if ((c & 0xF0) == 0xE0 && i + 2 < s.size())
        {
            uint32_t cp = ((c & 0x0F) << 12) |
                (((uint8_t)s[i + 1] & 0x3F) << 6) |
                ((uint8_t)s[i + 2] & 0x3F);
            cps.push_back(cp);
            i += 3;
        }
        else if ((c & 0xF8) == 0xF0 && i + 3 < s.size())
        {
            uint32_t cp = ((c & 0x07) << 18) |
                (((uint8_t)s[i + 1] & 0x3F) << 12) |
                (((uint8_t)s[i + 2] & 0x3F) << 6) |
                ((uint8_t)s[i + 3] & 0x3F);
            cps.push_back(cp);
            i += 4;
        }
        else
        {
            // skip invalid byte
            i += 1;
        }
    }
    return cps;
}

class FontAtlasAE
{
public:
    struct TextMetrics {
        float width;      // 가장 긴 줄의 픽셀 폭
        float height;     // 전체 텍스트 박스 높이(여러 줄 포함)
        float ascender;   // 1줄 기준 위쪽 여유(px)
        float descender;  // 1줄 기준 아래쪽 여유(px, 보통 음수 아님: 절댓값)
        float lineHeight; // 줄 간격(px)
    };

    TextMetrics GetPrintMetricsUTF8(const std::string& utf8,
        float scale = 1.0f,
        bool useKerning = true)
    {
        std::vector<uint32_t> cps = DecodeUTF8(utf8);
        for (uint32_t cp : cps)
            if (cp != '\n') EnsureGlyph(cp);

        // FreeType 메트릭에서 lineHeight/asc/desc 가져오기 (px)
        // face->size->metrics.* 는 26.6 fixed → >> 6
        const float asc = (face && face->size) ? float(face->size->metrics.ascender >> 6)
            : float(face->ascender >> 6);
        const float desc = (face && face->size) ? float((-(face->size->metrics.descender >> 6))) // 양수로 취급
            : float((-(face->descender >> 6)));
        const float lH = (face && face->size) ? float(face->size->metrics.height >> 6)
            : float(face->height >> 6);

        const float sx = scale;               // 가로 스케일
        const float sy = scale * yStretch;    // 세로 스케일(클래스 토글 반영)

        float curLineW = 0.f;   // 현재 줄 폭(픽셀)
        float maxLineW = 0.f;   // 최종 가장 긴 줄 폭
        int   lineCount = 1;    // 최소 1줄

        FT_UInt prevIndex = 0;

        for (size_t i = 0; i < cps.size(); ++i)
        {
            uint32_t cp = cps[i];
            if (cp == '\n')
            {
                maxLineW = (maxLineW > curLineW) ? maxLineW : curLineW;
                curLineW = 0.f;
                prevIndex = 0;
                ++lineCount;
                continue;
            }

            // 글리프 메트릭 조회
            const Glyph* g = EnsureGlyph(cp);
            if (!g) { prevIndex = 0; continue; }

            // 커닝(가로폭 보정)
            if (useKerning && prevIndex && FT_HAS_KERNING(face))
            {
                FT_Vector kern{};
                FT_Get_Kerning(face, prevIndex, FT_Get_Char_Index(face, cp),
                    FT_KERNING_DEFAULT, &kern);
                curLineW += float(kern.x >> 6) * sx;
            }

            // advance로 가로폭 누적
            curLineW += g->advance * sx;

            prevIndex = FT_Get_Char_Index(face, cp);
        }

        maxLineW = (maxLineW > curLineW) ? maxLineW : curLineW;

        // 전체 높이: (lineCount 줄) * lineHeight
        // 세로 스케일(sy) 적용
        float totalH = (lineCount <= 0 ? 0.f : (lineCount - 1) * lH * sy + (asc + desc) * sy);

        TextMetrics m;
        m.width = maxLineW;      // 가장 긴 줄 폭(픽셀)
        m.height = totalH;        // 전체 텍스트 박스 높이(픽셀)
        m.ascender = asc * sy;      // 1줄 기준 위 여유
        m.descender = desc * sy;     // 1줄 기준 아래 여유(양수로 제공)
        m.lineHeight = lH * sy;       // 줄 간격
        return m;
    }


    void SetYStretch(float k) { yStretch = k; }
    bool Init(const char* fontPath, int pixelHeight, int atlasW = 2048, int atlasH = 2048)
    {
        atlasWidth = atlasW;
        atlasHeight = atlasH;
        pixels.assign(atlasWidth * atlasHeight * 4, 0); // RGBA all zero (transparent)
        penX = 1;
        penY = 1;
        rowH = 0;

        if (FT_Init_FreeType(&ft))
            return false;
        if (FT_New_Face(ft, fontPath, 0, &face))
            return false;
        FT_Set_Pixel_Sizes(face, 0, pixelHeight);

        // Create an empty atlas texture up-front (all transparent)
        texture = AEGfxTextureLoadFromMemory((u8*)pixels.data(), (u32)atlasWidth, (u32)atlasHeight);
        return (texture != nullptr);
    }

    void Shutdown()
    {
        if (texture)
        {
            AEGfxTextureUnload(texture);
            texture = nullptr;
        }
        if (face)
        {
            FT_Done_Face(face);
            face = nullptr;
        }
        if (ft)
        {
            FT_Done_FreeType(ft);
            ft = nullptr;
        }
        glyphs.clear();
        pixels.clear();
    }

    // ====== orientation/UV toggles ======
    void SetWorldYAxisUp(bool up) { worldYAxisUp = up; } // true: +Y up, false: +Y down
    void SetUVFlipV(bool flip) { flipV = flip; }     // true: swap v0/v1 when drawing

    // Ensure glyph exists in atlas; returns pointer for rendering
    const Glyph* EnsureGlyph(uint32_t codepoint)
    {
        auto it = glyphs.find(codepoint);
        if (it != glyphs.end())
            return &it->second;

        if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER))
            return nullptr;

        FT_GlyphSlot g = face->glyph;
        int gw = g->bitmap.width;
        int gh = g->bitmap.rows;

        // New row if the glyph doesn't fit in current row
        if (penX + gw + padding > atlasWidth)
        {
            penX = 1;
            penY += rowH + padding;
            rowH = 0;
        }
        // Out of atlas?
        if (penY + gh + padding > atlasHeight)
        {
            // Minimal example: fail if atlas full.
            // (실전: atlas 리사이즈/재배치 또는 다중 atlas)
            return nullptr;
        }

        // Copy 8-bit alpha into RGBA buffer (white text, straight alpha)
        for (int y = 0; y < gh; ++y)
        {
            for (int x = 0; x < gw; ++x)
            {
                int dstX = penX + x;
                int dstY = penY + y;
                uint8_t a = g->bitmap.buffer[(gh - 1 - y) * g->bitmap.pitch + x];

                size_t idx = (size_t)(dstY * atlasWidth + dstX) * 4;
                pixels[idx + 0] = 255; // R
                pixels[idx + 1] = 255; // G
                pixels[idx + 2] = 255; // B
                pixels[idx + 3] = a;   // A
            }
        }

        Glyph glyph;
        glyph.w = gw;
        glyph.h = gh;
        glyph.bearingX = g->bitmap_left;
        glyph.bearingY = g->bitmap_top;
        glyph.advance = (g->advance.x >> 6);

        glyph.u0 = (float)penX / (float)atlasWidth;
        glyph.v0 = (float)penY / (float)atlasHeight;
        glyph.u1 = (float)(penX + gw) / (float)atlasWidth;
        glyph.v1 = (float)(penY + gh) / (float)atlasHeight;

        glyphs.emplace(codepoint, glyph);

        // Advance shelf cursor
        penX += gw + padding;
        rowH = max(rowH, gh); // FIX: std::max

        atlasDirty = true;
        return &glyphs[codepoint];
    }

    void UploadIfDirty()
    {
        if (!atlasDirty) return;
        if (texture)
            AEGfxTextureUnload(texture);
        texture = AEGfxTextureLoadFromMemory((u8*)pixels.data(), (u32)atlasWidth, (u32)atlasHeight);
        atlasDirty = false;
    }

    void RenderTextUTF8(const std::string& utf8, float x, float y, float scale = 1.0f, u32 color = 0xFFFFFFFF)
    {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxTextureSet(texture, 0.f, 0.f);

        unsigned char r = (color >> 24) & 0xFF;
        unsigned char g = (color >> 16) & 0xFF;
        unsigned char b = (color >> 8) & 0xFF;
        unsigned char a = color & 0xFF;

        AEGfxSetColorToMultiply(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

        AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);

        AEMtx33 I; AEMtx33Identity(&I);
        AEGfxSetTransform(I.m);

        std::vector<uint32_t> cps = DecodeUTF8(utf8);
        for (uint32_t cp : cps)
        {
            if (cp == '\n') continue;
            EnsureGlyph(cp);
        }

        UploadIfDirty();

        const int lineAdvance =
            (face->size && face->size->metrics.height)
            ? (int)(face->size->metrics.height >> 6)
            : (int)(face->height >> 6);

        float penX = x;
        float penY = y;
        const float sy = scale * yStretch; 

        AEGfxMeshStart();

        uint32_t prev = 0;
        for (uint32_t cp : cps)
        {
            if (cp == '\n')
            {
                penX = x;
                penY += (worldYAxisUp ? -1.f : +1.f) * (lineAdvance * sy); // ★ 줄간격에 sy
                prev = 0;
                continue;
            }

            if (prev && FT_HAS_KERNING(face))
            {
                FT_Vector k;
                FT_Get_Kerning(face,
                    FT_Get_Char_Index(face, prev),
                    FT_Get_Char_Index(face, cp),
                    FT_KERNING_DEFAULT, &k);
                penX += (k.x >> 6) * scale;
            }

            const Glyph* g = EnsureGlyph(cp);
            if (!g) { prev = 0; continue; }

            // ====== Y축 방향 토글 + 세로 스케일 적용 ======
            float x0 = penX + g->bearingX * scale;
            float y0, y1;

            if (worldYAxisUp) {
                // Y가 위로 증가하는 좌표계
                // bottom = baseline - (h - bearingY)
                float yBottom = penY - (g->h - g->bearingY) * sy;
                float yTop = yBottom + g->h * sy;
                y0 = yBottom;
                y1 = yTop;
            }
            else {
                // Y가 아래로 증가하는 좌표계
                // top = baseline + (h - bearingY)
                float yTop = penY + (g->h - g->bearingY) * sy;
                float yBottom = yTop - g->h * sy;
                y0 = yTop;
                y1 = yBottom;
            }

            float x1 = x0 + g->w * scale;

            // ====== V 플립 토글 ======
            float v0 = flipV ? g->v1 : g->v0;
            float v1 = flipV ? g->v0 : g->v1;

            AEGfxTriAdd(x0, y0, color, g->u0, v0,
                x1, y0, color, g->u1, v0,
                x1, y1, color, g->u1, v1);
            AEGfxTriAdd(x0, y0, color, g->u0, v0,
                x1, y1, color, g->u1, v1,
                x0, y1, color, g->u0, v1);

            penX += g->advance * scale;
            prev = cp;
        }

        AEGfxVertexList* vlist = AEGfxMeshEnd();
        AEGfxMeshDraw(vlist, AE_GFX_MDM_TRIANGLES);
        AEGfxMeshFree(vlist);
    }

private:
    // Packing
    int padding = 1;
    int atlasWidth = 0;
    int atlasHeight = 0;
    int penX = 0, penY = 0, rowH = 0;

    float yStretch = 1.0f; // ★ 세로 늘림 팩터(기본 1.0)

    // Data
    std::vector<uint8_t> pixels; // RGBA
    std::unordered_map<uint32_t, Glyph> glyphs;

    // FreeType
    FT_Library ft = nullptr;
    FT_Face    face = nullptr;

    // AlphaEngine
    AEGfxTexture* texture = nullptr;

    // Config
    bool worldYAxisUp = true; // 기본: +Y 위쪽. 필요 시 SetWorldYAxisUp(false)
    bool flipV = true;        // 기본: V축 뒤집기. 필요 시 SetUVFlipV(false)

    bool atlasDirty = false;
};
