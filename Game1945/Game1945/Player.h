#pragma once
#include "Airplane.h"

class Player : public Airplane
{
	using Super = Airplane;
public:
	void Init();
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
	virtual void OnEnter(Actor* other) override;
	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Player; }
	virtual ActorType GetActorType() override { return ActorType::Player; }
	int32 GetHp() const { return _hp; }
	int32 GetMaxHp() const { return 100; }
private:
	void move(float x, float y);
	void takeDamage();

private:
	int32 _attack = 100;	// °ø°Ý·Â
	float _moveSpeed = 300;

	int32 _hp = 100;
};


