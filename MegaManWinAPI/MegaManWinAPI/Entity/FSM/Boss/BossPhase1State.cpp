#include "pch.h"
#include "BossPhase1State.h"
#include "FSMComponent.h"
#include "Boss.h"
#include "Selector.h"
#include "Sequence.h"
#include "GeminiPhase1Nodes.h"
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
    BossBlackboard* bb = _boss->GetBlackboard();

    float PlayerX = bb->PlayerTransform->GetPos().x;
    float BossX = bb->BossTransform->GetPos().x;
    bb->DirXToPlayer = (PlayerX > BossX) ? 1.0f : -1.0f;
    if (_rootNode && bb)
    {
        _rootNode->Tick(bb);
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
