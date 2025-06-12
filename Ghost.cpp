#include "Ghost.h"
#include "Engine/Model.h"

Ghost::Ghost(GameObject* parent)
{
}

void Ghost::Initialize()
{
	LoadStatus(GHOST);
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
