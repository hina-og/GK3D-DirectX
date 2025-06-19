#pragma once
#include "Animation.h"
#include "Button.h"
#include "Engine/Text.h"

class Calendar
{
	Animation hImage_;
	int hPageSE_;
	int hNumber_;
	int day_;

	Text* tutoerialText_;
public:
	Button calendar_;

	Calendar();
	void Initialize();
	void Update();
	void Draw();
	void Back();
	void Next();
	void SetDay(int _day);
};

