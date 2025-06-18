#include "Golem.h"
#include "Engine/Model.h"

Golem::Golem(GameObject* parent)
{
}

void Golem::Initialize()
{
	LoadStatus(GOLEM);

	EmitterData data;
	data.textureFileName = "Particle\\Cloud\\cloudB.png";
	data.delay = 0;
	//data.gravity = 0.0001f;
	data.direction = { 0, 1, 0 };
	data.speed = 0.0f;
	data.color = { 1, 1, 0, 0.7 };
	data.deltaColor = { 0, -0.03, 0, -0.02 };
	data.spin = { 0,0,5 };
	data.scale = { 0.9,0.9 };
	for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
	{
		particle_[rangeNum] = data;
	}
}

void Golem::Release()
{
}

void Golem::Attack()
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
