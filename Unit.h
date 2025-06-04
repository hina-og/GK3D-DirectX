#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"
#include "Mushroom.h"
#include "Slime.h"
#include "Golem.h"
#include "Ghost.h"

class Unit
	: public GameObject
{
	std::vector<Puppet*> puppet_;
	
	int shakePower_;
	float shakeTime_;
public:
	Unit();
	Unit(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	void AddCharacter(XMFLOAT3 _pos, int _type, Puppet::DIRECTION _dir);
	void AddCharacter(XMFLOAT3 _pos, int _type, int _dir);
	std::vector<Puppet*> GetPuppetArray();
	void InvaderMove();
	void InRange(std::vector<Puppet*> _puppet);
	void PastLine(float _z, int& _durability);
};

