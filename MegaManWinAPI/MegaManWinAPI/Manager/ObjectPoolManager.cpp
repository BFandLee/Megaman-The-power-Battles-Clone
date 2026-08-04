#include "pch.h"
#include "ObjectPoolManager.h"

void ObjectPoolManager::Init()
{
	_playerBulletPool = new ObjectPool<PlayerBullet>();
	_playerBulletPool->Init(3);
}

void ObjectPoolManager::Clear()
{

}