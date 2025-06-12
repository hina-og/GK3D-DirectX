#include "Mouse.h"
#include "Engine/Model.h"

Mouse::Mouse(GameObject* parent)
{
}

void Mouse::Initialize()
{
	LoadStatus(MOUSE);
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
