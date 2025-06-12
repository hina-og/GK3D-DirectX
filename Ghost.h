#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Ghost
	: public Puppet
{
public:
	Ghost(GameObject* parent);
	void Initialize();
	void Release();
	void Attack() override;
};

