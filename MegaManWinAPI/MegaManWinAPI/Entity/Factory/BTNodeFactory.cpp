#include "pch.h"
#include "BTNodeFactory.h"
#include "Sequence.h"
#include "Selector.h"

BTNodeFactory::BTNodeFactory()
{
    // TODO 2: _registry 맵에 "클래스이름"을 키로, 생성 함수를 밸류로 등록하세요.
    _registry["Sequence"] = []() -> BTNode* { return new Sequence(); };
    _registry["Selector"] = []() -> BTNode* { return new Selector(); };
}

BTNode* BTNodeFactory::CreateNode(const std::string& typeName)
{
    auto name = _registry.find(typeName);
    
    if (name != _registry.end())
    {
        return name->second();
    }
    return nullptr;
}
