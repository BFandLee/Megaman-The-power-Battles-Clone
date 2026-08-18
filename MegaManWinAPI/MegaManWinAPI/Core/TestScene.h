#pragma once
#include "Scene.h"
class TestScene : public Scene
{
	using Super = Scene;
public:
	virtual ~TestScene() = default;

	virtual void Update(float deltaTime) override;

protected:
	virtual void loadResources() override;
	virtual void createObjects() override;
	virtual void createUI() override;

private:
	class ContinueUI* _continueUI = nullptr;
	class PlayerHpBarUI* _playerHpBar = nullptr;
	class BossHpBarUI* _bossHpBar = nullptr;

};

