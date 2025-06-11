#include "Utils.h"

AEGfxVertexList* Utils::CreateMesh(int row, int column)
{
	AEGfxVertexList* Mesh;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f / static_cast<float>(row), 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f - 1.0f / static_cast<float>(column));

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f / static_cast<float>(row), 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f / static_cast<float>(row), 1.0f - 1.0f / static_cast<float>(column),
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f - 1.0f / static_cast<float>(column));

	Mesh = AEGfxMeshEnd();

	return Mesh;
}

void DestroyMesh(AEGfxVertexList* Mesh)
{
	if(Mesh)
		AEGfxMeshFree(Mesh);
}
