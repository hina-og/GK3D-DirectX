#pragma once
#include "Engine/GameObject.h"
#include "Puppet.h"

class Slime
	: public Puppet
{
public:
	Slime(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	void Attack() override;
};

