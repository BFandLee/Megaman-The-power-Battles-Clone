#include "pch.h"
#include "CollisionManager.h"
#include "Game.h"
#include "Scene.h"
#include "Actor.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "CircleCollider.h"
#include "BoxCollider.h"


namespace DispatchTable
{
	bool CircleToCircle(Collider* a, Collider* b, HitResult& result)
	{
		CircleCollider* circleA = static_cast<CircleCollider*>(a);
		CircleCollider* circleB = static_cast<CircleCollider*>(b);

		// Q7. 원과 원 사이의 충돌 판정 로직을 구현해 보세요. (두 점 사이의 거리 < 두 반지름의 합)
		Vector c_aPos = circleA->GetColliderPos();
		Vector c_bPos = circleB->GetColliderPos();
		float distSq = (c_aPos - c_bPos).LengthSquared();
		float rSum = circleA->GetRadius() + circleB->GetRadius();
		return distSq < (rSum * rSum);
	}

	bool CircleToBox(Collider* a, Collider* b, HitResult& result)
	{
		CircleCollider* circle = static_cast<CircleCollider*>(a);
		BoxCollider* box = static_cast<BoxCollider*>(b);

		// Q8. [핵심] 원과 Box(AABB) 사이의 충돌 로직을 구현해 보세요. (Clamp 활용)
		Vector cPos = circle->GetColliderPos();
		Vector bPos = box->GetColliderPos();

		float left = bPos.x   - box->GetWidth()  / 2.0f;
		float right = bPos.x  + box->GetWidth()  / 2.0f;
		float top = bPos.y    - box->GetHeight() / 2.0f;
		float bottom = bPos.y + box->GetHeight() / 2.0f;

		float closetX = std::clamp(cPos.x, left, right);
		float closetY = std::clamp(cPos.y, top, bottom);

		Vector closestPoint(closetX, closetY);
		float distSq = (cPos - closestPoint).LengthSquared();
		return distSq <= (circle->GetRadius() * circle->GetRadius());
	}

	bool BoxToCircle(Collider* a, Collider* b, HitResult& result)
	{
		// 순서만 바꿔서 호출합니다.
		return CircleToBox(b, a, result);
	}

	bool BoxToBox(Collider* a, Collider* b, HitResult& result)
	{
		BoxCollider* boxA = static_cast<BoxCollider*>(a);
		BoxCollider* boxB = static_cast<BoxCollider*>(b);
		
		// Q9. Box와 Box 사이의 충돌 판정 로직을 구현해 보세요. (AABB 충돌)
		Vector posA = boxA->GetColliderPos();
		Vector posB = boxB->GetColliderPos();

		bool overlapX = std::abs(posA.x - posB.x) <= (boxA->GetWidth() + boxB->GetWidth()) / 2.0f;
		bool overlapY = std::abs(posA.y - posB.y) <= (boxA->GetHeight() + boxB->GetHeight()) / 2.0f;

		return overlapX && overlapY;
	}
}

void CollisionManager::Init()
{
	DISPATCH_TABLE[(int32)ColliderType::Circle][(int32)ColliderType::Circle] = DispatchTable::CircleToCircle;
	DISPATCH_TABLE[(int32)ColliderType::Circle][(int32)ColliderType::Box] = DispatchTable::CircleToBox;

	DISPATCH_TABLE[(int32)ColliderType::Box][(int32)ColliderType::Circle] = DispatchTable::BoxToCircle;
	DISPATCH_TABLE[(int32)ColliderType::Box][(int32)ColliderType::Box] = DispatchTable::BoxToBox;
}

void CollisionManager::Clear()
{
	_prev.clear();
	_curr.clear();
	_collisionCheckList.clear();
}

