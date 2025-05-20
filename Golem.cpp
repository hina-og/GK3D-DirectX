#include "Golem.h"
#include "Engine/Model.h"

Golem::Golem(GameObject* parent)
{
}

void Golem::Initialize()
{
	modelList_[STAND] = Model::Load("Model\\Enemy.fbx");
	assert(modelList_[STAND] >= 0);
	modelList_[RUN] = Model::Load("Model\\Enemy.fbx");
	assert(modelList_[RUN] >= 0);
	modelList_[ATTACK] = Model::Load("Model\\Enemy.fbx");
	assert(modelList_[ATTACK] >= 0);
	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, 180, 1);

	range_ =
	{
		{-1,-1}, {0,-1}, {1,-1},
		{-1, 0}, {0, 0}, {1, 0}
	};
	dir_ = DIRECTION::UP;

	hp_ = 21;
	cost_ = 1;
	power_ = 10;
	speed_ = 0.2;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
}

void Golem::Update()
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
		Model::SetAnimFrame(hModel_, 15, 170, 1);
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

void Golem::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Golem::Release()
{
}

void Golem::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-power_);
	}
	attacked_ = true;
}
