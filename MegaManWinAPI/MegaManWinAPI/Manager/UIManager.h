#pragma once
#include "Singleton.h"
class UIElement;
class UIManager : public Singleton<UIManager>
{
	friend Singleton<UIManager>;
private:
	UIManager() = default;
	~UIManager() = default;
public:
	void AddUI(UIElement* ui);
	void RemoveUI(UIElement* ui);
	void Update(float deltaTime);
	void Render(ID2D1RenderTarget* renderTarget);
	void Cleanup();
private:
	vector<UIElement*> _uiElements;
};

