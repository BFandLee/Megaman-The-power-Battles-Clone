#pragma once

#include "ObjectPool.h"

//#include "Enemy.h"
//#include "Bullet.h"
// C++17
class Enemy;
class Bullet;

// 게임화면에 등장하는 모든 오브젝트를 관리
class Scene
{
public:
	// vector<T> 풀에서 사용하는 Enemy,Bullet 값자체를 전방선언으로 해결하기 위해
	// Scene의 생성자와 소멸자는 cpp 쪽에 구현을 해야한다.
	Scene();
	~Scene();

	void Init();
	void Cleanup();

	virtual void Update(float deltaTime);
	virtual void Render(ID2D1RenderTarget* renderTarget);
	
	// 모든 업데이트가 끝나고 호출되는 함수
	void PostUpdate();
	void AddPostUpdateAction(std::function<void()> action);

	SceneType GetSceneType() { return _sceneType; }

	// 씬에 관리되는 Actor 추가
	void AddActor(class Actor* actor);

	// 씬에서 관리되는 Actor중에 하나 삭제해달라고 요청
	void DeleteActor(class Actor* actor);
	// void DeleteActorByCell(Cell cell);	grid
	
	Actor* FindActorByType(ActorType type) const;

	// 전부다 지우기
	void RemoveAllActor();

	void CreateEffect(Vector pos);
	Actor* CreateActor(ActorType type);

	void RenderUI();
	void SaveScene(const string& filename);
	bool LoadScene(const string& filename);

public:
	const vector<Actor*>& GetRenderList(RenderLayer layer) const;
	// const GridInfo& GetGridInfo(const Cell& cell);
	// int32 GetGridSize() const { return _gridSize; }

	// void UpdateGrid(Actor* actor, Vector prevPos);

protected:
	virtual void loadResources();
	virtual void createObjects();

	// actor List / render List 의 동기화를 맞춰주기 위해서, 항상 호출되는 함수
	void registerActor(Actor* actor);
	void removeActor(Actor* actor);

	// bool isValidCell(Cell cell) const;

protected:
	// 모든 클래스를 관리하는 공통 자료구조를 선언
	vector<Actor*> _actors;	 // 여기가 진짜 Update,Render하는 객체들

	// 렌더링 순서를 위한 list
	vector<Actor*> _renderList[(int32)RenderLayer::Count];
	
	// 지연 시스템
	vector<Actor*>		_reservedAdd;	

	// 제거 요청을 중복처리하지 않기 위해, set 자료구조
	unordered_set<Actor*>	 _reservedRemove;		// vector vs map

	// 한프레임 뒤로 미뤄서 특정 함수를 호출해주는 기능
	std::vector<std::function<void()>> _postUpdateActions;

	// 본인의 타입
	SceneType _sceneType = SceneType::Max;

private:
	std::map<string, std::function<class Actor* ()>> _actorFactory;

	// 팩토리에 쉽게 등록하기 위한 탬플릿 함수
	template<typename T>
	void RegisterActor(const string& name)
	{
		// 람다함수를 이용하여 new T()를 호출하는 함수 자체를 저장
		_actorFactory[name] = []() {return new T(); };
	}
};


