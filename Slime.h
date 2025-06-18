#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Slime
	: public Puppet
{
public:
	Slime(GameObject* parent);
	void Initialize() override;
	void Release();
	void Attack() override;
};

