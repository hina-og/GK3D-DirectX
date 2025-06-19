#pragma once
#include <string>

namespace File
{
	int GetFileNum(std::string _path);
	std::string GetFileName(const std::string _path, int _num);
}
