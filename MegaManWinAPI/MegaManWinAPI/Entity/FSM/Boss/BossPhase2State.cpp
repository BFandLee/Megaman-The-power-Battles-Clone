#include "pch.h"
#include "BossPhase2State.h"
#include "Entity/Component/FSMComponent.h"
#include "Entity/Actor/Boss.h"

BossPhase2State::BossPhase2State(FSMComponent* fsm) : State(fsm)
{
}

BossPhase2State::~BossPhase2State()
{
}

void BossPhase2State::Enter()
{
    // TODO: 페이즈 2 진입 시 초기화 (예: 발악 패턴 등 Phase 2 Behavior Tree 로드 및 실행)
}

void BossPhase2State::Update(float deltaTime)
{
    // TODO: Phase 2 로직 업데이트 (BT 업데이트)
    // TODO: HP가 0 이하가 되면 Dead 상태로 전환하는 로직 구현
}

void BossPhase2State::Exit()
{
    // TODO: 페이즈 2 종료 시 정리 작업
}
