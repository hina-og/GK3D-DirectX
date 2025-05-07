#include "Zombie.h"
#include "Engine/Image.h"

Zombie::Zombie(GameObject* parent)
{
}

void Zombie::Initialize()
{
	hPict_ = Image::Load("Image\\Zombie.png");
	assert(hPict_ > 0);

	rangePict_ = Image::Load("Image\\Range.png");
	assert(rangePict_ > 0);

	range_ = 
	{ 
		{-1,-1}, {0,-1}, {1,-1},
		{-1, 0}, {0, 0}, {1, 0},
		{-1, 1}, {0, 1}, {1, 1} 
	};
	dir_ = DIRECTION::UP;

	hp_ = 20;
	cost_ = 3;
	power_ = 7;

	isAlive_ = true;
}

void Zombie::Update()
{


	if (isAttack_)
	{
		Attack();
	}
	else
	{
		transform_.position_.y += 1;
	}

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

void Zombie::Draw()
{
	//Image::Draw(transform_.position_.x, transform_.position_.y, hPict_);
}

void Zombie::Release()
{
}

void Zombie::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-power_);
	}
}
