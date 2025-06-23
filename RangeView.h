#pragma once
#include "Puppet.h"
#include <vector>
#include "Engine/Image.h"

//�\������^�C���̍ő吔
const int MAX_TILE_X{ 7 }, MAX_TILE_Y{ 7 };

class RangeView
{
	enum RangeType
	{
		ME = 0,
		DEFAULT,
		ATTACK,
		OTHER,
	};

	struct RangeTile
	{
		XMINT2 rangeData_;//�U���͈͂̃f�[�^
		int type_;//�^�C���̎��
		int hRangeTile_;//�^�C���̉摜�n���h��
	};

	RangeTile tile[MAX_TILE_Y][MAX_TILE_X];

	float imageSize;

public:
	void Initialize(XMFLOAT3 _position) {
		for (int y = 0;y < MAX_TILE_Y;y++)
		{
			for (int x = 0;x < MAX_TILE_X;x++)
			{
				tile[y][x].rangeData_ = { x - MAX_TILE_X / 2,y - MAX_TILE_Y / 2 };
				if (tile[y][x].rangeData_.x == 0 && tile[y][x].rangeData_.y == 0)
				{
					tile[y][x].type_ = ME;
				}
				else
				{
					tile[y][x].type_ = DEFAULT;
				}
				tile[y][x].hRangeTile_ = Image::Load("Image\\rangeTile.png");
				imageSize = Image::GetImageSize(tile[y][x].hRangeTile_).y;

				XMFLOAT3 tempPosition = { _position.x + imageSize * 2 * x / Direct3D::screenWidth_,_position.y + imageSize * 2 * y / Direct3D::screenHeight_,0 };
				Image::SetPosition(tile[y][x].hRangeTile_, tempPosition);

				
			}
		}
	}

	void Draw() {
		for (int y = 0;y < MAX_TILE_Y;y++)
		{
			for (int x = 0;x < MAX_TILE_X;x++)
			{
				Image::SetRect(tile[y][x].hRangeTile_, imageSize * tile[y][x].type_, 0, imageSize, imageSize);//�^�C���͐����`�̂��߃T�C�Y��y�Q��
				Image::Draw(tile[y][x].hRangeTile_);
			}
		}
	}
	void SetData(std::vector<XMINT2> _data) {

		for (int i = 0;i < _data.size();i++)
		{
			for (int y = 0;y < MAX_TILE_Y;y++)
			{
				for (int x = 0;x < MAX_TILE_X;x++)
				{
					//tile[y][x].type_ = DEFAULT;
					if (_data[i].x == tile[y][x].rangeData_.x &&
						_data[i].y == tile[y][x].rangeData_.y)
					{
						tile[y][x].type_ = ATTACK;
					}

				}
			}
		}
	}
};

