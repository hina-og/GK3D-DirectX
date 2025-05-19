#include "Mouse.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

Mouse::Mouse(GameObject* parent)
{
}

void Mouse::Initialize()
{
	hModel_ = Model::Load("Model\\ZombieD_Attack.fbx");
	assert(hModel_ >= 0);
	Model::SetAnimFrame(hModel_, 20, 170, 2);

	hPict_ = Image::Load("Image\\Mouse.png");
	assert(hPict_ > 0);

	rangePict_ = Image::Load("Image\\Range.png");
	assert(rangePict_ > 0);

	range_ =
	{
		{-1,-1}, {0,-1}, {1,-1},
				 {0, 0}
	};
	dir_ = DIRECTION::UP;

	hp_ = 100;
	cost_ = 1;
	power_ = 5;
	speed_ = 1.0;

	isAlive_ = true;
}

void Mouse::Update()
{
	if (isAttack_)
	{
		Attack();
	}
	else if (Model::GetAnimFrame(hModel_) >= 115)
	{
		//Model::SetAnimFrame(hModel_, 6, 115, 1);
	}
	

	if (hp_ < 1)
	{
		isAlive_ = false;
	}

}

void Mouse::Draw()
{
	if (!isAttack_)
	{
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	}
	//Image::SetTransform(hPict_, transform_);
	//Image::Draw(hPict_);
}

void Mouse::Release()
{
}

void Mouse::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-power_);
	}
}
