#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Mushroom
	: public Puppet
{
public:
	Mushroom(GameObject* parent);
	void Initialize() override;
	void Release();
	void Attack() override;
};

