#include "pch.h"
#include "WeaponComponent.h"
#include "Weapon.h"
#include "Buster.h"
#include "InputManager.h"
#include "ObjectPoolManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "AnimatorComponent.h"
#include "ChargeEffectActor.h"
#include <Player.h>

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

    _chargeEffect = new ChargeEffectActor();
    _chargeEffect->Init();
    SceneManager::GetInstance().GetScene()->AddActor(_chargeEffect);
    _chargeEffect->SetActive(false);

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
            if (!_chargeEffect->GetActive() && _chargeTimer > _midChargeTime)
            {
                _chargeEffect->SetActive(true);
            }
            if (_chargeEffect != nullptr)
            {
                // 이펙트 위치 업데이트
                Vector pos = GetOwner()->GetPos();
                _chargeEffect->SetPos(pos);

                // FlipX 구현
                Player* player = static_cast<Player*>(GetOwner());
                float dirX = player->GetLookDirX();
                _chargeEffect->SetScale(Vector(dirX, 1.0f));

                // 2. 차지 시간에 따른 레벨(애니메이션) 업데이트
                if (_chargeTimer >= MAX_CHARGE_TIME)
                {
                    _chargeEffect->SetChargeLevel((int32)ChargeLevel::Max);
                }
                else if (_chargeTimer >= _midChargeTime)
                {
                    _chargeEffect->SetChargeLevel((int32)ChargeLevel::Mid);
                }
            }
        }
    }

    // 3. 차지 샷 발사 (ButtonUp)
    if (InputManager::GetInstance().GetButtonUp(KeyType::A))
    {
        if (_isCharging && _chargeTimer >= _midChargeTime)
        {
            ChargeLevel level = ChargeLevel::None;
            if (_chargeTimer >= MAX_CHARGE_TIME)
            {
                level = ChargeLevel::Max;
                
            }
            else if (_chargeTimer >= _midChargeTime)
            {
                level = ChargeLevel::Mid;
            }

            // 발사 및 모션 재생
            _currentWeapon->Fire(level);
            _isAttacking = true;
            _currentAnimTimer = _attackAnimDuration;

            
        }
        _chargeEffect->SetActive(false);



        
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
