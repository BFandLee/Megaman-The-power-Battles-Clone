#include "pch.h"
#include "BTState.h"
#include "BTNode.h"

void BTState::Update(float deltaTime)
{
	// _rootNode->Tick();
}

void BTState::Enter()
{
}

void BTState::Exit()
{
}

void BTState::RenderUI()
{
	ImGui::Begin("Boss Behavior Tree");

	if (_rootNode != nullptr)
	{
	}
	ImGui::End();
}
