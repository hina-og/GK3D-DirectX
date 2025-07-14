#include "RangeView.h"

void RangeView::Initialize(XMFLOAT3 _position)
{
	//����{-1,-1}�E��{1,1}�̍��W�Ɉʒu��ϊ�����
	//_position = { _position.x / Direct3D::screenWidth_, _position.y / Direct3D::screenHeight_, 0 };


	for (int y = 0; y < MAX_TILE_Y; y++)
	{
		for (int x = 0; x < MAX_TILE_X; x++)
		{
			//������{0,0}�ɂ���
			tile[y][x].rangeData_ = { x - RANGE_CENTER.x,y - RANGE_CENTER.y };

			//�����̃^�C����ނ�ς��đ��̓f�t�H���g�ɂ���
			if (tile[y][x].rangeData_.x == 0 && tile[y][x].rangeData_.y == 0)
			{
				tile[y][x].type_ = ME;
			}
			else
			{
				tile[y][x].type_ = DEFAULT;
			}
			//�摜�ǂݍ���
			tile[y][x].hRangeTile_ = Image::Load("Image\\rangeTile.png");
			//�����`������c����������Ă���
			imageSize_ = Image::GetImageSize(tile[y][x].hRangeTile_).y;

			//����{-1,-1}�E��{1,1}�̍��W�Ɉʒu��ϊ����^�C������ׂ�
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
			Image::SetRect(tile[y][x].hRangeTile_, imageSize_ * tile[y][x].type_, 0, imageSize_, imageSize_);//�^�C���͐����`�̂��߃T�C�Y��y�Q��
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
				//�U���͈͂̃f�[�^�ƃ^�C�����W�������ꍇ�Ɏ�ނ�ATTACK�ɂ���i���̑�DEFAULT�j
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
	//�����͏��ME
	tile[RANGE_CENTER.y][RANGE_CENTER.x].type_ = ME;
}
