#pragma once
#include "Actor.h"

class Background : public Actor
{
	using Super = Actor;

	virtual RenderLayer GetRenderLayer() override {};
	virtual ActorType GetActorType() override {};
};

