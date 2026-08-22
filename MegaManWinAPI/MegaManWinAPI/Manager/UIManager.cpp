#include "pch.h"
#include "UIManager.h"
#include "UIElement.h"

void UIManager::AddUI(UIElement* ui)
{
	if (ui == nullptr) return;
	_uiElements.push_back(ui);
}

void UIManager::RemoveUI(UIElement* ui)
{
	std::erase_if(_uiElements, [ui](const UIElement* iter)
		{
			return iter == ui;
		});
}

void UIManager::Update(float deltaTime)
{
	for (auto& elem : _uiElements)
	{
		if (elem != nullptr)
		{
			if (elem->IsVisible() == true)
			{
				elem->Update(deltaTime);
			}
		}
	}
}

void UIManager::Render(ID2D1RenderTarget* renderTarget)
{
	for (auto& elem : _uiElements)
	{
		if (elem != nullptr)
		{
			if (elem->IsVisible() == true)
			{
				elem->Render(renderTarget);
			}
		}
	}
}

void UIManager::Cleanup()
{
	for (auto& elem : _uiElements)
	{
		if (elem != nullptr)
		{
			delete elem;
			elem = nullptr;
		}
	}

	_uiElements.clear();
}
