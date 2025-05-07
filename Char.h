#pragma once
#include <string>

const int HEIGHT{ 6 };
const int WIDTH{ 16 };

namespace Char
{
	static int charPict;
	void Load();
	void Draw(int _x, int _y, std::string _str);
};

