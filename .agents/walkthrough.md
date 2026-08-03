# 록맨(MegaMan) 플레이어 아키텍처 스켈레톤 세팅 완료! 🎉

2주 차 메인 목표인 **"플레이어 상태 폭발(State Explosion) 방지 및 유연한 무기 시스템"** 구축을 위한 모든 기초 뼈대(Skeleton) 공사가 완료되었습니다!

## 완료된 작업 내역

### 1. FSM (상태 기계) 컴포넌트 뼈대 생성
플레이어의 복잡한 움직임(기능)과 자세를 깔끔하게 제어할 상태 패턴 기반 코드가 생성되었습니다.
- [FSMComponent.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Component/FSMComponent.h) / .cpp
- [State.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/FSM/State.h) (모든 상태의 부모 인터페이스)
- [IdleState.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/FSM/IdleState.h) / .cpp (대기 상태 예시)

### 2. Weapon (무기) 컴포넌트 뼈대 생성
움직임(하체)과 완벽하게 독립적으로 동작하는 전략 패턴 기반의 무기 시스템 코드가 생성되었습니다.
- [WeaponComponent.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Component/WeaponComponent.h) / .cpp
- [Weapon.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Weapon/Weapon.h) (모든 무기의 부모 인터페이스)
- [Buster.h](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Weapon/Buster.h) / .cpp (기본 콩알탄 예시)

### 3. Player 오브젝트에 심장(Component) 부착
[Player.cpp](file:///d:/GitHub/Megaman-The-power-Battles-Clone/MegaManWinAPI/MegaManWinAPI/Entity/Actor/Player.cpp) 파일의 `Init()` 함수에 방금 만든 두 가지 핵심 컴포넌트를 `AddComponent`를 통해 든든하게 부착해 두었습니다.

> [!TIP]
> **이제부터 유저님께서 직접 코딩하실 차례입니다!**
> 1. `Player.cpp`의 `Update()` 함수를 열어보시면 제가 남겨둔 `// TODO:` 주석이 있습니다. 기존에 길게 나열되어 있던 지저분한 키보드 입력 로직을 모두 과감하게 지우고, 방금 만들어드린 `IdleState`, `MoveState` 안으로 코드를 옮겨보세요!
> 2. FSM 상태 내부에 선언해둔 `TODO` 주석들을 따라가며 로직을 채워보세요. 
> 3. 코딩 중 막히는 부분이 생기면 언제든지 코드를 보여주세요. 코치로서 방향을 다시 잡아드리겠습니다!
