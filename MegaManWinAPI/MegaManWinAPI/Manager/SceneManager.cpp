#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LobbyScene.h"
#include "TestScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"

void SceneManager::Init()
{
	_scene = createScene(SceneType::Lobby);
	_scene->Init();
}

void SceneManager::Cleanup()
{
	if (_scene)
	{
		_scene->Cleanup();
		delete _scene;
		_scene = nullptr;
	}
}

void SceneManager::Update(float deltaTime)
{
	applySceneChange();

	if (_scene)
	{
		_scene->Update(deltaTime); 

		_scene->PostUpdate();
	}
}

void SceneManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (_scene)
	{
		_scene->Render(renderTarget);
	}
}

void SceneManager::ChangeScene(SceneType type)
{
	_nextSceneType = type;
}

Scene* SceneManager::createScene(SceneType type)
{
	switch (type)
	{
	case SceneType::Lobby: return new LobbyScene();
	case SceneType::Test: return new TestScene();
	case SceneType::GameOver: return new GameOverScene();
	case SceneType::GameClear: return new GameClearScene();
	}

	return nullptr;
}

void SceneManager::applySceneChange()
{
	if (_nextSceneType == SceneType::Max)
		return;

	if (_scene)
	{
		_scene->Cleanup();
		delete _scene;
	}

	_scene = createScene(_nextSceneType);
	_scene->Init();

	_nextSceneType = SceneType::Max;
}

void SceneManager::RenderUI()
{
	if (_scene != nullptr)
	{
		_scene->RenderUI();
	}
}
