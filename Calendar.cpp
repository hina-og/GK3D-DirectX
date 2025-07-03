#include "Calendar.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"

Calendar::Calendar()
{
}

void Calendar::Initialize()
{
	calendar_.Initialize(0, 0, "Image\\Calendar.png");


	//�A�j���[�V�����̏�����
	hImage_.Initialize(CALENDER);



	hNumber_ = Image::Load("Image\\CalendarNumber.png");
	assert(hNumber_ >= 0);

	hPageSE_ = Audio::Load("Sounds\\SE\\page.wav", false, 10);
	assert(hPageSE_ >= 0);

	day_ = 0;

	tutoerialText_ = new Text;
	tutoerialText_->Initialize();
}

void Calendar::Update()
{
	Image::SetRect(
		hNumber_,//�摜�n���h��
		0, day_ * 270,//�؂���J�n�ʒu
		256,270//�؂蔲���͈�
	);
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

	//0���ڂ̓`���[�g���A��
	if (day_ == 0)
	{
		XMINT2 textPos = { 550,650 };
		tutoerialText_->Draw(textPos.x, textPos.y, "Tutorial");
	}
}

void Calendar::Back()
{
	if (!hImage_.NowReverse())
		hImage_.ChangeReverse();
	hImage_.Start();
	Audio::Play(hPageSE_);
	day_ -= 1;
}

void Calendar::Next()
{
	if (hImage_.NowReverse())
		hImage_.ChangeReverse();
	hImage_.Start();		
	Audio::Play(hPageSE_);
	day_ += 1;
}

void Calendar::SetDay(int _day)
{
	day_ = _day;
}
