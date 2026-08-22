#pragma once
#include "Scene.h"
class TestScene : public Scene
{
	using Super = Scene;
public:
	virtual ~TestScene() = default;
	virtual void Init() override;
	virtual void Update(float deltaTime) override;

protected:
	virtual void loadResources() override;
	virtual void createObjects() override;
	virtual void createUI() override;

private:
	class ContinueUI* _continueUI = nullptr;
	class PlayerHpBarUI* _playerHpBar = nullptr;
	class BossHpBarUI* _bossHpBar = nullptr;

	int _spawnCount = 0;
	const int _maxSpawnCount = 4; // 최초 1회 + 컨티뉴 부활 3회 = 총 4회 스폰 허용
	float _gameOverDelayTimer = 0.0f;
	bool _isWaitingGameOver = false;

	float _gameClearDelayTimer = 0.0f;
	bool _isWaitingGameClear = false;
};
