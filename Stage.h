#pragma once
#include <vector>
#include "Engine/GameObject.h"


//�^�C���̍ő吔
const int WIDTH{ 7 }, HEIGHT{ 11 };
//�^�C���P�̏c�E���̕��i�����`�����瓯���j
const float TILE_SIZE{ 1.0f };

struct MAPCHIP
{
	int hTile_;//���f���̃n���h��
	int type_;//�^�C���̎�ށi���͎g���ĂȂ����ǂ�����X�e�[�W���ƂɃM�~�b�N��ǉ��ł������j
	XMFLOAT3 pos_;//�^�C���̈ʒu
	bool select_;//���̃^�C����I�����Ă��邩
	bool onPlayer_;//�v���C���[�L����������Ă��邩
};

class Stage
	: public GameObject
{
	MAPCHIP mapData_[HEIGHT][WIDTH];
	MAPCHIP beginData_;//�����i�ŏ��j�̃^�C��
	MAPCHIP endData_;//�E��i�Ō�j�̃^�C��

	//�n���h��
	int hGround_;//�n�ʂ̃��f��
	int hWall_;//�ǂ̃��f��
	int hTileSign_;//�I�����Ă���ꏊ���킩��₷��������

	bool isSelect_;//�ǂꂩ�I�����Ă��邩

	bool isZooming_;//�Y�[������
	int zoomdist_;//�Y�[�����鋗��

public:
	float spawnLine_;//�G���o�Ă���ꏊ
	float wallLine_;//�G���ǂɃ_���[�W��^����ꏊ

	Stage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	//����_num�̃^�C����I�����Ă��āA�v���C���[�L���������邩
	bool HasPlayer(XMFLOAT2 _num);
	//�I�����Ă���^�C���̈ʒu
	XMFLOAT3 SelectTilePosition(XMFLOAT2 _mousePos);
	//�I�����Ă���^�C���̃i���o�[
	XMFLOAT2 SelectTileNumber(XMFLOAT2 _mousePos);

	/// <summary>
	/// �^�C����I�����Ă��邩�̔��������
	/// </summary>
	/// <param name="_mousePos">�}�E�X�̈ʒu</param>
	/// <param name="_outNum">�I�����Ă���ꍇ�͂��̃^�C���̔ԍ�������</param>
	/// <param name="_outPos">�I�����Ă���ꍇ�͂��̃^�C���̈ʒu������</param>
	/// <returns>�^�C����I�����Ă��邩</returns>
	bool SelectTile(XMFLOAT2 _mousePos, XMFLOAT2& _outNum, XMFLOAT3& _outPos);

	//�v���C���[�L���������܂����Ԃɂ���
	void PlaceCharacter(XMFLOAT2 _num);

	/// <summary>
	/// �C�ӂ̃^�C���̈ʒu
	/// </summary>
	/// <param name="_x">�ԍ�x</param>
	/// <param name="_y">�ԍ�y</param>
	/// <returns></returns>
	XMFLOAT3 TilePos(int _x, int _y);
	//�Y�[�����Ă��邩
	bool IsZooming() { return isZooming_; }
};

