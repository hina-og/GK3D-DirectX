#include "Zombie.h"
#include "Engine/Model.h"

Zombie::Zombie(GameObject* parent)
{
}

void Zombie::Initialize()
{
	LoadStatus(ZOMBIE);

	EmitterData data;
	data.textureFileName = "Particle\\Others\\bite.png";
	data.delay = 0;
	data.speed = 0.0f;
	data.color = { 1, 0, 0, 0.8 };
	data.deltaColor = { 0, 0, 0, -0.02 };
	for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
	{
		particle_[rangeNum] = data;
	}
}

void Zombie::Release()
{
}

void Zombie::Attack()
{
	for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
	{
		particle_[rangeNum].position = rangePos_[rangeNum];
		VFX::Start(particle_[rangeNum]);
	}


	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
		break;
	}
	attacked_ = true;
}
