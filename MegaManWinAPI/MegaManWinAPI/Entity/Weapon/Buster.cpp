#include "pch.h"
#include "Buster.h"
#include "WeaponComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerBullet.h"
#include "ObjectPoolManager.h"
#include "Player.h"
#include "MuzzleFlash.h"

Buster::Buster(WeaponComponent* owner)
    : Weapon(owner)
{
}

Buster::~Buster()
{
}

void Buster::Fire(ChargeLevel level)
{
    // TODO(USER): chargeLevel에 따라 다르게 PlayerBullet을 셋팅(Reset)하도록 수정해 보세요.
    // (예: bullet->Reset(pos, bulletDir, level); )
    // 1. 오브젝트 풀에서 꺼내기
    PlayerBullet* bullet = ObjectPoolManager::GetInstance().GetPlayerBulletPool()->Acquire();
    MuzzleFlash* flash = new MuzzleFlash();

    // 2. 발사에 필요한 정보 가져오기
    Player* player = static_cast<Player*>(m_pOwner->GetOwner());
    float dirX = player->GetLookDirX();
    if (bullet)
    {
        // 오프셋
        float offsetX = 45.0f;
        float offsetY = 0.0f;

        Vector pos = player->GetPos();
        pos.x += offsetX * dirX;
        pos.y += offsetY;

        Vector bulletDir = Vector(dirX, 0.0f);

        bullet->Reset(pos, bulletDir, level);

        SceneManager::GetInstance().GetScene()->AddActor(bullet);
    }

    if (flash)
    {
        flash->Init();
        // 오프셋
        float offsetX = 45.0f;
        float offsetY = 0.0f;

        Vector pos = player->GetPos();
        pos.x += offsetX * dirX;
        pos.y += offsetY;
        flash->SetPos(pos);

        Vector scale = flash->GetScale();
        scale.x = abs(scale.x) * dirX;
        flash->SetScale(scale);

        SceneManager::GetInstance().GetScene()->AddActor(flash);
    }
}
