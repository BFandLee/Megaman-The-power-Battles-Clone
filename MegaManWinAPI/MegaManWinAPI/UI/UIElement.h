#pragma once
class UIElement
{
public:
	virtual ~UIElement() = default;
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1RenderTarget* renderTarget);
	virtual void Cleanup();

public:
	void SetPos(Vector pos) { _pos = pos; }
	Vector GetPos() { return _pos; }
	void SetScale(Vector scale) { _scale = scale; }
	Vector GetScale() { return _scale; }
private:
	Vector _pos{ 0.0f, 0.0f };
	Vector _scale{ 1.0f, 1.0f };
	bool _isVisible = false;
};

