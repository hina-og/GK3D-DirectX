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
	//�ŏ��̃^�C���ƍŌ�̃^�C���̃f�[�^
	MAPCHIP beginData_, endData_;

	//�n���h��
	int hGround_;//�n�ʂ̃��f��
	int hWall_;//�ǂ̃��f��
	int hTileSign_;//�I�����Ă���ꏊ���킩��₷��������

	bool isSelect_;//�ǂꂩ�I�����Ă��邩

	bool isZooming_;//�Y�[������
	int zoomdist_;//�Y�[�����鋗��

public:
	float startLine_;//�G���o�Ă���ꏊ
	float endLine_;//�G���ǂɃ_���[�W��^����ꏊ

	Stage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	bool HasPlayer(XMFLOAT2 _num);
	XMFLOAT3 SelectTilePosition(XMFLOAT2 _pos);//�I�����Ă���^�C���̈ʒu
	XMFLOAT2 SelectTileNumber(XMFLOAT2 _pos);//�I�����Ă���^�C���̃i���o�[
	bool SelectTile(XMFLOAT2 _screenPos, XMFLOAT2& _outNum, XMFLOAT3& _outPos);
	void PlaceCharacter(XMFLOAT2 _num);//�v���C���[�L���������܂����Ԃɂ���
	bool IsZooming() { return isZooming_; }
};

