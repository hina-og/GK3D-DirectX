#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"

class Unit
	: public GameObject
{
	std::vector<Puppet*> puppet_;
public:
	Unit();
	Unit(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	void AddCharacter(XMFLOAT3 _pos, int _type, Puppet::DIRECTION _dir);
	std::vector<Puppet*> GetPuppetArray();
	void InRange(std::vector<Puppet*> _puppet);
};

