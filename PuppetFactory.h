#pragma once
#include "Mouse.h"
#include "Zombie.h"
#include "Mushroom.h"
#include "Slime.h"
#include "Golem.h"
#include "Ghost.h"
#include <string>
#include <map>

//実体を作る
inline Puppet* CreatePuppetByType(int _type, GameObject* _parent)
{
	switch (_type)
	{
	case CHARA_TYPE::MOUSE:
		return new Mouse(_parent);
	case CHARA_TYPE::ZOMBIE:
		return new Zombie(_parent);
	case CHARA_TYPE::MUSHROOM:
		return new Mushroom(_parent);
	case CHARA_TYPE::SLIME:
		return new Slime(_parent);
	case CHARA_TYPE::GOLEM:
		return new Golem(_parent);
	case CHARA_TYPE::GHOST:
		return new Ghost(_parent);
	default:
		break;
	}
	return nullptr;
}

//↑のCreatePuppetByNameのInstantiateバージョン
inline Puppet* CreateInitPuppetByType(int _type, GameObject* _parent)
{
	switch (_type)
	{
	case CHARA_TYPE::MOUSE:
		return Instantiate<Mouse>(_parent);
	case CHARA_TYPE::ZOMBIE:
		return Instantiate<Zombie>(_parent);
	case CHARA_TYPE::MUSHROOM:
		return Instantiate<Mushroom>(_parent);
	case CHARA_TYPE::SLIME:
		return Instantiate<Slime>(_parent);
	case CHARA_TYPE::GOLEM:
		return Instantiate<Golem>(_parent);
	case CHARA_TYPE::GHOST:
		return Instantiate<Ghost>(_parent);
	default:
		break;
	}
	return nullptr;
}

//キャラの名前→種類番号に変換
inline int GetCharacterTypeFromName(const std::string& _name)
{
	static const std::map<std::string, int> map = {
		{"Mouse", CHARA_TYPE::MOUSE},
		{"Zombie", CHARA_TYPE::ZOMBIE},
		{"Mushroom", CHARA_TYPE::MUSHROOM},
		{"Slime", CHARA_TYPE::SLIME},
		{"Golem", CHARA_TYPE::GOLEM},
		{"Ghost", CHARA_TYPE::GHOST}
	};
	auto it = map.find(_name);
	return (it != map.end()) ? it->second : -1;
}

//キャラの種類番号→名前に変換
inline std::string GetCharacterNameFromType(int _type)
{
	static const std::string names[] = {
		"Mouse",
		"Zombie",
		"Mushroom",
		"Slime",
		"Golem",
		"Ghost"
	};
	if (_type >= 0 && _type < CHARA_TYPE::CHARA_END)
		return names[_type];
	return "Unknown";
}
