#include "RangeView.h"

void RangeView::Initialize(XMFLOAT3 _position)
{
	//左上{-1,-1}右下{1,1}の座標に位置を変換する
	//_position = { _position.x / Direct3D::screenWidth_, _position.y / Direct3D::screenHeight_, 0 };


	for (int y = 0; y < MAX_TILE_Y; y++)
	{
		for (int x = 0; x < MAX_TILE_X; x++)
		{
			//中央を{0,0}にする
			tile[y][x].rangeData_ = { x - RANGE_CENTER.x,y - RANGE_CENTER.y };

			//中央のタイル種類を変えて他はデフォルトにする
			if (tile[y][x].rangeData_.x == 0 && tile[y][x].rangeData_.y == 0)
			{
				tile[y][x].type_ = ME;
			}
			else
			{
				tile[y][x].type_ = DEFAULT;
			}
			//画像読み込み
			tile[y][x].hRangeTile_ = Image::Load("Image\\rangeTile.png");
			//正方形だから縦幅だけ取ってくる
			imageSize_ = Image::GetImageSize(tile[y][x].hRangeTile_).y;

			//左上{-1,-1}右下{1,1}の座標に位置を変換しつつタイルを並べる
			XMFLOAT3 tempPos = { 
				_position.x + imageSize_ * 2 * x,
				_position.y + imageSize_ * 2 * y,
				0 
			};
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
			for (int data = 0; data < _data.size(); data++)
			{
				//攻撃範囲のデータとタイル座標が同じ場合に種類をATTACKにする（その他DEFAULT）
				if (_data[data].x == tile[y][x].rangeData_.x &&
					_data[data].y == tile[y][x].rangeData_.y)
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
	//中央は常にME
	tile[RANGE_CENTER.y][RANGE_CENTER.x].type_ = ME;
}
