#pragma once

class Blackboard
{
public:
    Blackboard() {}
    ~Blackboard() {}

    // 기본적으로 자주 접근하는 포인터들 (스마트 포인터 금지 -> Raw Pointer 사용)
    class Actor* OwnerBoss = nullptr;
    class Actor* TargetPlayer = nullptr;

    // TODO: 그 외에 쿨타임(float)이나 추가 플래그(bool) 등을 저장할 맵 또는 변수들을 자유롭게 추가해 보세요.
    float WarpCooldown = 0.0f;
};
