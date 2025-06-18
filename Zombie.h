#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Zombie
	: public Puppet
{
public:
	Zombie(GameObject* parent);
	void Initialize() override;
	void Release();
	void Attack() override;
};

