---
name: week1
description: 기존에 만들어둔 엔진 수준의 코드들을 MegaManWinAPI 프로젝트로 옮기고 기본 게임 루프가 돌아가게 만들어야 합니다.
---

# 1주차 목표: 프레임워크 뼈대 이식

이번 주차의 핵심 목표는 사용자가 기존 `Game1945` 또는 `BounceBall` 프로젝트에서 엔진 수준의 코드들을 `MegaManWinAPI` 프로젝트로 직접 복사하고, 기본 게임 루프가 정상적으로 동작하도록 구성하는 것입니다.

## 에이전트 행동 지침 (Agent Guidelines)

이 스킬이 활성화되면 에이전트는 다음 단계에 따라 작업을 진행하고 사용자를 코칭해야 합니다.

### 1단계: 파일 이식 안내 (User Action Required)
사용자에게 아래 핵심 파일 목록을 `MegaManWinAPI/MegaManWinAPI` 폴더로 복사하고, **Visual Studio 솔루션 탐색기에서 '기존 항목 추가'를 통해 프로젝트에 포함**시키도록 안내합니다. 
(※ 에이전트가 직접 스크립트로 파일을 복사하지 않으며, 안내 후 사용자가 복사 및 추가를 완료했다고 응답할 때까지 대기합니다.)

**이식해야 할 핵심 파일 목록:**
- **코어 및 루프:** `Game.h` / `Game.cpp`, `Scene.h` / `Scene.cpp`
- **엔티티 시스템:** `Actor.h` / `Actor.cpp`, `Component.h` / `Component.cpp`
- **매니저 클래스 (싱글톤):**
  - `TimeManager.h` / `TimeManager.cpp`
  - `InputManager.h` / `InputManager.cpp`
  - `ResourceManager.h` / `ResourceManager.cpp`
  - `CollisionManager.h` / `CollisionManager.cpp`
  - `SoundManager.h` / `SoundManager.cpp` (사운드 재생용)
- **렌더링 시스템:** `SpriteRenderer.h` / `SpriteRenderer.cpp`, `ImageRenderer.h` / `ImageRenderer.cpp`
- **기타 유틸:** `Singleton.h`, `Util.h` / `Util.cpp`

### 1-2단계: 추가 시스템 구현 및 업그레이드 (New!)
단순 이식을 넘어 록맨(MegaMan) 모작에 필수적인 기능들을 1주차에 함께 세팅하도록 안내합니다.
- **GDI+ 도입:** 투명도(알파) 렌더링, 회전 및 스케일 변경을 위해 기존 GDI 기반 렌더러를 **GDI+** 기반으로 업그레이드하도록 유도합니다. (`Game` 클래스 초기화 시 GDI+ 스타트업 코드 추가 포함)
- **AnimatorComponent 생성:** 캐릭터의 다양한 상태(대기, 달리기, 점프 등)에 따른 프레임 애니메이션을 전문적으로 처리할 `AnimatorComponent` 클래스의 설계를 제안하고 작성을 유도합니다.

### 2단계: 코드 호환성 검토 및 수정 제안
사용자가 파일 추가를 완료하면, 에이전트는 이식된 파일들이 새 프로젝트 환경에 맞게 동작하도록 코드를 검토하고 **수정 사항을 제안**해야 합니다.
- **주요 검토 사항:**
  - `pch.h` (미리 컴파일된 헤더) 포함 관련 경로 설정 확인
  - 네임스페이스 및 기존 프로젝트 종속성 제거 (필요시)
  - 새로운 MegaMan 프로젝트 구조에 맞춘 include 경로 수정
  - `Game` 클래스의 메인 게임 루프 구성 확인

### 3단계: 완료 검증
코드 수정 제안 및 반영이 마무리되면, 에이전트는 다음을 검증하여 1주차 목표의 완료 여부를 확인합니다.
1. 지정된 핵심 파일들이 올바른 프로젝트 경로에 존재하는지 확인.
2. 코드 오류 없이 **프로젝트가 정상적으로 빌드되는지** 확인.
3. 빌드 후 실행 시 기본 윈도우 창이 정상적으로 렌더링되며 루프가 도는지 확인.