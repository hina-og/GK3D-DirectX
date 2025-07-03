#include "Font.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <windows.h>

std::map<std::string, Font::FontData> Font::fonts_;



std::wstring Font::UTF8toWString(const std::string& str)
{
    if (str.empty())
    {
        return std::wstring();
    }

    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring result(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], sizeNeeded);
    result.pop_back();//nullï∂éöÇçÌèú
    return result;
}

void Font::Initialize(const std::string& configFile)
{
    std::ifstream ifs(configFile);
    if (!ifs)
    {
        std::cout << "FontConfig file open failed: " << configFile << std::endl;
        return;
    }


    std::string line;
    std::string currentFontName;
    FontData data;


    while (std::getline(ifs, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        if (line.find('=') == std::string::npos)
        {
            if (!currentFontName.empty())
            {
                fonts_[currentFontName] = data;
            }

            currentFontName = line;
            data = FontData();
        }
        else
        {
            auto pos = line.find('=');
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            if (key == "file")
            {
                data.handle = Image::Load(value.c_str());
            }
            else if (key == "type")
            {
                //âΩÇ‡Ç‚Ç¡ÇƒÇ»Ç¢
            }
            else if (key == "start")
            {
                wchar_t start = static_cast<wchar_t>(std::stoi(value.substr(2), nullptr, 16));
                data.characters += start;
            }
            else if (key == "end")
            {
                wchar_t start = data.characters[0];
                wchar_t end = static_cast<wchar_t>(std::stoi(value.substr(2), nullptr, 16));
                data.characters = L"";


                for (wchar_t c = start; c <= end; c++)
                {
                    data.characters += c;
                }
                for (size_t i = 0; i < data.characters.size(); ++i)
                {
                    data.charToIndex[data.characters[i]] = static_cast<int>(i);
                }
            }
            else if (key == "cellWidth")
            {
                data.cellWidth = std::stoi(value);
            }
            else if (key == "cellHeight")
            {
                data.cellHeight = std::stoi(value);
            }
            else if (key == "rowLength")
            {
                data.rowLength = std::stoi(value);
            }
        }
    }

    if (!currentFontName.empty())
    {
        fonts_[currentFontName] = data;
    }
}

void Font::Draw(const std::string& _name, int _x, int _y, const std::wstring& _str)
{
    if (fonts_.count(_name) == 0)
    {
        return;
    }

    const FontData& data = fonts_[_name];

    float px = static_cast<float>(_x);
    float py = static_cast<float>(_y);

    float lineSpacing = 0.2;

    for (auto ch : _str)
    {
        auto it = data.charToIndex.find(ch);
        if (it != data.charToIndex.end())
        {
            int index = it->second;
            int col = index % data.rowLength;
            int row = index / data.rowLength;

            Transform transform;
            transform.position_ = { px, py, 0 };
            Image::SetTransform(data.handle, transform);
            Image::SetRect(data.handle,
                col * data.cellWidth,
                row * data.cellHeight,
                data.cellWidth, data.cellHeight);

            Image::Draw(data.handle);

            px += Image::GetImageSize(data.handle).x * lineSpacing;
        }
        else
        {
            float spaceSize = 0.4;
            //ñ¢ìoò^ï∂éö -> ÉXÉyÅ[ÉXëäìñ
            px += Image::GetImageSize(data.handle).x * lineSpacing * spaceSize;
        }
    }
}

void Font::Draw(const std::string& _name, int _x, int _y, const std::wstring& _str, XMFLOAT3 _size, int _lineWrapLength) 
{
    if (fonts_.count(_name) == 0)
    {
        return;
    }

    const FontData& data = fonts_[_name];

    float px = static_cast<float>(_x);
    float py = static_cast<float>(_y);

    int charCount = 0;

    float lineSpacing = 0.2;

    for (auto ch : _str)
    {
        auto it = data.charToIndex.find(ch);

        if (it != data.charToIndex.end())
        {
            int index = it->second;
            int col = index % data.rowLength;
            int row = index / data.rowLength;

            Transform transform;
            transform.position_ = { px, py, 0 };
            transform.scale_ = _size;
            Image::SetTransform(data.handle, transform);
            Image::SetRect(data.handle,
                col * data.cellWidth,
                row * data.cellHeight,
                data.cellWidth, data.cellHeight);

            Image::Draw(data.handle);

            px += Image::GetImageSize(data.handle).x * lineSpacing;
            if (charCount >= _lineWrapLength)
            {
                px = static_cast<float>(_x);
                py -= Image::GetImageSize(data.handle).y * lineSpacing;
                charCount = 0;
            }
            charCount++;
        }
        else
        {
            float spaceSize = 0.4;
            //ñ¢ìoò^ï∂éö -> ÉXÉyÅ[ÉXëäìñ
            px += Image::GetImageSize(data.handle).x * lineSpacing * spaceSize;
            if (charCount >= _lineWrapLength)
            {
                px = static_cast<float>(_x);
                py -= Image::GetImageSize(data.handle).y * lineSpacing;
                charCount = 0;
            }
            charCount++;
        }
    }
}

void Font::Draw(const std::string& _name, int _x, int _y, const std::string& _str, XMFLOAT3 _size, int _lineWrapLength)
{
    int size_needed = MultiByteToWideChar(932, 0, _str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed - 1, 0);
    MultiByteToWideChar(932, 0, _str.c_str(), -1, &wstr[0], size_needed);

    if (fonts_.count(_name) == 0)
    {
        return;
    }

    const FontData& data = fonts_[_name];

    float px = static_cast<float>(_x);
    float py = static_cast<float>(_y);

    int charCount = 0;

    float lineSpacing = 0.2;

    for (auto ch : wstr)
    {
        auto it = data.charToIndex.find(ch);
        if (it != data.charToIndex.end())
        {
            int index = it->second;
            int col = index % data.rowLength;
            int row = index / data.rowLength;

            Transform transform;
            transform.position_ = { px, py, 0 };
            transform.scale_ = _size;
            Image::SetTransform(data.handle, transform);
            Image::SetRect(data.handle,
                col * data.cellWidth,
                row * data.cellHeight,
                data.cellWidth, data.cellHeight);

            Image::Draw(data.handle);

            px += Image::GetImageSize(data.handle).x * lineSpacing;
            if (charCount >= _lineWrapLength)
            {
                px = static_cast<float>(_x);
                py -= Image::GetImageSize(data.handle).y * lineSpacing;
                charCount = 0;
            }
            charCount++;
        }
        else
        {
            float spaceSize = 0.4;
            //ñ¢ìoò^ï∂éö Å® ÉXÉyÅ[ÉXëäìñ
            px += Image::GetImageSize(data.handle).x * lineSpacing * spaceSize;
            if (charCount >= _lineWrapLength)
            {
                px = static_cast<float>(_x);
                py -= Image::GetImageSize(data.handle).y * lineSpacing;
                charCount = 0;
            }
            charCount++;
        }
    }
}

void Font::Release()
{
    for (auto& pair : fonts_)
    {
        Image::Release(pair.second.handle);
    }
    fonts_.clear();
}
