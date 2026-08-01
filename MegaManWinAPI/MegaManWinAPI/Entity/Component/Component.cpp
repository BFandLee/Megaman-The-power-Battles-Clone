#include "pch.h"
#include "Component.h"

Component::Component(string name) : _name(name)
{

}

json Component::ToJson()
{
	json j;
	j["name"] = _name;
	return j;
}

void Component::FromJson(const json& j)
{
	if (j.contains("name"))
		_name = j["name"];
}
