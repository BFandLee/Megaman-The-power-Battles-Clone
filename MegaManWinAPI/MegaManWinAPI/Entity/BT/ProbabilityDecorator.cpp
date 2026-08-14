#include "pch.h"
#include "ProbabilityDecorator.h"
#include "Blackboard.h"

bool ProbabilityDecorator::checkCondition(Blackboard* bb)
{
    int randint = rand() % 100;
    if (randint < _probability)
    {
        return true;
    }
    return false;
}

void ProbabilityDecorator::DrawProperty()
{
    ImGui::PushItemWidth(150.0f);
    ImGui::SliderFloat("Probability (%)", &_probability, 0.0f, 100.0f);
    ImGui::PopItemWidth();
}

void ProbabilityDecorator::SaveProperty(json& j)
{
    j["Probability"] = _probability;
}

void ProbabilityDecorator::LoadProperty(const json& j)
{
    if (j.contains("Probability"))
    {
        _probability = j["Probability"];
    }
}
