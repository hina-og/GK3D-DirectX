#include "Ghost.h"
#include "Engine/Model.h"

Ghost::Ghost(GameObject* parent)
{
}

void Ghost::Initialize()
{
	LoadStatus(GHOST);

	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, animData_.totalRunFrame_, animData_.runSpeed_);

	dir_ = DIRECTION::UP;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
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
		break;
	}
	attacked_ = true;
}
