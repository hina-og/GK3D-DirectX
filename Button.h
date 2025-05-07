#pragma once
#include "Engine/GameObject.h"
#include <string>

class Button
{
	int x_;
	int y_;
	int w_;
	int h_;

	int hPict_;

	int mouseX;
	int mouseY;
	bool prevLeftClick;
public:
	bool isPress_;

	Button();
	void Initialize(int _x, int _y, int _w, int _h, std::string _fileName);
	void Update();
	void Draw();
	void Release();

	bool IsMouseInButton(XMFLOAT3 _mousePos);
};

