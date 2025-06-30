#pragma once
#include <string>
#include <map>
#include <vector>
#include "Engine/Image.h"

class Font
{
private:
    struct FontData
    {
        int handle;
        int cellWidth;
        int cellHeight;
        int rowLength;
        std::wstring characters;
        std::map<wchar_t, int> charToIndex;
    };

    static std::map<std::string, FontData> fonts_;

public:
    static void Initialize(const std::string& configFile);
    static void Draw(const std::string& _name, int _x, int _y, const std::wstring& _str);
    static void Draw(const std::string& _name, int _x, int _y, const std::wstring& _str, XMFLOAT3 _size, int _lineWrapLength);
    static void Draw(const std::string& _name, int _x, int _y, const std::string& _str, XMFLOAT3 _size, int _lineWrapLength);
    static void Release();
    static std::wstring UTF8toWString(const std::string& str);
};
