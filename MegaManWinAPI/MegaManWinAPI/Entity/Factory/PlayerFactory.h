#pragma once
#include "ActorFactory.h"
class PlayerFactory : public ActorFactory
{
public:
	virtual Actor* CreateActor(Vector pos) override;
};
