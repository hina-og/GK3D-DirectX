#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Zombie
	: public Puppet
{
public:
	Zombie(GameObject* parent);
	void Initialize();
	void Release();
	void Attack() override;
};

