#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "Game.h"
#include "Scene.h"
#include "Enemy.h"
#include "Bullet.h"
#include "ColliderCircle.h"

void Player::Init()
{
	loadTexture(L"Player");

	// 충돌체가 만들어져있는데, 충돌매니저에서 충돌체크를 실행해야하는 '주체'
	if (_collider)
	{
		_collider->SetCheckCell(true);
	}
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (InputManager::GetInstance().GetButtonPressed(KeyType::W))
	{
		move(0, -_moveSpeed * deltaTime);
	}

	if (InputManager::GetInstance().GetButtonPressed(KeyType::S))
	{
		move(0, _moveSpeed * deltaTime);
	}

	if (InputManager::GetInstance().GetButtonPressed(KeyType::A))
	{
		move(-_moveSpeed * deltaTime, 0);
	}

	if (InputManager::GetInstance().GetButtonPressed(KeyType::D))
	{
		move(_moveSpeed * deltaTime, 0);
	}

	if (InputManager::GetInstance().GetButtonDown(KeyType::SpaceBar))
	{
		Game::GetInstance().GetScene()->CreateBullet(GetPos(), BulletType::Player);
	}

	// 적비행기 가지고와서 충돌체크 수행?
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::OnEnter(Actor* other)
{
	// 적 총알 or 적 비행기라면 피해입기
	if (other->GetActorType() == ActorType::Enemy ||
		other->GetActorType() == ActorType::EnemyBullet)
	{
		takeDamage();
	}
}

void Player::move(float x, float y)
{
	Vector newPos = GetPos();
	newPos.x += x;
	newPos.y += y;

	// 양옆
	if (newPos.x <= GetWidth())
	{
		newPos.x = (float)GetWidth();
	}
	else if (newPos.x >= GWinSizeX - GetWidth())
	{
		newPos.x = (float)GWinSizeX - GetWidth();
	}

	// 위아래
	if (newPos.y <= GetHeight())	// 이런 로직들은 world 좌표계로 생각해서 그대로 두고.
	{
		newPos.y = (float)GetHeight();
	}
	else if (newPos.y >= GWinSizeY - GetHeight())
	{
		newPos.y = (float)GWinSizeY - GetHeight();
	}

	SetPos(newPos);
}

void Player::takeDamage()
{
	_hp -= 10;

	// 터지는 이펙트 추가 
	Game::GetInstance().GetScene()->CreateEffect(GetPos());
	
	// 체력이 0이면, 스스로 삭제
	if (_hp <= 0)
	{
		Destroy();
	}
}

