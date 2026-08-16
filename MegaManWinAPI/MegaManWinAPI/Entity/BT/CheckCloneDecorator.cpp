#include "pch.h"
#include "CheckCloneDecorator.h"
#include "Blackboard.h"

bool CheckCloneDecorator::checkCondition(Blackboard* bb)
{
    bool isExist = false;
    if (bb->GetActor("BossClone", _clonePtr))
    {
        if (_clonePtr != nullptr)
        {
            isExist = true;
        }
    }
    return isExist == _checkExist;
}

void CheckCloneDecorator::DrawProperty()
{
    ImGui::Checkbox("Check Exist", &_checkExist);
}
