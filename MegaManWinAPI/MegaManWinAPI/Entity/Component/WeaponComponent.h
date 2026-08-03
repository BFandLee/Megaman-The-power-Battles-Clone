#pragma once
#include "Component.h"

class Weapon;

class WeaponComponent : public Component
{
private:
    Weapon* _currentWeapon = nullptr;
    bool _isAttacking = false;

public:
    WeaponComponent();
    virtual ~WeaponComponent();

    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void RenderUI() override;
    virtual json ToJson() override;
    virtual void FromJson(const json& j) override;

    void ChangeWeapon(Weapon* weapon);
    bool IsAttacking() const { return _isAttacking; }

    // TODO: 공격 데미지, 공격 쿨타임(CoolTime) 등 필요한 스탯 변수를 선언하세요.
};
