#pragma once
#include "Singleton.h"
#include "Collider.h"

class CollisionManager : public Singleton<CollisionManager>
{
	// Singleton 객체를 '친구'로 선언해서 private 접근 가능하게 열어준다.
	friend Singleton<CollisionManager>;
public:

	void Init();
	void Clear();
	void Update();
	void Render(ID2D1HwndRenderTarget* renderTarget);

	// 충돌체크가 필요한 녀석들
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void addOverlapState(Actor* actor1, Actor* actor2, const HitResult& result);

private:
	void setIgnoreMask(ActorType A, ActorType B);

	// actor와 인접한 셀을 훑으면서 충돌체크 수행
	void checkCollision(Actor* actor);
	
	// 디버깅용 라인 그리기
	// void drawGridLine(ID2D1HwndRenderTarget* renderTarget);

	// 아무나 생성못하게 생성자/소멸자를 숨기자
	CollisionManager() = default;
	~CollisionManager() = default;

public:
	bool GetDrawdebug() { return _drawDebug; }
private:
	bool _drawDebug = false;

	set<std::pair<Actor*, Actor*>> _prev; // 이전에 (충돌된 쌍) 관리
	set<std::pair<Actor*, Actor*>> _curr; // 이전에 (충돌된 쌍) 관리

	// 충돌체크 해야하는 모든 Actor
	// 디폴트로 무조건 충돌체크 수행,
	// ( ActorType vs ActorType ) 옵션에 따라서 충돌체크 무시
	vector<Actor*> _collisionCheckList; // (E,P)bullet, player, enemy
	
	bool IGNORE_MASK[(int32)ActorType::Count][(int32)ActorType::Count] = {}; // 일종의 테이블
	
	// Collider Type 에 따라 호출해야하는 함수 포인터
	using CheckFunc = bool(*)(Collider* a, Collider* b, HitResult& result);
	CheckFunc DISPATCH_TABLE[(int32)ColliderType::Max][(int32)ColliderType::Max];
};
