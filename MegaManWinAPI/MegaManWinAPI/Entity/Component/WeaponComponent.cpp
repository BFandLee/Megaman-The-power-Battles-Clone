#include "pch.h"
#include "WeaponComponent.h"
#include "../Weapon/Weapon.h"

WeaponComponent::WeaponComponent()
{
}

WeaponComponent::~WeaponComponent()
{
    // TODO: 현재 장착된 무기가 있다면 메모리 해제(delete)
}

void WeaponComponent::Init()
{
    // TODO: 기본 무기(Buster 등)를 동적 할당하여 _currentWeapon에 장착
}

void WeaponComponent::Update(float deltaTime)
{
    // TODO: 공격 키 입력을 검사하고 _currentWeapon->Fire() 호출
    // TODO: 발사 후 일정 시간 동안 _isAttacking을 true로 유지하다가 false로 돌리는 쿨타임 처리
}

void WeaponComponent::RenderUI()
{
    // TODO: ImGui로 무기 데미지 등의 데이터를 실시간 수정할 수 있게 구현
}

json WeaponComponent::ToJson()
{
    json j = Component::ToJson();
    // TODO: 무기 설정 데이터를 json에 직렬화
    return j;
}

void WeaponComponent::FromJson(const json& j)
{
    Component::FromJson(j);
    // TODO: 무기 설정 데이터를 json에서 복원
}

void WeaponComponent::ChangeWeapon(Weapon* weapon)
{
    // TODO: 무기 교체 전 기존 무기가 있다면 delete하고 새 무기로 포인터 갱신--> 나중에 플레이어 폼 추가되면 그때가서 구현
}
