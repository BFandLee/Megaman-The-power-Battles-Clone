#include "pch.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "CollisionManager.h"
#include "UIManager.h"
#include "Background.h"
#include "Actor.h"
#include "Player.h"
#include "Ground.h"
#include "WallActor.h"
#include "ActorFactory.h"
#include "PlayerFactory.h"
#include "Boss.h"
#include "Effect.h"
#include "AnimatorComponent.h"


Scene::Scene() 
{
}
Scene::~Scene()
{
}

void Scene::Init()
{
	_actorFactory["Player"] = []() -> class Actor* {
		ActorFactory* factory = new PlayerFactory();
		Actor* player = factory->CreateActor(Vector(0, 0)); // 팩토리가 완벽히 세팅해줌
		delete factory;
		return player;
		};

	RegisterActor<Background>("Background");
	RegisterActor<Ground>("Ground");
	RegisterActor<WallActor>("WallActor");
	RegisterActor<Boss>("Boss");


	// Scene에 필요한 리소스 로드
	loadResources();

	// Scene에 필요한 객체 생성
	
	if (LoadScene("SceneData.json") == false)
	{
		createObjects();
	}

	createUI();
}

void Scene::Cleanup()
{
	// 예약된 추가 리스트 정리
	for (auto actor : _reservedAdd)
	{
		if (actor->GetPool() == nullptr)
		{
			delete actor;
		}
	}
	_reservedAdd.clear();
	_reservedRemove.clear();
	_postUpdateActions.clear();

	// 씬에 존재하는 모든 객체들의 delete 담당
	for (auto iter : _actors)
	{
		// Scene이 new 한 객체는 delete 해도 된다.
		if (iter->GetPool() == nullptr)
		{
			delete iter;
		}
	}
	_actors.clear();

	for (auto& iter : _renderList)
	{
		iter.clear();
	}

	// 씬 전환 시 충돌 매니저 내부 등록 목록 완전히 클리어
	CollisionManager::GetInstance().Clear();

	UIManager::GetInstance().Cleanup();
}

void Scene::Update(float deltaTime)
{
	if(!_isPaused)
	{
		for (auto actor : _actors)
		{
			if (!actor->GetActive())
				continue;

			actor->Update(deltaTime);
		}
		UIManager::GetInstance().Update(deltaTime);
	}

	std::erase_if(_actors, [this](Actor* actor)
		{
			return _reservedRemove.contains(actor);
		});

	for (auto deleteActor : _reservedRemove)
	{
		// 삭제되는 Actor
		removeActor(deleteActor);

		// 해당 Actor가 풀에서 태어난 경우에는 반환
		if (deleteActor->GetPool())
		{
			// 해당 풀에다가 반환
			deleteActor->GetPool()->Return(deleteActor);
		}
		else
		{
			// new 태어난 경우는 delete
			delete deleteActor;
		}
	}


	//-> 미리 한번만 할당해놓고, 복사하기
	_actors.reserve(_actors.size() + _reservedAdd.size()); // 개수X, Capacity(메모리)
	for (Actor* actor : _reservedAdd)
	{
		// 추가되는 Actor
		registerActor(actor);		
		_actors.push_back(actor);	// reserve() 함수로 미리 capacity 확보해뒀다.
	}

	// 지연리스트 초기화
	_reservedAdd.clear();
	_reservedRemove.clear();

}

void Scene::Render(ID2D1RenderTarget* renderTarget)
{
	// 명확한 렌더링 순서를 지키기 위해 별도의 리스트 순서대로 그린다.
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			if (!actor->GetActive())
				continue;
			
			actor->Render(renderTarget);
		}
	}

	UIManager::GetInstance().Render(renderTarget);

}

void Scene::PostUpdate()
{
	// 후처리 액션 수행 (std::function)
	for (auto& action : _postUpdateActions)
	{
		// 함수포인터 호출 : 결합도 느슨해진다.
		action();
	}
	_postUpdateActions.clear();
}

void Scene::AddPostUpdateAction(std::function<void()> action)
{
	_postUpdateActions.push_back(action);
}

void Scene::AddActor(class Actor* actor)
{
	// 즉시 추가안하고, 안전하게 모든 update다 돌고나서 추가하자.
	_reservedAdd.push_back(actor);
}

void Scene::DeleteActor(Actor* actor)
{
	// 지연 삭제
	// 추가적인 리스트에 넣고, 나중에 한번에 삭제
	_reservedRemove.insert(actor);
}


Actor* Scene::FindActorByType(ActorType type) const
{
	for (Actor* actor : _actors)
	{
		if (actor->GetActorType() == type)
		{
			return actor;
		}
	}

	return nullptr;
}

