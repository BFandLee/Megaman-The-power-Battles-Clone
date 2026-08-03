#pragma once

class WeaponComponent;

class Weapon
{
protected:
    WeaponComponent* m_pOwner = nullptr;

public:
    Weapon(WeaponComponent* owner) : m_pOwner(owner) {}
    virtual ~Weapon() {}

    virtual void Fire() = 0; // 자식 무기 클래스들이 반드시 구현해야 할 발사 함수
};
