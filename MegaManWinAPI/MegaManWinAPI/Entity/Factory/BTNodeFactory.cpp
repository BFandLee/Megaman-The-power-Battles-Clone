#include "pch.h"
#include "BTNodeFactory.h"
#include "Sequence.h"
#include "Selector.h"

// 보스 ActionNode
#include "ActionNode.h"
#include "GeminiCommonNodes.h"
#include "GeminiPhase1Nodes.h"
#include "GeminiPhase2Nodes.h"

// 보스 Decorator
#include "CooldownDecorator.h"
#include "ProbabilityDecorator.h"
#include "CheckCloneDecorator.h"

BTNodeFactory::BTNodeFactory()
{
    // Compsite Node
    _registry["Sequence"] = []() -> BTNode* { return new Sequence(); };
    _registry["Selector"] = []() -> BTNode* { return new Selector(); };

    // Decorator Node
    _registry["Cooldown"] = []() -> BTNode* { return new CooldownDecorator(); };
    _registry["Probability"] = []() -> BTNode* { return new ProbabilityDecorator(); };
    _registry["CheckClone"] = []() -> BTNode* { return new CheckCloneDecorator(); };

    // Action Node
    _registry["Jump"] = []() -> BTNode* { return new BTAction_Gemini_Jump(); };
    _registry["Move"] = []() -> BTNode* { return new BTAction_Gemini_Move(); };
    _registry["Idle"] = []() -> BTNode* { return new BTAction_Gemini_Idle(); };
    _registry["BaseMissile"] = []() -> BTNode* { return new BTAction_Gemini_BaseMissile(); };
    _registry["Clone"] = []() -> BTNode* { return new BTAction_Gemini_CloneActivate(); };
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
