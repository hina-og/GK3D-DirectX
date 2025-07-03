#pragma once
#include <string>
#include <vector>
#include "Engine/CsvReader.h"

//�摜�̃f�[�^
struct ImageData 
{
    std::string name;
    float positionX;
    float positionY;
    float scaleX;
    float scaleY;
};

//�摜�f�[�^�������Ă���csv�̃J����
enum ImageCsvColumn 
{
    NAME = 0,
    POSITION_X,
    POSITION_Y,
    SCALE_X,
    SCALE_Y
};

//csv�̉摜�f�[�^���܂Ƃ߂ďo��
inline std::vector<ImageData> LoadImageData(const std::string& csvPath) 
{
    std::vector<ImageData> imageList;
    CsvReader csv(csvPath);

    for (int i = 0; i < csv.GetLines(); ++i) {
        ImageData data;
        data.name = csv.GetString(i, NAME);
        data.positionX = csv.GetFloat(i, POSITION_X);
        data.positionY = csv.GetFloat(i, POSITION_Y);
        data.scaleX = csv.GetFloat(i, SCALE_X);
        data.scaleY = csv.GetFloat(i, SCALE_Y);

        imageList.push_back(data);
    }

    return imageList;
}
