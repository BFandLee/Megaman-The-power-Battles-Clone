#include "pch.h"
#include "BossPhase1State.h"
#include "FSMComponent.h"
#include "Boss.h"
#include "Selector.h"
#include "Sequence.h"
#include "GeminiPatten.h"
#include "ActionNode.h"
#include "DecoratorNode.h"
#include "CooldownDecorator.h"
#include "BossBlackboard.h"
#include "BTSerializer.h"
#include "TransformComponent.h"

BossPhase1State::BossPhase1State(FSMComponent* fsm) : State(fsm)
{
}

BossPhase1State::~BossPhase1State()
{
}

void BossPhase1State::Enter()
{
    _rootNode = BTSerializer::LoadFromJSON("Phase1Nodes.json");
    _boss = static_cast<Boss*>(m_pOwnerFSM->GetOwner());
}

void BossPhase1State::Update(float deltaTime)
{
    if (_boss == nullptr)
    {
        return;
    }

    if (m_pOwnerFSM == nullptr)
    {
        return;
    }

    BossBlackboard* bb = _boss->GetBlackboard();

    if (bb == nullptr)
    {
        return;
    }

    if (bb->BossTransform == nullptr)
        return;
    
    if (_boss->IsHit())
    {
        return;
    }

    if (_boss->GetHP() <= _boss->GetMaxHP() * 0.5f)
    {
        m_pOwnerFSM->ChangeState("Phase2");
        return;
    }

    if (_rootNode && bb)
    {
        _rootNode->Execute(bb);
    }
}

void BossPhase1State::Exit()
{

    if (_rootNode)
    {
        delete _rootNode;
        _rootNode = nullptr;
    }
}
