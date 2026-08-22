// ObjectPoolManager.h 뼈대
#pragma once
#include "Singleton.h"
#include "PlayerBullet.h"
#include "ObjectPool.h"

class ObjectPoolManager : public Singleton<ObjectPoolManager>
{
    friend Singleton<ObjectPoolManager>;

private:
    ObjectPool<PlayerBullet>* _playerBulletPool = nullptr;

public:
    void Init();    // 여기서 _playerBulletPool = new ObjectPool<PlayerBullet>(3); 등으로 초기화
    void Clear();   // 씬이 넘어가거나 게임 종료 시 메모리 정리

    ObjectPool<PlayerBullet>* GetPlayerBulletPool() { return _playerBulletPool; }
};