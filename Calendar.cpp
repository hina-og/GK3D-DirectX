#include "Calendar.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"

Calendar::Calendar()
{
}

void Calendar::Initialize()
{
	calendar_.Initialize(0, 0, "Image\\Calendar.png");
	hImage_.Initialize(
		"Image\\CalendarAnim.png",//画像のパス
		0, 0,//画像切り取りの左上 (x,y)
		512, 512,//画像切り取りサイズ (w,h)
		false,//終わったら繰り返すか
		8,//フレーム数
		true//終わっても描画するか
	);
	hImage_.SetSpeed(0.8);
	hImage_.ChangeReverse();

	hNumber_ = Image::Load("Image\\CalendarNumber.png");
	assert(hNumber_ >= 0);

	hPageSE_ = Audio::Load("Sounds\\SE\\page.wav", false, 10);

	day_ = 0;

	tutoerialText_ = new Text;
	tutoerialText_->Initialize();
}

void Calendar::Update()
{
	Image::SetRect(
		hNumber_,//画像ハンドル
		0, day_ * 270,//切り取り開始位置
		256,270//切り抜き範囲
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
