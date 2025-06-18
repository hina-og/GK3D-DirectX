#include "Ghost.h"
#include "Engine/Model.h"

Ghost::Ghost(GameObject* parent)
{
}

void Ghost::Initialize()
{
	LoadStatus(GHOST);

	EmitterData data;

	data.textureFileName = "Particle\\Others\\face.png";
	data.delay = 0;
	data.gravity = -0.003f;
	data.direction = { 0, 1, 0 };
	data.speed = 0.0f;
	data.color = { 1, 0, 0, 0.6 };
	for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
	{
		particle_[rangeNum] = data;
	}
}

void Ghost::Release()
{
}

void Ghost::Attack()
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
