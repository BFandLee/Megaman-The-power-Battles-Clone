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
	virtual void TakeDamage(float damage, float hitDirX = 0.0f) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Enemy; }
	virtual ActorType GetActorType() override { return ActorType::Enemy; }

    // Phase 관리를 위한 Getter/Setter
    float GetHP() const { return _hp; }
    float GetMaxHP() const { return _maxHp; }

	class BossBlackboard* GetBlackboard() const { return _bb; }

	// Phase 1 패턴 액션용 인터페이스 (BT 노드에서 호출됨)
	void SetInvincible(bool isInvincible);
	void PullPlayer();
	void ActivateShield();
	void JumpTo(struct Vector dest); // Vector 구조체는 미리 선언되어 있다고 가정
	void ShootMagnetMissile();

private:
	FSMComponent* _fsm = nullptr;
	class BossBlackboard* _bb = nullptr;
	// TODO: 보스의 체력(HP), 무적 상태, 피격 상태, 기타 필요한 멤버 변수를 선언하세요.
    float _hp = 100.0f;
    float _maxHp = 100.0f;
	bool _isInvincible = false;
    
};
