#pragma once
#include "Singleton.h"

// 전방 선언
class BTNode;

class BTNodeFactory : public Singleton<BTNodeFactory>
{
    friend Singleton<BTNodeFactory>;
private:
    BTNodeFactory();
    ~BTNodeFactory() = default;

public:
    // JSON에서 읽어온 타입(문자열)을 기반으로 적절한 노드 객체를 생성
    BTNode* CreateNode(const std::string& typeName);

private:
    // "Sequence" -> new Sequence() 식의 생성 함수들을 보관하는 맵 (Registry)
    std::map<std::string, std::function<BTNode*()>> _registry;
};
