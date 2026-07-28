#include "pch.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "Bullet.h"
#include "CollisionManager.h"
#include "Effect.h"
#include "DataManager.h"
#include "ResourceData.h"
#include "WorldBG.h"

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
	// Scene -> LobbyScene, GameScene, EditScene
	// Scene에 필요한 리소스 로드
	loadResources();

	// 객체생성전에 미리 풀을 생성해둔다.
	// 미리 100개를 만들어둔다. Bullet [100]
	_bulletPool.Init(100);
	_enemyPool.Init(100);

	// Scene에 필요한 객체 생성
	createObjects();

	// Grid 미리 생성
	_gridCountX = (int32)_mapSize.x / _gridSize;
	_gridCountY = (int32)_mapSize.y / _gridSize;

	int32 totalGridCount =_gridCountX * _gridCountY;
	_grid.resize(totalGridCount);


	// ObjectPool 미리 생성
	//for (int32 i = 0; i < 100; ++i)
	//{
	//	// 이 방식은, 운영체제가 알아서 메모리를 할당해주기때문에
	//	// 연속메모리를 줄수도 있고, 아닐수도 있고.
	//	// 캐시 히트율이 좋을수도 있고, 안좋을수도 있다.
	//	_bulletList.push_back(new Bullet());	
	//}

	// vector 자체 순회는 캐시 히트가 좋지만,
	//for (auto bullet : _bulletList)
	//{
	//	bullet->GetPos();	// 직접 Bullet 을 찾아가서 정보를 읽어야 한다면, 캐시 미스가 발생할 확률이 있다.
	//}
}

void Scene::Cleanup()
{
	// 씬에 등장하는 모든 객체들의 delete 담당
	for (auto iter : _actors)
	{
		// Scene이 new 한 객체는 delete 해도 된다.
		if (iter->GetPool() == nullptr)
		{
			delete iter;
		}
	}
	_actors.clear();
}

void Scene::Update(float deltaTime)
{
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
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
		for (GridInfo& grid : _grid)
		{
			grid.actors.clear();
		}

		// 게임 특성상 매프레임 위치 변경이 있으니깐, 
		// 그냥 전체 순회하면서 Grid 등록을 해준다.
		// 전체 순회니깐, 어차피 또 성능적인 측면의 이점이 없는거 아닌가요.
		// O(N*M) -> O(N)
		for (auto actor : _actors)
		{
			updateGrid(actor);
		}
	}


	// 플레이어의 위치와 카메라의 위치를 맞추고 싶다.
	if (_player)
	{
		_cameraPos = _player->GetPos();

		// camera가 좌상단, 우하단 범위를 벗어나면 안되니깐, 좌표를 보정해준다.
		float halfSizeX = GWinSizeX / 2;
		float halfSizeY = GWinSizeY / 2;

		// clamp 함수는 아래 4줄을 한줄로 해주는 함수
		/*
		if (_cameraPos.x < halfSizeX) // 최소
			_cameraPos.x = halfSizeX;
		else if(_cameraPos.x > _mapSize.x - halfSizeX) // 최대
			_cameraPos.x = _mapSize.x - halfSizeX;
		*/

		_cameraPos.x = ::clamp(_cameraPos.x, halfSizeX, _mapSize.x - halfSizeX);
		_cameraPos.y = ::clamp(_cameraPos.y, halfSizeY, _mapSize.y - halfSizeY);
	}
}

void Scene::Render(HDC hdc)
{
	// 명확한 렌더링 순서를 지키기 위해 별도의 리스트 순서대로 그린다.
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			actor->Render(hdc);
		}
	}

	//for (auto actor : _actors)
	//{
	//	actor->Render(hdc);
	//}
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

void Scene::CreateBullet(Vector pos, BulletType type)
{
	Bullet* bullet = _bulletPool.Acquire(); // new Bullet();
	bullet->Init(type);
	bullet->SetPos(pos);

	_reservedAdd.push_back(bullet);
}

void Scene::CreateEffect(Vector pos)
{
	Effect* effect = new Effect();
	effect->Init(L"Effect");
	effect->SetPos(pos);

	_reservedAdd.push_back(effect);
}

Vector Scene::ConvertWorldToScreen(Vector worldPos)
{
	Vector offset;
	offset.x = _cameraPos.x - (GWinSizeX / 2);
	offset.y = _cameraPos.y - (GWinSizeY / 2);

	Vector screenPos = worldPos - offset;
	return screenPos;
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
	// DataManager에 가서 GameScene에 필요한 모든 데이터를 다 로드해달라고 요청
	ResourceData* data = DataManager::GetInstance().FindData<ResourceData>(L"ResourceData");
	if (data)
	{
		// 새로운 텍스처 로드가 필요하면, json에 추가하면 끝!
		for (auto iter : data->_gameSceneData)
		{
			const ResourceData::Item& item = iter.second;
			ResourceManager::GetInstance().LoadTexture(
				iter.first, 
				item.fileName, 
				item.transparent,
				item.countY,
				item.countX,
				item.dur);
		}
	}


	/*
	// 실제 텍스처 로드 요청
	ResourceManager::GetInstance().LoadTexture(L"Player", L"Player.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"BG", L"BG.bmp", -1);
	ResourceManager::GetInstance().LoadTexture(L"Enemy1", L"Enemy1.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"Enemy2", L"Enemy2.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"Enemy3", L"Enemy3.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"Enemy4", L"Enemy4.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"EnemyBullet", L"EnemyBullet.bmp", RGB(0, 0, 0), 1, 5);
	ResourceManager::GetInstance().LoadTexture(L"Effect", L"explosion.bmp", RGB(0, 0, 0), 2, 6, 2.0f);
	ResourceManager::GetInstance().LoadTexture(L"Item", L"GoldTresureClosed.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"PlayerBullet", L"PlayerBullet.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"UI_HP", L"PlayerHP.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance().LoadTexture(L"Stage_2", L"Stage_2.bmp", -1);
	*/
}

