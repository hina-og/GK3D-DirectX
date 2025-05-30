#include "Golem.h"
#include "Engine/Model.h"

Golem::Golem(GameObject* parent)
{
}

void Golem::Initialize()
{
	LoadStatus(GOLEM);

	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, animData_.totalRunFrame_, animData_.runSpeed_);

	dir_ = DIRECTION::UP;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
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
		inRangeChara_[i]->ControlHP(-status_.power_);
		break;
	}
	attacked_ = true;
}
