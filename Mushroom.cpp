#include "Mushroom.h"
#include "Engine/Model.h"

Mushroom::Mushroom(GameObject* parent)
{
}

void Mushroom::Initialize()
{
	LoadStatus(MUSHROOM);

	EmitterData data;
	data.textureFileName = "Particle\\Flash\\flashA_R.png";
	data.delay = 0;
	data.gravity = -0.003f;
	data.direction = { 0, 1, 0 };
	data.speed = 0.0f;
	data.color = { 1, 1, 0, 0.8 };
	data.deltaColor = { 0, -0.03, 0, -0.02 };
	data.spin = { 0,0,5 };
	for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
	{
		particle_[rangeNum] = data;
	}
}

void Mushroom::Release()
{
}

void Mushroom::Attack()
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