void Scene::createObjects()
{
	// 윈도우 크기와 1:1맞는 고정 배경
	if(false)
	{
		Background* bg = new Background();
		bg->Init();
		_reservedAdd.push_back(bg);
	}

	// 윈도우 크기보다 훨씬큰 배경
	if(true)
	{
		WorldBG* bg = new WorldBG();
		bg->Init();

		_mapSize = bg->GetMapSize();
		_reservedAdd.push_back(bg);
	}

	// 플레이어
	Player* player = new Player();
	player->Init();
	player->SetPos(Vector(GWinSizeX * 0.5f, 400));
	_reservedAdd.push_back(player);

	// 플레이어 객체를 캐싱해두자.
	_player = player;
	
	// 랜덤하게 등장하는 적들
	createRandomEnemy();

	// 적들을 2초마다 주기적으로 스폰하는 타이머 설정
	TimeManager::GetInstance().AddTimer([this]()
		{
			createRandomEnemy();
		},
		2.0f, // 2초마다
		true); // 반복(true) 알람을 울려달라
}

// Scene에 등록되는 Actor들이 모두 해야할일
void Scene::registerActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() >= RenderLayer::Count)
		return;

	// 렌더링 순서에 맞게 리스트 갱신
	_renderList[(int32)actor->GetRenderLayer()].push_back(actor);

	// 충돌체크가 필요하다면, 충돌체크 등록
	if (actor->GetCollider())
	{
		CollisionManager::GetInstance().AddActor(actor);
	}
}

// Scene에 제거되는 Actor들이 모두 해야할일
void Scene::removeActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	// 삭제해야하는 객체가 Player였다면, 댕글링포인터를 예방하기 위해
	// 캐싱해두고 있던 포인터도 갱신해주자.
	if (actor == _player)
	{
		_player = nullptr;
	}

	if (actor->GetRenderLayer() >= RenderLayer::Count)
		return;

	// RenderList vector에서 찾아서 제거
	std::erase_if(_renderList[(int32)actor->GetRenderLayer()],
		[actor](Actor* iter)
		{
			return iter == actor;
		});

	// 삭제될 Actor포인터를 CollisionManager 에서 Enter/Exit 비교를 위해
	// 포인터를 저장하고 있기 때문에, 충돌체크에서도 빼자.
	if (actor->GetCollider())
	{
		CollisionManager::GetInstance().RemoveActor(actor);
	}
}

void Scene::createRandomEnemy()
{
	wstring textureKey[4] = { L"Enemy1", L"Enemy2", L"Enemy3", L"Enemy4" };
	int randomIndex = rand() % 4;

	const int32 enemyCount = 4;
	Vector pos{ 50, 100 };
	int32 xDelta = GWinSizeX / enemyCount;

	for (int32 i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = _enemyPool.Acquire(); //new Enemy();
		if (nullptr == enemy)
			return;

		enemy->Init(Vector{ pos.x + (xDelta * i), pos.y }, textureKey[randomIndex]);

		// 추가는 무조건 예약 리스트에 넣는다.
		_reservedAdd.push_back(enemy);
	}
}

void Scene::updateGrid(Actor* actor)
{
	Cell cell = Cell::ConvertToCell(actor->GetPos(), _gridSize);

	// Cell의 범위를 체크해서, 굳이 관리가 필요없는 좌표의 경우는 무시
	if (cell.iX < 0 || cell.iY < 0 || cell.iX >= _gridCountX || cell.iY >= _gridCountY)
		return;

	// 1차원 배열의 인덱스로 변환을 해줘야한다.
	int32 index = cell.iY * _gridCountX + cell.iX; // Sprite Anim 방식과 비슷

	// 배열을 [] 인덱스 기법으로 접근할때는 범위체크 해주자.
	if (index >= 0 && index < _grid.size())
	{
		_grid[index].actors.push_back(actor);
	}
}

const GridInfo& Scene::GetGridInfo(const Cell& cell)
{
	// per-axis bounds check to prevent index wraparound
	static GridInfo emptyGridInfo{};
	if (cell.iX < 0 || cell.iX >= _gridCountX || cell.iY < 0 || cell.iY >= _gridCountY)
		return emptyGridInfo;

	int32 index = cell.iY * _gridCountX + cell.iX;
	if (index >= 0 && index < _grid.size())
	{
		GridInfo& gridInfo = _grid[index];
		return gridInfo;
	}

	// null object 패턴.
	return emptyGridInfo;
}