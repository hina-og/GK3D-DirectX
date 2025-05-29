#include "SelectStage.h"
#include "Engine/Input.h"

SelectStage::SelectStage(GameObject* parent)
	: GameObject(parent, "Stage")
{
}

void SelectStage::Initialize()
{
	nowStage_ = 0;
	calendar.Initialize();

	decision_ = false;
}

void SelectStage::Update()
{
	if (Input::IsKeyDown(DIK_A) && 0 < nowStage_)
	{
		nowStage_--;
		calendar.Back();
		calendar.SetDay(nowStage_);
	}
	if (Input::IsKeyDown(DIK_D) && nowStage_ < STAGE_NUM)
	{
		nowStage_++;
		calendar.Next();
		calendar.SetDay(nowStage_);
	}
	calendar.Update();

	if (calendar.calendar_.isPress_ || Input::IsKeyDown(DIK_RETURN))
	{
		decision_ = true;
	}
}

void SelectStage::Draw()
{
	calendar.Draw();
}

void SelectStage::Release()
{
}

int SelectStage::GetStageNum()
{
	return nowStage_;
}
