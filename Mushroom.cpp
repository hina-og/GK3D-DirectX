#include "Mushroom.h"
#include "Engine/Model.h"

Mushroom::Mushroom(GameObject* parent)
{
}

void Mushroom::Initialize()
{
	LoadStatus(MUSHROOM);

	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, 60, 1);

	range_ =
	{
		{-1,-1}, {0,-1}, {1,-1},
		{-1, 0}, {0, 0}, {1, 0},
		{ 1, 1}, {0, 1}, {1, 1},
	};
	dir_ = DIRECTION::UP;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
}

void Mushroom::Update()
{
	FacingDirection();

	if (isAttack_)
	{
		if (hModel_ != modelList_[ATTACK])
		{
			hModel_ = modelList_[ATTACK];
			Model::SetAnimFrame(hModel_, 1, 120, 1);
		}
		if (Model::GetAnimFrame(hModel_) >= 90 && !attacked_)
			Attack();
	}
	else if (hModel_ != modelList_[RUN])
	{
		hModel_ = modelList_[RUN];
		Model::SetAnimFrame(hModel_, 10, 60, 1);
	}


	if (hModel_ == modelList_[ATTACK] && Model::GetAnimFrame(hModel_) >= 120)
	{
		isAttack_ = false;
		attacked_ = false;
	}

	if (status_.hp_ < 1)
	{
		KillMe();
	}
}

void Mushroom::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Mushroom::Release()
{
}

void Mushroom::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
	}
	attacked_ = true;
}
