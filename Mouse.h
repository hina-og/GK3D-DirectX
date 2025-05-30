#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Mouse
	: public Puppet
{
public:
	Mouse(GameObject* parent);
	void Initialize();
	void Draw();
	void Release();
	void Attack() override;
};

