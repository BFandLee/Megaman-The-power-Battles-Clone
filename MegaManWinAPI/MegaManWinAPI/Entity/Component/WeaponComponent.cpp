#include "pch.h"
#include "WeaponComponent.h"
#include "Weapon.h"
#include "Buster.h"
#include "InputManager.h"
#include "ObjectPoolManager.h"
#include "SceneManager.h"
#include "AnimatorComponent.h"
#include "ChargeEffectActor.h"

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
    _currentWeapon = new Buster(this);

    AnimatorComponent* animator = GetOwner()->GetComponent<AnimatorComponent>();
    animator->LoadAnimationFromJson(L"IdleAttack", L"../Resources/sprites/Player/Animation/IdleAttack.json");
    animator->LoadAnimationFromJson(L"MoveAttack", L"../Resources/sprites/Player/Animation/MoveAttack.json");
    animator->LoadAnimationFromJson(L"JumpAttack", L"../Resources/sprites/Player/Animation/JumpAttack.json");


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

    if (_isAttacking)
    {
        _currentAnimTimer -= deltaTime;
        if (_currentAnimTimer <= 0) _isAttacking = false;
    }


    // 1. 일반 발사 (ButtonDown) - 기존 로직 유지
    if (InputManager::GetInstance().GetButtonDown(KeyType::A) && _currentCooldown <= 0.0f)
    {
        _currentWeapon->Fire(ChargeLevel::Normal);
        _isAttacking = true;
        _currentAnimTimer = _attackAnimDuration;
        _currentBurstCount--;
        _currentCooldown = (_currentBurstCount != 0) ? _attackInterval : _reloadCooldown;
        
        // 차지 타이머 초기화 및 차지 시작
        _chargeTimer = 0.0f;
        _isCharging = true;
    }

    // 2. 차지 진행 (Button)
    if (InputManager::GetInstance().GetButtonPressed(KeyType::A))
    {
        if (_isCharging)
        {
            _chargeTimer += deltaTime;
            if (_chargeTimer >= _maxChargeTime)
            {
                _chargeEffect->SetChargeLevel((int32)ChargeLevel::Max);
            }
            // TODO(USER): _chargeTimer가 일정 시간 이상이면 _chargeEffect를 활성화하고,
            // 플레이어의 위치에 맞춰 이펙트 좌표를 업데이트 해보세요!
            if (_chargeTimer >= _midChargeTime && _chargeEffect == nullptr)
            {
                _chargeEffect = new ChargeEffectActor();
                SceneManager::GetInstance().GetScene()->AddActor(_chargeEffect);
            }

            if (_chargeEffect != nullptr)
            {
                Vector pos = GetOwner()->GetPos();
                _chargeEffect->SetPos(pos);
            }
        }
    }

    // 3. 차지 샷 발사 (ButtonUp)
    if (InputManager::GetInstance().GetButtonUp(KeyType::A))
    {
        if (_isCharging && _chargeTimer >= _midChargeTime)
        {
            ChargeLevel level = ChargeLevel::None;

            // TODO(USER): _chargeTimer 값에 따라 ChargeLevel을 Mid 또는 Max로 판별하세요.
            if (_chargeTimer == 2)
            {
                level = ChargeLevel::Mid; // 임시
            }
            else if (_chargeTimer >= _maxChargeTime)
            {
                level = ChargeLevel::Max;
                
            }
            else
            {
                level = ChargeLevel::Mid;
            }

            // 발사 및 모션 재생
            _currentWeapon->Fire(level);
            _isAttacking = true;
            _currentAnimTimer = _attackAnimDuration;

            // TODO(USER): 발사 후 _chargeEffect를 비활성화(숨김 혹은 소멸) 처리하세요.
            _chargeEffect->Destroy();
            _chargeEffect = nullptr;
        }
        
        // 차지 상태 초기화
        _isCharging = false;
        _chargeTimer = 0.0f;
    }
}

void WeaponComponent::RenderUI()
{
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