void CollisionManager::Update()
{
	// 현재 상태에 대한 충돌체크만 수행해서 결과를 저장
	_curr.clear();

	// 주체가 되는 녀석들만 순회
	for (auto actor : _collisionCheckList)
	{
		if (!actor->GetActive())
			continue;

		// 어떤 대상과 충돌체크를 해야하냐면, 내가 있는 셀과 인접한 셀만 충돌체크 수행
		checkCollision(actor);
	}

	// 현재 프레임에 충돌체크가 필요한 상태 체크 완료
	// Exit 
	for (const auto& iter : _prev)
	{
		// 이전에는 있었는데, 현재는 없다.
		// Exit
		if (_curr.contains(iter) == false)
		{
			// pair<Actor*, Actor*>
			// 양방향으로 Exit 함수를 호출해준다.
			iter.first->OnExit(iter.second);
			iter.second->OnExit(iter.first);
		}
	}

	// curr -> prev
	_prev = _curr;	// 이제부터 curr 상태가 prev 상태로 변경.
	//swap(_prev, _curr);

#ifdef _DEBUG
	// 디버깅 정보 토글
	if (InputManager::GetInstance().GetButtonDown(KeyType::F1))
	{
		_drawDebug = !_drawDebug;
	}
#endif
}

void CollisionManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
#ifdef _DEBUG
	if (_drawDebug)
	{
		// 디버깅을 위한 충돌체 상태 보기
		for (auto actor : _collisionCheckList)
		{
			Collider* col = actor->GetCollider();
			if (col != nullptr)
			{
				actor->GetCollider()->Render(renderTarget);
			}
		}
	}
#endif
}

void CollisionManager::AddActor(Actor* actor)
{
	_collisionCheckList.push_back(actor);
}

void CollisionManager::RemoveActor(Actor* actor)
{
	// 충돌체크가 필요한 객체에서 제거
	std::erase_if(_collisionCheckList, [actor](const Actor* iter) 
		{
			return iter == actor;
		});


	// 제거해야할 대상을 걸러주는 람다식
	auto checkActor = [actor](const std::pair<Actor*, Actor*>& pair)
		{
			if (pair.first == actor || pair.second == actor)
				return true;
			return false;
		};

	// 전체순회해도 비용이 크지 않다.
	// 겹쳐져있는 대상만 set 에 추가될꺼에요.
	std::erase_if(_prev, checkActor);
	std::erase_if(_curr, checkActor);
}

void CollisionManager::addOverlapState(Actor* actor1, Actor* actor2, const HitResult& result)
{
	auto pair = (actor1 < actor2) ? make_pair(actor1, actor2) : make_pair(actor2, actor1);

	// 현재 프레임에 충돌상태 체크 됨
	bool insert = _curr.insert(pair).second;	// second : true, 중복된 키를 추가했으면, second : false
	bool prev = _prev.contains(pair); // 이전 프레임에 key 조합이 있었는지 확인

	if (insert == true && prev == false)
	{
		actor1->OnEnter(actor2, result);
		actor2->OnEnter(actor1, result);
	}
	// Stay 해보고 싶으면
	// 현재 insert == true, 이전 : true
	else if (insert == true && prev == true)
	{
		// Stay : 이전 프레임에도 겹쳐 있었고, 지금도 겹쳐 있다.
		// Enter 는 최초 진입 1회만 호출되므로, 겹침이 유지되는 동안
		// 침투를 해소할 기회는 Stay 에서만 줄 수 있다. (예: 벽 방향 입력을 유지한 채 파고드는 경우)
		actor1->OnStay(actor2, result);
		actor2->OnStay(actor1, result);
	}
}

void CollisionManager::setIgnoreMask(ActorType A, ActorType B)
{
	// 항상 양방향으로 관리
	IGNORE_MASK[(int32)A][(int32)B] = true;
	IGNORE_MASK[(int32)A][(int32)A] = true;

	IGNORE_MASK[(int32)B][(int32)B] = true;
	IGNORE_MASK[(int32)B][(int32)A] = true;
}

void CollisionManager::checkCollision(Actor* actor)
{
	Collider* colA = actor->GetCollider();
	if (colA == nullptr) return;
	// Q10. 이중 for문을 돌며 actor와 충돌 가능한 다른 actor들을 순회하고,
	for (Actor* other : _collisionCheckList)
	{
		if (actor == other) continue;
		if (IGNORE_MASK[(int32)actor->GetActorType()][(int32)other->GetActorType()])
			continue;

		Collider* colB = other->GetCollider();
		if (colB == nullptr) continue;

		CheckFunc checkFunc = DISPATCH_TABLE[(int32)colA->GetColliderType()][(int32)colB->GetColliderType()];
		if (checkFunc != nullptr)
		{
			HitResult hit;
			bool isHit = checkFunc(colA, colB, hit);

			if (isHit)
			{
				addOverlapState(actor, other, hit);
			}
		}
	}
}