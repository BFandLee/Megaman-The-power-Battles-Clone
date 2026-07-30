#pragma once
#include "Scene.h"
class TestScene : public Scene
{
public:
	virtual ~TestScene() = default;

protected:
	virtual void loadResources() override;
	virtual void createObjects() override;
};

