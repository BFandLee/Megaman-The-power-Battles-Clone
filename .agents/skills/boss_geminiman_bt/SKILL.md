---
name: Boss Gemini Man BT Design
description: 제미니맨(Gemini Man)의 FSM 및 Phase 1, Phase 2 비헤이비어 트리(BT) 구조 설계 가이드입니다.
---

# Gemini Man Behavior Tree Design Guide

본 지침은 `BossAI BT.png` 다이어그램을 바탕으로 제미니맨(Gemini Man)의 AI 패턴을 구현할 때 준수해야 할 트리 구조와 핵심 노드들을 정의합니다.

## 1. FSM (최상위 상태 머신)
- **Spawn State:** 보스 등장 상태
- **Phase 1 State:** 노말 페이즈 (HP > 50%) -> `Phase 1 BT` 실행
- **Phase 2 State:** 발악 페이즈 (HP <= 50%) -> `Phase 2 BT` 실행
- **Dead State:** 사망 상태 (HP == 0)

## 2. Phase 1 BT (노말 페이즈)
루트 노드: `[Selector] Phase 1 Root`
1. **[Sequence] 기본점프:**
   - [Condition] 분신 쿨다운 & 확률 체크 (`ProbabilityDecorator`, `CooldownDecorator` 활용)
   - (성공 시) [Action] 분신 활성화 (`BTAction_Gemini_CloneActivate` - 분신 Actor Spawn)
   - [Action] 점프 이동 (`BTAction_Gemini_JumpMove`)
2. **[Sequence] Base 미사일 발사:**
   - [Condition] Base 쿨다운 체크
   - (성공 시) [Action] Base 미사일 발사 (`BTAction_Gemini_BaseMissile`)
3. **[Sequence] 기본 이동:**
   - [Condition] 분신 ON 체크 (`CheckCloneDecorator` 등 구현 예정) -> (성공 시) [Action] 점프 (`BTAction_Gemini_JumpMove`)
   - [Condition] 확률 체크 -> (성공 시) [Action] Base 미사일 발사 (`BTAction_Gemini_BaseMissile`)

## 3. Phase 2 BT (HP 50% 이하)
루트 노드: `[Selector] Phase 2 Root`
1. **분신 해제 시퀀스:**
   - [Condition] 유지 시간 & 확률 체크
   - [Condition] 분신 여부 체크
   - (성공 시) [Action] 분신 해제 (`BTAction_Gemini_CloneDeactivate` - 분신 Actor Destroy)
2. **[Selector] 분신/미사일 패턴:**
   - [Condition] 분신 쿨다운 & 확률 -> [Action] 분신 소환 (`BTAction_Gemini_CloneSummon`)
   - [Condition] Base 쿨다운 & 확률 -> [Action] Base 미사일 발사 (`BTAction_Gemini_BaseMissile`)
3. **[Selector] 미사일 발사 패턴:**
   - [Condition] 레이저 쿨다운 & 확률 -> [Action] 레이저 미사일 발사 (`BTAction_Gemini_LaserMissile` - 벽 튕김)
   - (실패 시) -> [Action] Base 미사일 발사 (`BTAction_Gemini_BaseMissile`)
4. **[Sequence] 기본 이동 패턴:**
   - [Action] 점프 -> [Action] 점프 -> [Action] 이동 (`BTAction_Gemini_SequentialJump`)

## 4. 구현시 유의사항 (ActionNode 구체화)
- **분신(Clone):** 분신 활성화, 소환 시 새로운 `Actor` 객체를 `Scene`에 **Spawn**하는 방식으로 구현합니다. 분신의 상태(활성화 여부)는 Blackboard를 통해 관리해야 합니다.
- **쿨다운 및 확률(Condition):** 이미 기구현된 `CooldownDecorator.h`와 `ProbabilityDecorator.h`를 적극 활용하여 조립합니다.
- **특수 공격:** 벽에 튕기는 '레이저 미사일'의 물리 및 충돌 로직이 ActionNode 또는 투사체(Projectile) Actor 내부에서 올바르게 처리되어야 합니다.
