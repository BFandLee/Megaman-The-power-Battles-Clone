#include "pch.h"
#include "CheckCloneDecorator.h"
#include "Blackboard.h"

CheckCloneDecorator::CheckCloneDecorator()
{
    SetName("CheckClone");
}

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

void CheckCloneDecorator::SaveProperty(json& j)
{
    j["CheckExist"] = _checkExist;
}

void CheckCloneDecorator::LoadProperty(const json& j)
{
    if (j.contains("CheckExist"))
    {
        _checkExist = j["CheckExist"];
    }
}
