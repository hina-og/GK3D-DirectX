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

	bool isSelect_;
public:
	bool isDown_;//�������u��
	bool isPress_;//���������Ă���



	Button();
	void Initialize(int _x, int _y, std::string _fileName);
	void Initialize(int _x, int _y);
	void Update();
	void Draw();
	void Release();

	void LoadButtonImage(std::string _fileName);
	void SetPosition(XMFLOAT3 _pos);
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetSize();
	bool IsMouseInButton();
	void ChangeImage(std::string _fileName);
};

