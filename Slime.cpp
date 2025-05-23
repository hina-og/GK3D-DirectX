#include "Slime.h"
#include "Engine/Model.h"

Slime::Slime(GameObject* parent)
{
}

void Slime::Initialize()
{
	LoadStatus(SLIME);

	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, 180, 1);

	range_ =
	{
		{0,-2},
		{0,-1},
		{0, 0},
	};
	dir_ = DIRECTION::UP;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
}

void Slime::Update()
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

void Slime::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Slime::Release()
{
}

void Slime::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
	}
	attacked_ = true;
}
