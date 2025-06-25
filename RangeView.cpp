#include "RangeView.h"

void RangeView::Initialize(XMFLOAT3 _position)
{
	_position = { _position.x / Direct3D::screenWidth_, _position.y / ::Direct3D::screenHeight_, 0 };
	for (int y = 0; y < MAX_TILE_Y; y++)
	{
		for (int x = 0; x < MAX_TILE_X; x++)
		{
			tile[y][x].rangeData_ = { x - RANGE_CENTER.x,y - RANGE_CENTER.y };
			if (tile[y][x].rangeData_.x == 0 && tile[y][x].rangeData_.y == 0)
			{
				tile[y][x].type_ = ME;
			}
			else
			{
				tile[y][x].type_ = DEFAULT;
			}
			tile[y][x].hRangeTile_ = Image::Load("Image\\rangeTile.png");
			imageSize_ = Image::GetImageSize(tile[y][x].hRangeTile_).y;

			XMFLOAT3 tempPos = { _position.x + imageSize_ * 2 * x / Direct3D::screenWidth_,_position.y + imageSize_ * 2 * y / Direct3D::screenHeight_,0 };
			Image::SetPosition(tile[y][x].hRangeTile_, tempPos);


		}
	}
}

void RangeView::Draw()
{
	for (int y = 0; y < MAX_TILE_Y; y++)
	{
		for (int x = 0; x < MAX_TILE_X; x++)
		{
			Image::SetRect(tile[y][x].hRangeTile_, imageSize_ * tile[y][x].type_, 0, imageSize_, imageSize_);//タイルは正方形のためサイズはy参照
			Image::Draw(tile[y][x].hRangeTile_);
		}
	}
	}

void RangeView::SetData(std::vector<XMINT2> _data)
{


	for (int y = 0; y < MAX_TILE_Y; y++)
	{
		for (int x = 0; x < MAX_TILE_X; x++)
		{
			for (int i = 0; i < _data.size(); i++)
			{
				//tile[y][x].type_ = DEFAULT;
				if (_data[i].x == tile[y][x].rangeData_.x &&
					_data[i].y == tile[y][x].rangeData_.y)
				{
					tile[y][x].type_ = ATTACK;
					break;
				}
				else
				{
					tile[y][x].type_ = DEFAULT;
				}


			}
		}
	}
	tile[RANGE_CENTER.y][RANGE_CENTER.x].type_ = ME;
}
