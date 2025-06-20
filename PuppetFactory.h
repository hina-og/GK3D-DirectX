#pragma once

#include "Mouse.h"
#include "Zombie.h"
#include "Mushroom.h"
#include "Slime.h"
#include "Golem.h"
#include "Ghost.h"
#include <string>
#include <unordered_map>

inline Puppet* CreatePuppetByName(const std::string& name, GameObject* parent)
{
	if (name == "Mouse")     return new Mouse(parent);
	if (name == "Zombie")    return new Zombie(parent);
	if (name == "Mushroom")  return new Mushroom(parent);
	if (name == "Slime")     return new Slime(parent);
	if (name == "Golem")     return new Golem(parent);
	if (name == "Ghost")     return new Ghost(parent);
	return nullptr;
}

inline int GetCharacterTypeFromName(const std::string& name)
{
	static const std::unordered_map<std::string, int> map = {
		{"Mouse", CHARA_TYPE::MOUSE},
		{"Zombie", CHARA_TYPE::ZOMBIE},
		{"Mushroom", CHARA_TYPE::MUSHROOM},
		{"Slime", CHARA_TYPE::SLIME},
		{"Golem", CHARA_TYPE::GOLEM},
		{"Ghost", CHARA_TYPE::GHOST}
	};
	auto it = map.find(name);
	return (it != map.end()) ? it->second : -1;
}

inline std::string GetCharacterNameFromType(int type)
{
	static const std::string names[] = {
		"Mouse",
		"Zombie",
		"Mushroom",
		"Slime",
		"Golem",
		"Ghost"
	};
	if (type >= 0 && type < CHARA_TYPE::CHARA_END)
		return names[type];
	return "Unknown";
}
