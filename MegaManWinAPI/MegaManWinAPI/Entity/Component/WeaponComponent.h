#pragma once
#include "Component.h"

class Weapon;

class WeaponComponent : public Component
{
    using Super = Component;
private:
    Weapon* _currentWeapon = nullptr;
    bool _isAttacking = false;
    float _attackAnimDuration = 0.2f; 
    float _currentAnimTimer = 0.0f;   

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

private:
    int _damage = 1;

    int _maxBurstCount = 3;
    int _currentBurstCount = 3;

    float _attackInterval = 0.15f;  // 연사 중 발사 간격 (짧은 쿨타임)
    float _reloadCooldown = 1.0f;   // 3발 다 쏜 후 장전 시간 (긴 쿨타임)
    float _currentCooldown = 0.0f;  // 현재 타이머

};
