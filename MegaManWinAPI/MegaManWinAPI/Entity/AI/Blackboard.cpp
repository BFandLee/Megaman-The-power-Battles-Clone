#include "pch.h"
#include "Blackboard.h"

void Blackboard::SetFloat(const std::string& key, float value)
{
	FloatData[key] = value;
}

bool Blackboard::GetFloat(const std::string& key, float& outValue)
{
	auto it = FloatData.find(key);
	if (it != FloatData.end())
	{
		outValue = FloatData[key];
		return true;
	}
	return false;
}

void Blackboard::SetBool(const string& key, bool value)
{
	BoolData[key] = value;
}

bool Blackboard::GetBool(const string& key, bool& outValue)
{
	auto it = BoolData.find(key);
	if (it != BoolData.end())
	{
		outValue = BoolData[key];
		return true;
	}
	return false;
}
