#pragma once
#include "Actor.h"
class Player : public Actor
{
public:
	virtual void Init() override;

	virtual RenderLayer GetRenderLayer() override { return RenderLayer::Player; }
	virtual ActorType GetActorType() override { return ActorType::Player; }
};

