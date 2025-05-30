#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Golem
	: public Puppet
{
public:
	Golem(GameObject* parent);
	void Initialize();
	void Draw();
	void Release();
	void Attack() override;
};

