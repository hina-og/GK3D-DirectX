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
	if ((Input::IsKeyDown(DIK_A) || Input::GetMouseMove().z < 0) && 0 < nowStage_)
	{
		nowStage_--;
		calendar.Back();
		//calendar.SetDay(nowStage_);
	}
	if ((Input::IsKeyDown(DIK_D) || Input::GetMouseMove().z > 0) && nowStage_ < stageNum_)
	{
		nowStage_++;
		calendar.Next();
		//calendar.SetDay(nowStage_);
	}
	calendar.Update();

	if (calendar.calendar_.isDown_ || Input::IsKeyDown(DIK_RETURN))
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

void SelectStage::SetStageNum(int _num)
{
	stageNum_ = _num;
}

int SelectStage::GetStageNum()
{
	return nowStage_;
}
