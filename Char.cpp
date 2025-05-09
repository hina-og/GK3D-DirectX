#include "Char.h"
#include "Engine/GameObject.h"
#include "Engine/Image.h"

void Char::Load()
{
	charPict = Image::Load("Image\\char.png");
}

void Char::Draw(int _x, int _y, std::string _str)
{
	for (int i = 0;i < _str.size();i++)
	{
		char c = _str[i];
		int x = c % 16;
		int y = c / 16 - 2;

		Transform fTrans;
		fTrans.position_ = { (float)_x + i * 16 ,(float)_y ,0.0f };

		Image::SetTransform(charPict, fTrans);
		Image::SetRect(charPict, x * 16, y * 32, 16, 32);
		Image::Draw(charPict);
	}
}
