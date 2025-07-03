#pragma once
#include "Puppet.h"
#include <vector>
#include "Engine/Image.h"

//�\������^�C���̍ő吔
const int MAX_TILE_X{ 7 }, MAX_TILE_Y{ 7 };

//�U���͈͂̕\��
class RangeView
{
	//����������}�X�̈ʒu�i�����j
	const XMINT2 RANGE_CENTER{ MAX_TILE_X / 2, MAX_TILE_Y / 2 };

	enum RangeType
	{
		ME = 0,//����������}�X
		DEFAULT,//�����Ȃ��}�X
		ATTACK,//�U������}�X
		OTHER,//���̑��i���͎g���ĂȂ����ǃf�o�t�}�X�Ƃ��j
	};

	struct RangeTile
	{
		XMINT2 rangeData_;//�^�C���f�[�^
		int type_;//�^�C���̎��
		int hRangeTile_;//�^�C���̉摜�n���h��
	};

	RangeTile tile[MAX_TILE_Y][MAX_TILE_X];

	//�����`�̂���float
	float imageSize_;

public:
	//������
	void Initialize(XMFLOAT3 _position);
	//�`��
	void Draw();
	/// <summary>
	/// �U���͈̓f�[�^���^�C���ɂ���
	/// </summary>
	/// <param name="_data">�U���͈͂̃f�[�^</param>
	void SetData(std::vector<XMINT2> _data);
};

