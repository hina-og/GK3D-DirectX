#include "Button.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

Button::Button()
{
}

void Button::Initialize(int _x, int _y, std::string _fileName)
{
	x_ = _x;
	y_ = -_y;

	hPict_ = Image::Load(_fileName.c_str());
	assert(hPict_ >= 0);

	XMFLOAT3 size = Image::GetImageSize(hPict_);

	size_.w_ = size.x * 2;
	size_.h_ = size.y * 2;

	isPress_ = false;
}

void Button::Update()
{
	isPress_ = false;

	Input::GetMousePosition(mouseX, mouseY);

	XMFLOAT3 mousePos = { (float)mouseX * 2 - 1280,(float)-(mouseY * 2 - 720) ,0 };

	if (IsMouseInButton(mousePos))
	{
		//É}ÉEÉXç∂ÇâüÇµÇƒÇ¢ÇÈÇ∆Ç´
		if ((Input::IsMouseButtonDown(LEFT_CLICK)))
		{
			isPress_ = true;
		}
	}
}

void Button::Draw()
{
	Transform fTrans;
	fTrans.position_ = { (float)x_,(float)y_,0.0f };
	Image::SetTransform(hPict_, fTrans);
	Image::Draw(hPict_);
}

void Button::Release()
{
}

void Button::SetPosition(XMFLOAT3 _pos)
{
	x_ = _pos.x;
	y_ = _pos.y;
}

XMFLOAT3 Button::GetSize()
{
	XMFLOAT3 returnSize = { (float)size_.w_,(float)size_.h_,0 };
	return returnSize;
}

bool Button::IsMouseInButton(XMFLOAT3 _mousePos)
{
	if (x_ - size_.w_ / 2 <= _mousePos.x && y_ - size_.h_ / 2 <= _mousePos.y &&
		x_ + size_.w_ / 2 >= _mousePos.x && y_ + size_.h_ / 2 >= _mousePos.y)
	{
		return true;
	}
	return false;
}
