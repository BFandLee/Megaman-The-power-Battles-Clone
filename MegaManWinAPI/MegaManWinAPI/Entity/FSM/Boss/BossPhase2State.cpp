#include "pch.h"
#include "BossPhase2State.h"
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
#include "ResourceManager.h"

BossPhase2State::BossPhase2State(FSMComponent* fsm) : State(fsm)
{
}

BossPhase2State::~BossPhase2State()
{
}

void BossPhase2State::Enter()
{
    _rootNode = BTSerializer::LoadFromJSON(ResourceManager::GetInstance().GetBTPath("Phase2Nodes.json"));
    _boss = static_cast<Boss*>(m_pOwnerFSM->GetOwner());
}

void BossPhase2State::Update(float deltaTime)
{
    if (_boss == nullptr || m_pOwnerFSM == nullptr) return;
    BossBlackboard* bb = _boss->GetBlackboard();
    if (bb == nullptr || bb->BossTransform == nullptr) return;

    if (_boss->IsHit()) return;

    if (_boss->GetHP() <= 0)
    {
        m_pOwnerFSM->ChangeState("Dead");
        return;
    }

    if (_rootNode && bb)
    {
        _rootNode->Execute(bb);
    }
}

void BossPhase2State::Exit()
{
    if (_rootNode != nullptr)
    {
        delete _rootNode;
        _rootNode = nullptr;
    }
}
