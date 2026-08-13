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
    ImGui::SliderFloat("Probability (%)", &_probability, 0.0f, 100.0f);
}
