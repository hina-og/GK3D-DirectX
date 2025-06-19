#include "File.h"
#include <filesystem>
#include <algorithm>
#include <vector>
#include <windows.h>

namespace File
{
	int GetFileNum(std::string _path)
	{
		int fileCount = 0;
		try {
			std::filesystem::directory_iterator it(_path);
			std::filesystem::directory_iterator end;

			for (; it != end; ++it) {
				++fileCount;
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			char message[256];
			wsprintf(message, "�u%s�v��������܂���", _path.c_str());
			MessageBox(0, message, "�t�H���_�̓ǂݍ��݂Ɏ��s", MB_OK);
		}

		return fileCount;
	}

	std::string GetFileName(const std::string _path, int _num)
	{
		std::vector<std::filesystem::directory_entry> entries;

		try {
			for (const auto& entry : std::filesystem::directory_iterator(_path)) {
				if (std::filesystem::is_regular_file(entry.status())) {
					entries.push_back(entry);
				}
			}

			std::sort(entries.begin(), entries.end(),
				[](const auto& a, const auto& b) {
					return a.path().filename() < b.path().filename();
				});

			if (entries.size() >= static_cast<size_t>(_num) && _num > 0) {
				return _path + "\\" + entries[_num - 1].path().filename().string();
			}
			else {
				char message[256];
				wsprintf(message, "�t�@�C����%d�����ł�", _num);
				MessageBox(0, message, "�t�H���_�̓ǂݍ��݂Ɏ��s", MB_OK);
				return "";
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			char message[256];
			wsprintf(message, "�u%s�v��������܂���", _path.c_str());
			MessageBox(0, message, "�t�H���_�̓ǂݍ��݂Ɏ��s", MB_OK);
			return "";
		}
	}
}
