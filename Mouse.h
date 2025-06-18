#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Mouse
	: public Puppet
{
public:
	Mouse(GameObject* parent);
	void Initialize() override;
	void Release();
	void Attack() override;
};

