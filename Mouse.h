#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Mouse
	: public Puppet
{
public:
	Mouse(GameObject* parent);
	void Initialize();
	void Release();
	void Attack() override;
};

