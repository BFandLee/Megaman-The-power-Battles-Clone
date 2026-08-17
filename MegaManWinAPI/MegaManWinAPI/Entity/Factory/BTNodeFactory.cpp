#include "pch.h"
#include "BTNodeFactory.h"
#include "Sequence.h"
#include "Selector.h"

// 보스 ActionNode
#include "ActionNode.h"
#include "GeminiCommonNodes.h"
#include "GeminiPatten.h"

// 보스 Decorator
#include "CooldownDecorator.h"
#include "ProbabilityDecorator.h"
#include "CheckCloneDecorator.h"
#include "CheckPlayerAttackingDecorator.h"
#include "SucceederDecorator.h"
#include "CheckCloneDurationDecorator.h"

BTNodeFactory::BTNodeFactory()
{
    // Compsite Node
    _registry["Sequence"] = []() -> BTNode* { return new Sequence(); };
    _registry["Selector"] = []() -> BTNode* { return new Selector(); };
    
    // Root Node
    _registry["Root"] = []() -> BTNode* { return new Selector(); };

    // Decorator Node
    _registry["Cooldown"] = []() -> BTNode* { return new CooldownDecorator(); };
    _registry["Probability"] = []() -> BTNode* { return new ProbabilityDecorator(); };
    _registry["CheckClone"] = []() -> BTNode* { return new CheckCloneDecorator(); };
    _registry["CheckPlayerAttacking"] = []() -> BTNode* { return new CheckPlayerAttackingDecorator(); };
    _registry["Succeeder"] = []() -> BTNode* { return new SucceederDecorator(); };
    _registry["CheckCloneDuration"] = []() -> BTNode* { return new CheckCloneDurationDecorator(); };

    // Action Node
    _registry["Jump"] = []() -> BTNode* { return new BTAction_Gemini_Jump(); };
    _registry["Move"] = []() -> BTNode* { return new BTAction_Gemini_Move(); };
    _registry["Idle"] = []() -> BTNode* { return new BTAction_Gemini_Idle(); };
    _registry["BaseMissile"] = []() -> BTNode* { return new BTAction_Gemini_BaseMissile(); };
    _registry["LaserMissile"] = []() -> BTNode* { return new BTAction_Gemini_LaserMissile(); };
    _registry["Clone"] = []() -> BTNode* { return new BTAction_Gemini_CloneActivate(); };
    _registry["CloneDeactivate"] = []() -> BTNode* { return new BTAction_Gemini_CloneDeactivate(); };
    
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
