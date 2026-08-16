#include "pch.h"
#include "BTState.h"
#include "BTNode.h"
#include "Blackboard.h"
#include "BossBlackboard.h"
#include "BTSerializer.h"

void BTState::Update(float deltaTime)
{
	if (_rootNode != nullptr)
	{
		if (_bb != nullptr)
		{
			_rootNode->Execute(_bb);
		}
	}
	
}

void BTState::Enter()
{
	_bb = new BossBlackboard();
	_rootNode = BTSerializer::LoadFromJSON("Phase1Nodes.json");
}

void BTState::Exit()
{
	delete _bb;
	if (_bb != nullptr)
	{
		_bb = nullptr;
	}
}

void BTState::RenderUI()
{
	ImGui::Begin("Boss Behavior Tree");

	if (_rootNode != nullptr)
	{
	}
	ImGui::End();
}
