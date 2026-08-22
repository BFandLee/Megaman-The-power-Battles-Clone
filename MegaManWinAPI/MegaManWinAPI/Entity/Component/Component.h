#pragma once

// Actor를 구성하는 기능들
class Component
{
public:
	Component(string name);
	
	virtual void Init() {}
	virtual void Update(float deltaTime)	 {}
	virtual void Render(ID2D1RenderTarget* renderTarge) {}
	virtual void RenderUI() {}
	
	// Json
	virtual json ToJson();
	virtual void FromJson(const json& j);

public:
	void SetOwner(class Actor* owner) { _owner = owner; }
	class Actor* GetOwner() { return _owner; }
	

private:
	class Actor* _owner = nullptr;		// 자신이 속한 Obejct
	string _name;
};

