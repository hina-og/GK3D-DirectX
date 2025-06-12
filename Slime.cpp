#include "Slime.h"
#include "Engine/Model.h"

Slime::Slime(GameObject* parent)
{
}

void Slime::Initialize()
{
	LoadStatus(SLIME);
}

void Slime::Release()
{
}

void Slime::Attack()
{
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
		break;
	}
	attacked_ = true;
}
