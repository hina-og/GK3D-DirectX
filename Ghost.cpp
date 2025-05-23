#include "Ghost.h"
#include "Engine/Model.h"

Ghost::Ghost(GameObject* parent)
{
}

void Ghost::Initialize()
{
	LoadStatus(GHOST);

	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, 180, 1);

	range_ =
	{
		{-1,-3}, {0,-3}, {1,-3},
		{-1,-2}, {0,-2}, {1,-2},
		{-1,-1}, {0,-1}, {1,-1},
				 {0, 0}
	};
	dir_ = DIRECTION::UP;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
}

void Ghost::Update()
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

	if (status_.hp_ < 1)
	{
		KillMe();
	}
}

void Ghost::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ghost::Release()
{
}

void Ghost::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
	}
	attacked_ = true;
}
