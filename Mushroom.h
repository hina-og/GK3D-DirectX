#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Mushroom
	: public Puppet
{
public:
	Mushroom(GameObject* parent);
	void Initialize();
	void Draw();
	void Release();
	void Attack() override;
};

