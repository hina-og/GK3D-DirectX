#pragma once
#include "Animation.h"
#include "Button.h"

class Calendar
{
	Animation hImage_;
	int hNumber_;
	int day_;
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

