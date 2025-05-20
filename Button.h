#pragma once
#include "Engine/GameObject.h"
#include <string>

class Button
{
	int x_;
	int y_;

	struct Size
	{
		int w_;
		int h_;
	};
	Size size_;

	int hPict_;

	int mouseX;
	int mouseY;
	bool prevLeftClick;
public:
	bool isPress_;

	Button();
	void Initialize(int _x, int _y, std::string _fileName);
	void Update();
	void Draw();
	void Release();

	void SetPosition(XMFLOAT3 _pos);
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetSize();
	bool IsMouseInButton(XMFLOAT3 _mousePos);
};

