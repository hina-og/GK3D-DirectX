#include "Button.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

Button::Button()
{
}

void Button::Initialize(int _x, int _y, int _w, int _h, std::string _fileName)
{
	x_ = _x;
	y_ = _y;
	w_ = _w;
	h_ = _h;

	hPict_ = Image::Load(_fileName.c_str());
	assert(hPict_ > 0);

	isPress_ = false;
}

void Button::Update()
{
	Input::GetMousePosition(mouseX, mouseY);

	XMFLOAT3 mousePos = { (float)mouseX,(float)mouseY ,0 };

	if (IsMouseInButton(mousePos))
	{
		//マウス左を押しているとき
		if ((Input::IsMouseButton(DIK_LEFT)))
		{
			//前のフレームでマウス左を押していないとき
			if (!prevLeftClick)
			{
				isPress_ = true;

				prevLeftClick = true;
			}
			else
			{
				isPress_ = false;
			}
		}
		else
		{
			prevLeftClick = false;
		}
	}
}

void Button::Draw()
{
	Transform fTrans;
	fTrans.position_ = { 0.0f,0.0f,0.0f };
	Image::SetTransform(hPict_, fTrans);
	Image::Draw(hPict_);
}

void Button::Release()
{
}

bool Button::IsMouseInButton(XMFLOAT3 _mousePos)
{
	if (   x_   <= _mousePos.x &&    y_   <= _mousePos.y &&
		x_ + w_ >= _mousePos.x && y_ + h_ >= _mousePos.y)
	{
		return true;
	}
	return false;
}
