
# 게임 개발 마일스톤 플랜 & 게임 문서

## Development Plan

| 마일스톤 | 주차 | 기간 | 목표 | 주요 작업 | 비고 |
|----------|------|-------|-------|------------|------|
| 개발 | 1주차 | 06-09 ~ 06-15 | - 기본적인 시스템 구현 | - 게임 내부 오브젝트들의 클래스 설계<br>- 기본 플레이 흐름 구축 |  |
| 개발 | 2주차 | 06-16 ~ 06-22 | - 게임 핵심 루프 설계<br>- 아이템 기획 및 구현 | - 1 스테이지 구현(보스 및 적 ai 추가)<br>- 아이템 class 생성 및 아이템 구현 |  |
| 개발 | 3주차 | 06-23 ~ 06-29 | - 2,3 스테이지 구현<br>- 아이템 기획 및 구현<br>- 기본 무기 강화 구현 | - 2 스테이지 보스 및 적 구현 <br>- 태그별 아이템 구현 <br>- 스탯 공식 구현 및 적용 <br>- 플레이어 총기 구현<br>- 포션 및 체력 UI 구현|  |
| 개발 | 4주차 | 06-30 ~ 07-06 | - 2,3 스테이지 구현<br>- 아이템 기획 및 구현<br>- 기본 무기 강화 구현 <br>- 사운드 삽입 | |  |
| 폴리싱 | 5주차 | 07-07 ~ 07-13 | - 게임 내부 밸런스 조정<br>- 내부 애니메이션 구현<br>- 사운드 삽입<br>- 디버깅 | |  |
| 폴리싱 | 6주차 | 07-14 ~ 07-16 | - 게임 내부 밸런스 조정<br>- 디버깅 | |  |

---

## Daily Report 링크

> https://docs.google.com/spreadsheets/d/1iiQpPAQpZT1S8zdqiECrYfTQu0Vxf6EsAT6ODF53S9g/edit?usp=sharing

---

## 게임 한 줄 소개

**총기를 강화하고, 아이템을 모아 빌드를 만들어 스테이지 및 보스를 클리어하는 탑뷰 로그라이크 게임입니다.**

---

## 참고한 게임 목록

- 아이작의 번제 (게임 시점)
- 세피리아 (무기 성장과 같은 게임 시스템)
- 세피리아의 시스템을 많이 참고하게 될 것 같습니다. 
- 아이템별 태그가 존재하고, 동일 태그의 아이템을 모을 시 추가 효과를 얻음.
- 플레이어의 시작 무기가 게임 진행에 따라 변화함.
---

## 핵심 플레이 루프

1. 일정 시간 동안 적을 처치해서 경험치를 획득.
2. 경험치를 통해 아이템을 획득하고 아이템을 모아 빌드를 만듬.
3. 일정 시간이 지난 후 등장하는 보스를 처치.
4. 보스 처치 시 다음 스테이지로 진입. 이를 게임 클리어까지 반복

---

## 타겟 유저층

- 로그라이크를 선호하는 유저

---

## 플레이 영상

- [1주차 영상](https://drive.google.com/file/d/1bHSuhmY9qJ8aNfNPiAnHpkBafLchmZBD/view?usp=sharing)
- [2주차 영상](https://drive.google.com/file/d/1WxueMh_DTdlNmREtKTrDE5OeJRer1ju2/view?usp=sharing)
- [3주차 영상](https://drive.google.com/file/d/1Q3lvfbdrs5O1BvOEBNIxgRsrHhmME7O_/view?usp=sharing)
- [4주차 영상](https://drive.google.com/file/d/1wjRcj9kd4gMJ_tf38YrJvPPhl_USKL-y/view?usp=sharing)
- [5주차 영상](https://drive.google.com/file/d/1xIbyPrvNlQEyGXtXgxhwnyqVttL9sR33/view?usp=sharing)

---

## Asset 출처

- [Explosion Animations Pack](https://ansimuz.itch.io/explosion-animations-pack)
- [Synthesized Water](https://freesound.org/people/JomelleJager/sounds/383145/)
- [Button FX by R. Mocci](https://rmocci.itch.io/button-fx)
- [rpg-essentials-sfx-free](https://leohpaz.itch.io/rpg-essentials-sfx-free)
- [Universal UI/Menu Soundpack](https://cyrex-studios.itch.io/universal-ui-soundpack)
- [Vignetting](https://forum.affinity.serif.com/index.php?/topic/114423-to-create-a-vignette-with-the-gradients-tools/)
- [Slime sound](https://freesound.org/people/greenlinker/sounds/794272/)