#include "pch.h"
#include "ObjectPoolManager.h"

void ObjectPoolManager::Init()
{
	_playerBulletPool = new ObjectPool<PlayerBullet>();
	_playerBulletPool->Init(10);
}

void ObjectPoolManager::Clear()
{

}