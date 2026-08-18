#pragma once
#include "Actor.h"

class Player : public Actor
{
	using Super = Actor;

public:
	Player() : Actor("Player") {}
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	
	// 충돌 처리 함수
	virtual void OnStay(Actor* other, const HitResult& hit) override;
	virtual void OnExit(Actor* other) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Player; }
	virtual ActorType GetActorType() override { return ActorType::Player; }

public:
	virtual void SetLookDirX(float dir) override;
	float GetLookDirX() { return _lookdirX; }

	// [피격/죽음 관련 인터페이스]
	virtual void TakeDamage(int damage, float hitDirX = 0.0f) override;
	int GetHp() const { return _hp; }
	void SetHp(int hp) { _hp = hp; }
	int GetMaxHp() const { return _maxHp; }
	float GetHitDirX() { return _hitdirX; }

	void SetInvincible(bool isInvincible) { _isInvincible = isInvincible; }

private:
	float _lookdirX = 1.0f;

	// [피격/죽음 관련 멤버 변수]
	int _hp = 100;
	int _maxHp = 100;
	float _hitdirX;
	bool _isInvincible = false;
	float _invincibleTimer = 0.0f;

	float _chargeTime;
	float _blinkTime;
	bool _isYellowColor;

	bool _isInit = false;

};
