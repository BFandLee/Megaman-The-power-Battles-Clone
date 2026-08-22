#pragma once
#include "Blackboard.h"

// 보스전용 board
class BossBlackboard : public Blackboard
{
public:
    BossBlackboard() {}
    virtual ~BossBlackboard() override {} // 소멸자 오버라이딩

public:
    int _currentPhase = 1;
    float _cooltime = 0.0f;
    float _warpcooltime = 0.0f;
    bool _ishp = false;
};