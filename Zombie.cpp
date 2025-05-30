#include "Zombie.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Model.h"

Zombie::Zombie(GameObject* parent)
{
}

void Zombie::Initialize()
{
	LoadStatus(ZOMBIE);

	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, animData_.totalRunFrame_, animData_.runSpeed_);

	dir_ = DIRECTION::UP;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
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
		inRangeChara_[i]->ControlHP(-status_.power_);
		break;
	}
	attacked_ = true;
}
