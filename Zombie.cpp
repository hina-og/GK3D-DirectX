#include "Zombie.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Model.h"

Zombie::Zombie(GameObject* parent)
{
}

void Zombie::Initialize()
{
	modelList_[STAND] = Model::Load("Model\\ZombieD.fbx");
	assert(modelList_[STAND] >= 0);
	modelList_[RUN] = Model::Load("Model\\ZombieD_Run.fbx");
	assert(modelList_[RUN] >= 0);
	modelList_[ATTACK] = Model::Load("Model\\ZombieD_Attack.fbx");
	assert(modelList_[ATTACK] >= 0);
	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 20, 170, 2);

	range_ = 
	{ 
		{-1,-1}, {0,-1}, {1,-1},
		{-1, 0}, {0, 0}, {1, 0},
	};
	dir_ = DIRECTION::UP;

	hp_ = 20;
	cost_ = 1;
	power_ = 5;
	speed_ = 0.8;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
}

void Zombie::Update()
{
	FacingDirection();

	if (isAttack_)
	{
		if (hModel_ != modelList_[ATTACK])
		{
			hModel_ = modelList_[ATTACK];
			Model::SetAnimFrame(hModel_, 1, 180, 1);
		}
		if (Model::GetAnimFrame(hModel_) >= 90 && !attacked_)
			Attack();
	}
	else if (hModel_ != modelList_[RUN])
	{
		hModel_ = modelList_[RUN];
		Model::SetAnimFrame(hModel_, 15, 170, 2);
	}


	if (hModel_ == modelList_[ATTACK] && Model::GetAnimFrame(hModel_) >= 180)
	{
		isAttack_ = false;
		attacked_ = false;
	}

	if (hp_ < 1)
	{
		KillMe();
	}
}

void Zombie::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Zombie::Release()
{
}

void Zombie::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-power_);
	}
	attacked_ = true;
}