void Scene::RemoveAllActor()
{

	// 예약된 추가 리스트
	for (auto actor : _reservedAdd)
	{
		delete actor;
	}
	_reservedAdd.clear();

	// 예약된 삭제 리스트
	_reservedRemove.clear();

	// 현재 추가된 리스트
	for (auto iter : _actors)
	{
		delete iter;
	}
	_actors.clear();

	for (auto& iter : _renderList)
	{
		iter.clear();
	}

	// 충돌 매니저도 clear
	CollisionManager::GetInstance().Clear();
}


const vector<Actor*>& Scene::GetRenderList(RenderLayer layer) const
{
	if ((int32)layer < 0 || layer >= RenderLayer::Count)
	{
		static vector<Actor*> emptyList;
		return emptyList;
	}
	return _renderList[(int32)layer];
}

void Scene::loadResources()
{
	
}

void Scene::createObjects()
{

}

// Scene에 등록되는 Actor들이 모두 해야할일
void Scene::registerActor(Actor* actor)
{
	if (actor == nullptr) return;
	_renderList[(int32)actor->GetRenderLayer()].push_back(actor);

	if (actor->GetCollider())
	{
		CollisionManager::GetInstance().AddActor(actor);
	}
}

void Scene::removeActor(Actor* actor)
{
	if (actor == nullptr) return;

	auto& layerList = _renderList[(int32)actor->GetRenderLayer()];

	erase(layerList, actor);

	CollisionManager::GetInstance().RemoveActor(actor);
}



Actor* Scene::CreateActor(ActorType type)
{
	switch (type)
	{
		case ActorType::Background:
		{
			Background* bg = new Background();
			bg->Init();
			return bg;
		}

		case ActorType::Player:
		{
			/*Player* player = new Player();
			player->Init();
			return player;*/
		}
	}

	return nullptr;
}

void Scene::RenderUI()
{
	ImGui::Begin("Hierarchy");

	if (ImGui::Button("Save Scene"))
	{
		SaveScene("SceneData.json");
	}

	ImGui::Separator();
	ImGui::Text("Spawn Actor");

	static string selectedActorName = "";
	if (selectedActorName.empty() && !_actorFactory.empty())
	{
		selectedActorName = _actorFactory.begin()->first;
	}

	if (ImGui::BeginCombo("Actor Type", selectedActorName.c_str()))
	{
		for (auto& iter : _actorFactory)
		{
			if (ImGui::Selectable(iter.first.c_str()))
			{
				selectedActorName = iter.first;
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::Button("Add Actor"))
	{
		if (!selectedActorName.empty() && _actorFactory.contains(selectedActorName))
		{
			Actor* newActor = _actorFactory[selectedActorName]();

			if (newActor != nullptr)
			{
				newActor->Init();

				AddActor(newActor);
			}
		}
	}

	ImGui::Checkbox("Pause Game", &_isPaused);

	ImGui::Separator();

	for (auto& actor : _actors)
	{
		actor->RenderUI();
	}
	ImGui::End();
}

void Scene::SaveScene(const string& filename)
{
	json j;
	json actorsArray = json::array();

	for (auto actor : _actors)
	{
		actorsArray.push_back(actor->ToJson());
	}
	j["Actors"] = actorsArray;

	std::ofstream file(filename);
	if (file.is_open())
	{
		// dump(4) : 4칸 들여쓰기해서 텍스트로 만듦
		file << j.dump(4);
		file.close();
	}
}

bool Scene::LoadScene(const string& filename)
{
	std::ifstream file(filename);
	if(!file.is_open()) return false;

	json j;
	file >> j;

	if (j.contains("Actors") && j["Actors"].is_array())
	{
		for (auto& actorJson : j["Actors"])
		{
			string name = actorJson["name"];
			_initialActorJsonData[name] = actorJson;
			Actor* newActor = nullptr;

			// 팩토리 맵에 해당 이름이 등록되어 있는지 확인
			if (_actorFactory.contains(name))
			{
				// 등록된 생성 함수를 호출하여 객체를 찍어냄
				newActor = _actorFactory[name]();
			}
			else
			{ 
				continue;
			}

			if (newActor)
			{
				newActor->Init();
				newActor->FromJson(actorJson);
				AddActor(newActor);
			}

		}
	}

	return true;
}

Actor* Scene::FindActorByType(ActorType type)
{
	// 1. 활성화된 액터 목록 검색
	for (auto* actor : _actors)
	{
		if (actor && actor->GetActorType() == type)
			return actor;
	}

	// 2. 생성 예약 목록(_reservedAdd) 검색
	for (auto* actor : _reservedAdd)
	{
		if (actor && actor->GetActorType() == type)
			return actor;
	}

	return nullptr;
}

Actor* Scene::SpawnActorFromInitialData(const string& name)
{
	if (!_initialActorJsonData.contains(name) || !_actorFactory.contains(name))
	{
		return nullptr;
	}

	Actor* newActor = _actorFactory[name]();
	if (newActor != nullptr)
	{
		newActor->Init();
		newActor->FromJson(_initialActorJsonData[name]);
	}

	return newActor;
}
