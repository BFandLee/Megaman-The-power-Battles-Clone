#include "pch.h"
#include "BossPhase1State.h"
#include "FSMComponent.h"
#include "Boss.h"
#include "Selector.h"
#include "Sequence.h"
#include "BossPhase1Nodes.h"
#include "ActionNode.h"
#include "DecoratorNode.h"
#include "CooldownDecorator.h"
#include "BossBlackboard.h"

BossPhase1State::BossPhase1State(FSMComponent* fsm) : State(fsm)
{
}

BossPhase1State::~BossPhase1State()
{
}

void BossPhase1State::Enter()
{
    // TODO: 페이즈 1 진입 시 초기화 (예: Phase 1 Behavior Tree 로드 및 실행)
    Selector* root = new Selector();
    Sequence* Attack = new Sequence();
    BTAction_Phase1_MagnetMissile* missileAction = new BTAction_Phase1_MagnetMissile();
    CooldownDecorator* decoator = new CooldownDecorator(3.0f);

    Attack->AddChild(missileAction);  // 시퀀스 아래에 액션 연결
    decoator->SetChild(Attack);   // 데코레이터 아래에 시퀀스 연결
    root->AddChild(decoator);     // 루트(셀렉터) 아래에 데코레이터 연결
    // 6. 멤버 변수에 저장
    _btroot = root;

}

void BossPhase1State::Update(float deltaTime)
{
    // TODO: Phase 1 로직 업데이트 (BT 업데이트)
    // TODO: HP가 50% 이하가 되면 Phase 2로 전환하는 로직 구현
    if (_btroot != nullptr)
    {
        BossBlackboard bb;
        bb.OwnerBoss = m_pOwnerFSM->GetOwner();
        _btroot->Tick(&bb);
    }
}

void BossPhase1State::Exit()
{
    // TODO: 페이즈 1 종료 시 정리 작업
    delete _btroot;
    if (_btroot != nullptr)
    {
        _btroot = nullptr;
    }
}
