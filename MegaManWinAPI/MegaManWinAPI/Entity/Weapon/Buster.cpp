#include "pch.h"
#include "Buster.h"
#include "WeaponComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerBullet.h"
#include "ObjectPoolManager.h"
#include "Player.h"

Buster::Buster(WeaponComponent* owner)
    : Weapon(owner)
{
}

Buster::~Buster()
{
}

void Buster::Fire()
{
    // 1. 오브젝트 풀에서 꺼내기
    PlayerBullet* bullet = ObjectPoolManager::GetInstance().GetPlayerBulletPool()->Acquire();

    if (bullet)
    {
        // 2. 발사에 필요한 정보 가져오기
        Player* player = static_cast<Player*>(m_pOwner->GetOwner());
        float dirX = player->GetLookDirX();

        // 오프셋
        float offsetX = 45.0f;
        float offsetY = 0.0f;


        Vector pos = player->GetPos();
        pos.x += offsetX * dirX;
        pos.y += offsetY;

        Vector bulletDir = Vector(dirX, 0.0f);

        bullet->Reset(pos, bulletDir);

        SceneManager::GetInstance().GetScene()->AddActor(bullet);
    }
}
