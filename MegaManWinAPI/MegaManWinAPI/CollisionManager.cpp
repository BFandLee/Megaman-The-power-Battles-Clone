#include "pch.h"
#include "CollisionManager.h"
#include "Game.h"
#include "Scene.h"
#include "Actor.h"
#include "InputManager.h"
#include "SceneManager.h"
// #include "ColliderAABB.h"


namespace DispatchTable
{
	//bool CircleToCircle(Collider* a, Collider* b, HitResult& result)
	//{
	//	return Collider::CheckCircleToCircle(static_cast<ColliderCircle*>(a), static_cast<ColliderCircle*>(b), result);
	//}

	//bool CircleToAABB(Collider* a, Collider* b, HitResult& result)
	//{
	//	return Collider::CheckCircleToAABB(static_cast<ColliderCircle*>(a), static_cast<ColliderAABB*>(b), result);
	//}

	//bool AABBToCircle(Collider* a, Collider* b, HitResult& result)
	//{
	//	return Collider::CheckCircleToAABB(static_cast<ColliderCircle*>(b), static_cast<ColliderAABB*>(a), result);
	//}

	//bool AABBToAABB(Collider*, Collider*, HitResult& result)
	//{
	//	return false; // AABB vs AABB 는 아직 미지원 (필요해지면 여기만 채우면 된다)
	//}
}

void CollisionManager::Init()
{
	// collider Type에 맞춰서 호출해야하는 함수를 결정
	/*DISPATCH_TABLE[(int32)ColliderType::Circle][(int32)ColliderType::Circle] = DispatchTable::CircleToCircle;
	DISPATCH_TABLE[(int32)ColliderType::Circle][(int32)ColliderType::AABB] = DispatchTable::CircleToAABB;

	DISPATCH_TABLE[(int32)ColliderType::AABB][(int32)ColliderType::Circle] = DispatchTable::AABBToCircle;
	DISPATCH_TABLE[(int32)ColliderType::AABB][(int32)ColliderType::AABB] = DispatchTable::AABBToAABB;*/
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

	// 충돌체크가 필요한 Actor는 전부다 비교해서 충돌체크를 수행한다.
	// 일단, 모든 녀석들을 다 순회하면서 체크한다.
	// [0] : player, [1] enemy, [2] p.bullet [3] e.bullet ..
	// [0]<->[1], [2], [3] 전부 비교

	// _collisionCheckList : 리스트에 존재한다는건, 충돌체크를 실행해야할 '주체'
	// 현재 : 내 비행기, 적 비행기, 내 총알, 적 총알
	// Grid 방식 : 내 비행기, 내 총알, 

	// 주체가 되는 녀석들만 순회
	for (auto actor : _collisionCheckList)
	{
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

	// 디버깅 정보 토글
	if (InputManager::GetInstance().GetButtonDown(KeyType::F1))
	{
		//if (_drawDebug)
		//{
		//	_drawDebug = false;
		//}
		//else
		//{
		//	_drawDebug = true;
		//}

		_drawDebug = !_drawDebug;
	}
}

void CollisionManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//if (_drawDebug)
	//{
	//	// 그리드 라인 보기
	//	drawGridLine(hdc);

	//	// 디버깅을 위한 충돌체 상태 보기
	//	for (auto actor : _collisionCheckList)
	//	{
	//		actor->GetCollider()->Render(hdc, actor->GetPos());
	//	}
	//}
}

void CollisionManager::AddActor(Actor* actor)
{
	
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

//void CollisionManager::addOverlapState(Actor* actor1, Actor* actor2, const HitResult& result)
//{
//	auto pair = (actor1 < actor2) ? make_pair(actor1, actor2) : make_pair(actor2, actor1);
//
//	// 현재 프레임에 충돌상태 체크 됨
//	bool insert = _curr.insert(pair).second;	// second : true, 중복된 키를 추가했으면, second : false
//	bool prev = _prev.contains(pair); // 이전 프레임에 key 조합이 있었는지 확인
//
//	if (insert == true && prev == false)
//	{
//		actor1->OnEnter(actor2, result);
//		actor2->OnEnter(actor1, result);
//	}
//	// Stay 해보고 싶으면
//	// 현재 insert == true, 이전 : true
//	else if (insert == true && prev == true)
//	{
//		// Stay : 이전 프레임에도 겹쳐 있었고, 지금도 겹쳐 있다.
//		// Enter 는 최초 진입 1회만 호출되므로, 겹침이 유지되는 동안
//		// 침투를 해소할 기회는 Stay 에서만 줄 수 있다. (예: 벽 방향 입력을 유지한 채 파고드는 경우)
//		actor1->OnStay(actor2, result);
//		actor2->OnStay(actor1, result);
//	}
//}

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
	
}

//void CollisionManager::drawGridLine(ID2D1HwndRenderTarget* renderTarget)
//{
//	int32 gridSize = SceneManager::GetInstance().GetScene()->GetGridSize();
//
//	// 빨간색 그리드 배경 선
//	{
//		// 화면 크기와 그리드 크기 설정
//		int32 width = GWinSizeX;
//		int32 height = GWinSizeY;
//
//		// 빨간색 펜 생성
//		HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
//		HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
//
//		// 가로선 그리기
//		for (int y = 0; y <= height; y += gridSize)
//		{
//			MoveToEx(hdc, 0, y, nullptr); // 시작점 설정
//			LineTo(hdc, width, y);        // 끝점까지 선 그리기
//		}
//
//		// 세로선 그리기
//		for (int x = 0; x <= width; x += gridSize)
//		{
//			MoveToEx(hdc, x, 0, nullptr); // 시작점 설정
//			LineTo(hdc, x, height);       // 끝점까지 선 그리기
//		}
//
//		// 이전 펜 복원 및 새 펜 삭제
//		SelectObject(hdc, oldPen);
//		DeleteObject(redPen);
//	}
//
//	// 충돌 체크가 필요한 선만 그리기 : 진한 청록색
//	{
//		// 펜 생성
//		HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
//		HPEN oldPen = (HPEN)SelectObject(hdc, myPen);
//
//		for (auto actor : _collisionCheckList)
//		{
//			const Cell& cell = Cell::ConvertToCell(actor->GetPos(), gridSize);
//
//			// 인접한 셀 모두 표시
//			for (int32 i = -1; i < 2; ++i)
//			{
//				for (int32 j = -1; j < 2; ++j)
//				{
//					Cell checkCell{ cell.iX + i, cell.iY + j };
//
//					// 사각형 그리기
//					int32 x = checkCell.iX * gridSize;
//					int32 y = checkCell.iY * gridSize;
//
//					{
//						MoveToEx(hdc, x, y, nullptr);
//						LineTo(hdc, x + gridSize, y);
//					}
//					{
//						MoveToEx(hdc, x + gridSize, y, nullptr);
//						LineTo(hdc, x + gridSize, y + gridSize);
//					}
//					{
//						MoveToEx(hdc, x + gridSize, y + gridSize, nullptr);
//						LineTo(hdc, x, y + gridSize);
//					}
//					{
//						MoveToEx(hdc, x, y + gridSize, nullptr);
//						LineTo(hdc, x, y);
//					}
//				}
//			}
//		}
//
//		// 이전 펜 복원 및 새 펜 삭제
//		SelectObject(hdc, oldPen);
//		DeleteObject(myPen);
//	}
//}
