#include "pch.h"
#include "Buster.h"
#include "WeaponComponent.h"

Buster::Buster(WeaponComponent* owner)
    : Weapon(owner)
{
}

Buster::~Buster()
{
}

void Buster::Fire()
{
    // TODO: 록맨의 기본 콩알탄(Buster) 오브젝트를 생성하고 씬(Scene)에 등록하는 발사 로직 구현
}
