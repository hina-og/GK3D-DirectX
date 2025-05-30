#include "Mouse.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

Mouse::Mouse(GameObject* parent)
{
}

void Mouse::Initialize()
{
	LoadStatus(MOUSE);

	hModel_ = modelList_[STAND];
	Model::SetAnimFrame(hModel_, 1, animData_.totalRunFrame_, animData_.runSpeed_);

	dir_ = DIRECTION::UP;

	isAlive_ = true;
	isAttack_ = false;
	attacked_ = false;
}

void Mouse::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Mouse::Release()
{
}

void Mouse::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
		break;
	}
	
	attacked_ = true;
}
