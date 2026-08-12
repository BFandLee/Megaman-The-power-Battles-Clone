#pragma once
#include <map>
#include <string>

class Blackboard
{
public:
    Blackboard() {}
    virtual ~Blackboard() {}

    // 기본적으로 자주 접근하는 포인터들 (스마트 포인터 금지 -> Raw Pointer 사용)
    class Actor* OwnerBoss = nullptr;
    class Actor* TargetPlayer = nullptr;

    // ---------------------------------------------------
    // 타입별로 맵(Map)을 나누어 관리하여 캐스팅 에러 방지
    // ---------------------------------------------------
    std::map<std::string, float> FloatData;
    std::map<std::string, bool> BoolData;
    // std::map<std::string, Vector> VectorData; // 필요시 주석 해제 후 사용
    std::map<std::string, class Object*> ObjectData;

    // TODO: Blackboard에서 데이터를 쉽게 Get/Set 할 수 있는 헬퍼 함수를 선언하고 구현해 보세요.
    // 예: void SetFloat(const std::string& key, float value);
    // 예: bool GetFloat(const std::string& key, float& outValue);
};
