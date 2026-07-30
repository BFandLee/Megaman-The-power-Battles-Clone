#include "pch.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "CollisionManager.h"
//#include "Effect.h"
//#include "DataManager.h"
//#include "ResourceData.h"
// #include "Player.h"	
#include "Background.h"
#include "Actor.h"

// 생성자/소멸자를 cpp 작성하면, Scene의 인스턴스화는 cpp에서 일어남.
// ObjectPool<T> (vector<T>) 값 자체를 가지고 있는 풀을 생성하는것도,
// cpp에서 인스턴스화할때 생성됨.
// 이때는 Bullet/Enemy #include 완료 상태
Scene::Scene() 
{
}
Scene::~Scene()
{
}

void Scene::Init()
{
	// Grid 미리 생성
	/*_gridCountX = (int32)GWinSizeX / _gridSize;
	_gridCountY = (int32)GWinSizeY / _gridSize;

	int32 totalGridCount = _gridCountX * _gridCountY;
	_grid.resize(totalGridCount);*/

	// Scene에 필요한 리소스 로드
	loadResources();

	// Scene에 필요한 객체 생성
	createObjects();


}

void Scene::Cleanup()
{
	// 씬에 등장하는 모든 객체들의 delete 담당
	//for (auto iter : _actors)
	//{
	//	// Scene이 new 한 객체는 delete 해도 된다.
	//	if (iter->GetPool() == nullptr)
	//	{
	//		delete iter;
	//	}
	//}
	//_actors.clear();
}

void Scene::Update(float deltaTime)
{
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);

			//Ball::Update()
			//	GameScene::Dead()
			//		StageLoader::Load()
			//			Scene::RemoveAllActor()
			//				_actors.clear();	// 자료 구조 클리어
	}

	// 삭제가 필요한 애들은 삭제
	// 1번 방식으로 '삭제 여부' 걸러도 되고,
	// 2번 방식으로 '삭제 여부' 걸러도 되고
	std::erase_if(_actors, [this](Actor* actor)
		{
			// 1번 방식은, _actor에서 제거는 되는데,
			// 제거되기전에 아래 delete + removeActor 함수 호출해야해서
			// 2번 방식으로 별도 리스트를 관리하는게 좋겟다.
			//return actor->GetPendingKill(); 
			return _reservedRemove.contains(actor);
		});

	// 실제 메모리 해제 까먹었따.
	//for (auto deleteActor : _reservedRemove)
	//{
	//	// 삭제되는 Actor
	//	removeActor(deleteActor);

	//	// 해당 Actor가 풀에서 태어난 경우에는 반환
	//	if (deleteActor->GetPool())
	//	{
	//		// 해당 풀에다가 반환
	//		deleteActor->GetPool()->Return(deleteActor);
	//	}
	//	else
	//	{
	//		// new 태어난 경우는 delete
	//		delete deleteActor;
	//	}
	//}

	// 추가가 필요한 애들은 추가
	// 1번 방식 : 매번 push_back 할때마다 비용 지불
	//for (auto iter : _reservedAdd)
	//{
	//	// vector
	//	// capacity, size
	//	// 새로 원소를 집어넣을떄 capacity 부족시, 메모리 추가 할당
	//	_actors.push_back(iter);
	//}

	// 2번 방식 :
	// 여긴, reserverdAdd 에 10개가 있을경우
	// vector를 한번에 10개 늘리고 복사해와서, 재할당이 1번 일어난다.
	//_actors.insert(_actors.end(), _reservedAdd.begin(), _reservedAdd.end());

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

	// 그리드 갱신 : 초기화 -> 재갱신 이방식이 마음에 안든다면,
	// Actor의 위치가 변경될때마다 Grid 의 위치를 갱신해주는 방식을 하면 된다.
	// 즉, 아래 코드는 다 사라지고 Actor가 Scene에게 요청을 해서, Grid 갱신한다.
	{
		// 모든 Actor의 최신화된 좌표 기준으로 Grid 갱신
		// 이전프레임에 있었던 Grid 정보는 초기화
		//for (GridInfo& grid : _grid)
		//{
		//	grid.actors.clear();
		//}

		// 1945 게임 특성상 매프레임 위치 변경이 있으니깐, 
		// 그냥 전체 순회하면서 Grid 등록을 해준다.
		// 전체 순회니깐, 어차피 또 성능적인 측면의 이점이 없는거 아닌가요.
		// O(N*M) -> O(N)
		//for (auto actor : _actors)
		//{
		//	updateGrid(actor);
		//}
	}


}

void Scene::Render(ID2D1RenderTarget* renderTarget)
{
	// 명확한 렌더링 순서를 지키기 위해 별도의 리스트 순서대로 그린다.
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			actor->Render(renderTarget);
		}
	}

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
	// 여기에서 즉시 삭제하지 않는다.
	// vector 빼고, delete 해주고.
	//	_actors.erase

	// 지연 삭제
	// 추가적인 리스트에 넣고, 나중에 한번에 삭제
	_reservedRemove.insert(actor);
}


Actor* Scene::FindActorByType(ActorType type) const
{
	for (Actor* actor : _actors)
	{
		/*if (actor->GetActorType() == type)
		{
			return actor;
		}*/
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

//void Scene::CreateEffect(Vector pos)
//{
//	Effect* effect = new Effect();
//	effect->Init(L"Effect");
//	effect->SetPos(pos);
//
//	_reservedAdd.push_back(effect);
//}


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


	/*
	// 실제 텍스처 로드 요청
	// Direct2D 방식의 이상적인 형태 (컬러 키 파라미터가 필요 없음)
	ResourceManager::GetInstance().LoadTexture(L"Player", L"Player.png");
	*/
// }

void Scene::createObjects()
{

}

// Scene에 등록되는 Actor들이 모두 해야할일
void Scene::registerActor(Actor* actor)
{
	if (actor == nullptr) return;
	_renderList[(int32)actor->GetRenderLayer()].push_back(actor);

	/*if (actor->GetCollider())
	{
		CollisionManager::GetInstance().AddActor(actor);
	}*/
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