#pragma once
#include "Animation.h"
#include "Button.h"
#include "Engine/Text.h"

class Calendar
{
	//ハンドル
	Animation hImage_;//ページをめくるアニメーション
	int hPageSE_;//ページをめくる音
	int hNumber_;//ステージの数字


	int day_;
	Text* tutoerialText_;
public:
	Button calendar_;

	Calendar();
	void Initialize();
	void Update();
	void Draw();
	//前に行く
	void Back();
	//次に行く
	void Next();
	void SetDay(int _day);
};

