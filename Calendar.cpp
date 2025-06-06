#include "Calendar.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"

Calendar::Calendar()
{
}

void Calendar::Initialize()
{
	calendar_.Initialize(0, 0, "Image\\Calendar.png");
	hImage_.Initialize("Image\\CalendarAnim.png", 0, 0, 512, 512, false, 8, true);
	hImage_.SetSpeed(0.8);
	hImage_.ChangeReverse();

	hNumber_ = Image::Load("Image\\CalendarNumber.png");
	assert(hNumber_ >= 0);

	hPageSE_ = Audio::Load("Sounds\\SE\\page.wav", false, 10);

	day_ = 0;
}

void Calendar::Update()
{
	Image::SetRect(hNumber_, 0, day_ * 270, 256, 270);
	calendar_.Update();
}

void Calendar::Draw()
{
	calendar_.Draw();
	if (hImage_.GetFrame() <= 7)
	{
		Image::Draw(hNumber_);
		hImage_.Draw();
	}
	else
	{
		hImage_.Draw();
		Image::Draw(hNumber_);
	}
	hImage_.Update();
}

void Calendar::Back()
{
	if (!hImage_.NowReverse())
		hImage_.ChangeReverse();
	hImage_.Start();
	Audio::Play(hPageSE_);
}

void Calendar::Next()
{
	if (hImage_.NowReverse())
		hImage_.ChangeReverse();
	hImage_.Start();		
	Audio::Play(hPageSE_);
}

void Calendar::SetDay(int _day)
{
	day_ = _day;
}
