#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Button.h"
#include "Puppet.h"
#include <vector>
#include "PuppetStorage.h"
#include "Animation.h"
#include "Recipe.h"

//�f�ނ�I���ł���ő吔
const int TABLE_SIZE{ 4 };

//�ێ��ł���f�ނ̍ő吔
const int MAX_MATERIAL_NUM{ 99 };

//�I�������f�ނ���ׂ���
struct Table
{
	Material material[TABLE_SIZE];//�f�ނ�u���g�݂����Ȃ��
	int x, y;//�\���ʒu
	int num;//���ׂ��Ă���f�ނ̐�
};

class MaterialTable
	: public GameObject
{
	

	//MaterialTableData�̍s
	enum IMAGE_ROW
	{
		TABLE = 1,
		MATERIAL,
		MATERIAL_NUM,
		MATERIAL_TABLE,
		MAKE,
		QUICK_RECIPE,
		POT,
	};

	std::vector<std::string> materialName_;//�f�ނ̖��O

	//�摜�n���h��
	int hTable_;//���̖؂̔�
	int hPot_;//�劘

	//���n���h��
	const int selectSoundNum{ 3 };//hSelect�𓯎��ɂȂ炷�ő吔
	int hSelect_;//�쐬��
	int hChoise_;//�I����

	Material materialList_[MATERIAL_TYPE::MATERIAL_END];//�f�ނ̔z��
	Table table;//�I�������f�ނ���ׂ���
	PuppetStorage* storage;//�L�����̃X�g���[�W

	Button makeButton_;//���{�^��
	Animation addAnim_;//�f�ނ��������Ƃ��̃A�j���[�V����
	Animation steamAnim_;//������Ƃ��̃{���b�Ă���A�j���[�V����

	std::vector<Recipe> recipeList_;//���V�s���X�g

	int returnProbability_;//�쐬���s�����Ƃ��ɑf�ނ��߂��Ă���m��

	QuickRecipe* quickRecipe;//��������V�s�̕ۑ�

	void InitFrameTable(int _row, CsvReader _csv);
	void InitMaterialList(int _row, CsvReader _csv);
	void InitMakeButton(int _row, CsvReader _csv);
	void InitSlotFrames(int _row, CsvReader _csv);

	bool isTutorial;

public:
	MaterialTable(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void TutorialInitialize();

	//���V�s�̓ǂݍ���
	void ReadRecipe();
	//�L���������
	int MakePuppet();
	//���ׂĂ���f�ނ����Z�b�g����i�쐬�Ɏ��s��������m���őf�ނ�߂��j
	void TableReset();
	//�I�����Ă���L�����̔ԍ���Ԃ�+���̃L�����̐������炷
	int GetSelectStragePuppet();
	//�I�����Ă���L������1�̈ȏア�邩
	bool isNotEmpty();
	//�����_���őf�ނ��z����
	void GetRandomMaterial();
	//_num�̐������ŏ��ɑf�ނ��^������
	void GiveMaterial(int _num);
};