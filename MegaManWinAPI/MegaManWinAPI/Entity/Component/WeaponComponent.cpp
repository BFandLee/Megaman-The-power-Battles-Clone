#include "pch.h"
#include "WeaponComponent.h"
#include "Weapon.h"
#include "Buster.h"
#include "InputManager.h"
#include "ObjectPoolManager.h"
#include "SceneManager.h"

WeaponComponent::WeaponComponent() : Component("WeaponComponent")
{
}

WeaponComponent::~WeaponComponent()
{
    if (_currentWeapon != nullptr)
    {
        delete _currentWeapon;
        _currentWeapon = nullptr;
    }
}

void WeaponComponent::Init()
{
    // TODO: 기본 무기(Buster 등)를 동적 할당하여 _currentWeapon에 장착
    _currentWeapon = new Buster(this);
}

void WeaponComponent::Update(float deltaTime)
{
    if (_currentCooldown > 0)
    {
        _currentCooldown -= deltaTime;
    }

    if (_currentCooldown <= 0 && _currentBurstCount == 0)
    {
        _currentBurstCount = _maxBurstCount;
        _isAttacking = false;
    }

    // TODO: 공격 키 입력을 검사하고 _currentWeapon->Fire() 호출
    if (InputManager::GetInstance().GetButtonDown(KeyType::A) && _currentCooldown <= 0.0f)
    {
        _currentWeapon->Fire();
        _isAttacking = true;
        _currentAnimTimer = _attackAnimDuration;
        _currentBurstCount--;
        _currentCooldown = (_currentBurstCount != 0) ? _attackInterval : _reloadCooldown;
    }
}

void WeaponComponent::RenderUI()
{
    // TODO: ImGui로 무기 데미지 등의 데이터를 실시간 수정할 수 있게 구현
    if (ImGui::TreeNode("Weapon"))
    {
        // 발사 간격
        ImGui::DragFloat("attackInterval", &_attackInterval);

        // 장전 시간
        ImGui::DragFloat("Reloading", &_reloadCooldown);

        // 쿨타임
        ImGui::DragFloat("CoolDown", &_currentCooldown);

        ImGui::TreePop();
    }
}

json WeaponComponent::ToJson()
{
    json j = Super::ToJson();

    // 고유한 데이터 추가
    j["attackInterval"] = _attackInterval;
    j["Reloading"] = _reloadCooldown;
    j["CoolDown"] = _currentCooldown;

    return j;
}

void WeaponComponent::FromJson(const json& j)
{
    Super::FromJson(j);

    if (j.contains("attackInterval")) _attackInterval = j["attackInterval"];
    if (j.contains("Reloading")) _reloadCooldown = j["Reloading"];
    if (j.contains("CoolDown")) _currentCooldown = j["CoolDown"];
}

void WeaponComponent::ChangeWeapon(Weapon* weapon)
{
    // TODO: 무기 교체 전 기존 무기가 있다면 delete하고 새 무기로 포인터 갱신--> 나중에 플레이어 폼 추가되면 그때가서 구현
    // 플레이어 무기 기본 무기1개라서 일단 Pass
}
