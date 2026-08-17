#pragma once
#include "Actor.h"

class FSMComponent;

class Boss : public Actor
{
	using Super = Actor;

public:
	Boss();
	virtual ~Boss();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void TakeDamage(int damage, float hitDirX = 0.0f) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Enemy; }
	virtual ActorType GetActorType() override { return ActorType::Boss; }

	virtual void OnStay(Actor* other, const HitResult& hit) override;
	virtual void OnExit(Actor* other) override;

    // Phase 관리를 위한 Getter/Setter
    float GetHP() const { return _hp; }
	void SetHP(float hp) { _hp = hp; }
    float GetMaxHP() const { return _maxHp; }

	class BossBlackboard* GetBlackboard() const { return _bb; }

	// Phase 1 패턴 액션용 인터페이스 (BT 노드에서 호출됨)
	void SetInvincible(bool isInvincible);

	virtual void SetLookDirX(float dir) override;

	const bool IsHit() { return _isHit; }
private:
	FSMComponent* _fsm = nullptr;
	class BossBlackboard* _bb = nullptr;
    float _hp = 100.0f;
    float _maxHp = 100.0f;
	bool _isInvincible = false;
	float _invincibleTimer = 0.0f;
	bool _isHit = false;
};
