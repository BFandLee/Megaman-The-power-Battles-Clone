#include "pch.h"
#include "CheckCloneDecorator.h"
#include "Blackboard.h"
#include "Actor.h"

CheckCloneDecorator::CheckCloneDecorator()
{
    SetName("CheckClone");
}

bool CheckCloneDecorator::checkCondition(Blackboard* bb)
{
    bool isExist = (bb->BossClone != nullptr && bb->BossClone->GetActive());
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
