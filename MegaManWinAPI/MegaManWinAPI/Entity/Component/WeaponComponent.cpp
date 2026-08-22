#include "pch.h"
#include "WeaponComponent.h"
#include "Weapon.h"
#include "Buster.h"
#include "InputManager.h"
#include "ObjectPoolManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "Scene.h"
#include "AnimatorComponent.h"
#include "ChargeEffectActor.h"
#include "Player.h"
#include "FSMComponent.h"

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
    animator->LoadAnimationFromJson(L"IdleAttack", L"Resources/sprites/Player/Animation/IdleAttack.json");
    animator->LoadAnimationFromJson(L"MoveAttack", L"Resources/sprites/Player/Animation/MoveAttack.json");
    animator->LoadAnimationFromJson(L"JumpAttack", L"Resources/sprites/Player/Animation/JumpAttack.json");


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

    // FSM 상태 확인: Spawn, Death 등 조작 불가 상태일 때는 공격 및 차징 차단
    bool isHitState = false;
    FSMComponent* fsm = GetOwner()->GetComponent<FSMComponent>();
    if (fsm != nullptr)
    {
        const std::string& state = fsm->GetCurrentStateName();
        if (state == "Spawn" || state == "Death")
        {
            if (_isCharging)
            {
                CancelCharge();
            }
            return;
        }
        else if (state == "Hit")
        {
            isHitState = true;
        }
    }

    // 1. 일반 발사 및 차징 시작 (ButtonDown)
    if (InputManager::GetInstance().GetButtonDown(KeyType::A))
    {
        if (!isHitState && _currentCooldown <= 0.0f)
        {
            _currentWeapon->Fire(ChargeLevel::Normal);
            _isAttacking = true;
            _currentAnimTimer = _attackAnimDuration;
            _currentBurstCount--;
            _currentCooldown = (_currentBurstCount != 0) ? _attackInterval : _reloadCooldown;
        }

        // 피격 중이든 일반/무적 상태든 A키를 누르면 차징 상태 활성화
        _chargeTimer = 0.0f;
        _isCharging = true;
        _isMidSoundPlayed = false;
        _isMaxSoundPlayed = false;
    }

    // 2. 차징 진행 (ButtonPressed)
    if (InputManager::GetInstance().GetButtonPressed(KeyType::A))
    {
        // A키를 누르고 있다면 피격/무적/일반 어떤 상태에서도 차징 활성화 보장
        if (!_isCharging)
        {
            _isCharging = true;
            _chargeTimer = 0.0f;
            _isMidSoundPlayed = false;
            _isMaxSoundPlayed = false;
        }

        _chargeTimer += deltaTime;

        // [1단계] Mid 차징 진입 -> megaman_charge (차징 시작음) 루프 재생
        if (_chargeTimer >= _midChargeTime && !_isMidSoundPlayed)
        {
            _isMidSoundPlayed = true;
            _chargeEffect->SetActive(true);
            _chargeEffect->SetChargeLevel((int32)ChargeLevel::Mid);

            SoundManager::GetInstance().PlaySFX(L"megaman_charge", true);
        }

        // [2단계] Max 풀차지 도달 -> megaman_charge 정지 후 megaman_chargeend (풀차지 완료음) 루프 재생
        if (_chargeTimer >= MAX_CHARGE_TIME && !_isMaxSoundPlayed)
        {
            _isMaxSoundPlayed = true;
            _chargeEffect->SetChargeLevel((int32)ChargeLevel::Max);

            SoundManager::GetInstance().StopSFX(L"megaman_charge");
            SoundManager::GetInstance().PlaySFX(L"megaman_chargeend", true);
        }

        // 차징 이펙트 위치 및 방향 갱신 (피격/무적/일반 모든 상태에서 실시간 플레이어 위치 추적)
        if (_chargeEffect != nullptr && _chargeEffect->GetActive())
        {
            Vector pos = GetOwner()->GetPos();
            _chargeEffect->SetPos(pos);

            Player* player = dynamic_cast<Player*>(GetOwner());
            if (player != nullptr)
            {
                float dirX = player->GetLookDirX();
                _chargeEffect->SetScale(Vector(dirX, 1.0f));
            }
        }
    }

    // 3. 차징 후 발사 및 버튼 해제 (ButtonUp)
    if (InputManager::GetInstance().GetButtonUp(KeyType::A))
    {
        // 🎵 모든 차징 사운드 즉시 정지
        SoundManager::GetInstance().StopSFX(L"megaman_charge");
        SoundManager::GetInstance().StopSFX(L"megaman_chargeend");

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

            // 차지샷 발사 (Buster::Fire 내부에서 Min/Max 발사 SFX 재생됨)
            _currentWeapon->Fire(level);
            _isAttacking = true;
            _currentAnimTimer = _attackAnimDuration;
        }

        _chargeEffect->SetActive(false);
        _isCharging = false;
        _chargeTimer = 0.0f;
        _isMidSoundPlayed = false;
        _isMaxSoundPlayed = false;
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

void WeaponComponent::CancelCharge()
{
    SoundManager::GetInstance().StopSFX(L"megaman_charge");
    SoundManager::GetInstance().StopSFX(L"megaman_chargeend");

    if (_chargeEffect != nullptr)
    {
        _chargeEffect->SetActive(false);
    }

    _isCharging = false;
    _chargeTimer = 0.0f;
    _isMidSoundPlayed = false;
    _isMaxSoundPlayed = false;
}
