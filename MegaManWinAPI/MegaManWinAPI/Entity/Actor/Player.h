#pragma once
#include "Actor.h"

// TODO 1: Player의 상태를 나타내는 열거형을 선언하세요.
// 예: Idle, Run, Jump, Fall, Attack 등
enum class PlayerState
{
	Idle,
	Run,
	Jump,
	// ... (필요에 따라 추가)
};

class Player : public Actor
{
	using Super = Actor;

public:
	Player() : Actor("Player") {}
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	
	// 충돌 처리 함수
	virtual void OnStay(Actor* other, const HitResult& hit) override;
	virtual void OnExit(Actor* other) override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Player; }
	virtual ActorType GetActorType() override { return ActorType::Player; }

private:
	PlayerState _state;
	float _speed = 300.f; // 플레이어의 이동 속도
	class RigidBodyComponent* _rigidbody;
};
