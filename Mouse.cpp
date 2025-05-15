#include "Mouse.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

Mouse::Mouse(GameObject* parent)
{
}

void Mouse::Initialize()
{
	hModel_ = Model::Load("Model\\Zombie.fbx");
	assert(hModel_ >= 0);
	//Model::SetAnimFrame(hModel_, 5, 120, 1);

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
	//if (Input::IsKeyDown(DIK_W))
	//{
	//	dir_ = UP;
	//}
	//if (Input::IsKeyDown(DIK_A))
	//{
	//	dir_ = LEFT;
	//}
	//if (Input::IsKeyDown(DIK_S))
	//{
	//	dir_ = DOWN;
	//}
	//if (Input::IsKeyDown(DIK_D))
	//{
	//	dir_ = RIGHT;
	//}

	

	if (hp_ < 1)
	{
		isAlive_ = false;
	}

	//‰æ–Ê“à‚É‚¢‚È‚¯‚ê‚Î
	if (!InWindow(transform_.position_))
	{
		//KillMe();
	}

}

void Mouse::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

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
