#include "pch.h"
#include "CheckCloneDurationDecorator.h"
#include "Blackboard.h"
#include "TimeManager.h"

CheckCloneDurationDecorator::CheckCloneDurationDecorator()
{
    SetName("CheckCloneDuration");
}

bool CheckCloneDurationDecorator::checkCondition(Blackboard* bb)
{
    if (bb->GetActor("BossClone", _clonePtr))
    {
        if (_clonePtr != nullptr)
        {
            float SpawnTime = 0.0f;
            if (bb->GetFloat("CloneSpawnTime", SpawnTime))
            {
                float currentTime = TimeManager::GetInstance().GetGlobalTime();

                if ((currentTime - SpawnTime) >= _minDuration)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void CheckCloneDurationDecorator::DrawProperty()
{
    ImGui::PushItemWidth(60.0f);
    ImGui::InputFloat("##Duration", &_minDuration, 0.0f, 0.0f, "%.1fs");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("Dur");
}

void CheckCloneDurationDecorator::SaveProperty(json& j)
{
    j["MinDuration"] = _minDuration;
}

void CheckCloneDurationDecorator::LoadProperty(const json& j)
{
    if (j.contains("MinDuration"))
    {
        _minDuration = j["MinDuration"];
    }
}
