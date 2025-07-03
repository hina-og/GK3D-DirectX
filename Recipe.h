#pragma once
#include <vector>
#include "Button.h"
#include "Engine/Text.h"
#include "Engine/CsvReader.h"
#include <map>

//�f�ނ̎��
enum MATERIAL_TYPE
{
	BONE = 0,
	MEAT,
	SOUL,
	BACTERIA,
	ROCK,
	BRAIN,
	WATER,
	MATERIAL_END,
	EMPTY = -1,
};

//�f�ނ̖��O���ԍ��o��
inline int GetMaterialTypeFromName(const std::string& name)
{
	static const std::map<std::string, int> map = {
		{"Bone", MATERIAL_TYPE::BONE},
		{"Meat", MATERIAL_TYPE::MEAT},
		{"Soul", MATERIAL_TYPE::SOUL},
		{"Bacteria", MATERIAL_TYPE::BACTERIA},
		{"Rock", MATERIAL_TYPE::ROCK},
		{"Brain", MATERIAL_TYPE::BRAIN},
		{"Water", MATERIAL_TYPE::WATER}
	};
	auto it = map.find(name);
	return (it != map.end()) ? it->second : -1;
}

//�f�ނ̖��O���ԍ��o�́i�摜��ǂݍ��ނƂ��Ƃ��g���j
inline std::string GetMaterialNameFromType(int type)
{
	static const std::string names[] = {
		"Bone",
		"Meat",
		"Soul",
		"Bacteria",
		"Rock",
		"Brain",
		"Water"
	};
	if (type >= 0 && type < MATERIAL_TYPE::MATERIAL_END)
		return names[type];
	return "Unknown";
}


struct Material
{
	int type;//���
	//std::vector<int> tag_;//�f�ނ̑����Ƃ����߂���
	XMFLOAT2 buttonPosition;//�{�^���̈ʒu(x,y)
	Button button;//�f�ރ{�^��
	std::string name;//���O
	int num;//�����Ă��鐔
	Text numText;//�����Ă��鐔�̃e�L�X�g�\��
	XMFLOAT2 textPosition;
};

class Recipe
{
public:
	std::vector<int> ratio;//�f�ނ̊���
	int puppetType;//����L����
};

//�ۑ����郌�V�s�̐�
const int saveNum{ 1 };

//�N�C�b�N���V�s�i�����Ⴒ���Ⴕ������������������j
class QuickRecipe
	: public GameObject
{
	struct QuickRecipeButton
	{
		Button saveRecipeBtn;//�x�[�X�̃{�^��
		int charaIcon;//�L�����摜
		std::vector<int>recipeIcon;//�f�މ摜
		std::vector<int>material;//�f��
		bool inData;//�f�[�^�������Ă��邩
	};

	QuickRecipeButton qButton[saveNum];

	XMFLOAT3 position_;
public:

	QuickRecipe(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	//csv�ňʒu�Ɖ摜��ǂݍ��ށi_row�F�s�j
	void SetPosition(int _row, CsvReader _csv);
	//�{�^���T�C�Y
	XMFLOAT3 GetSize();
	/// <summary>
	/// �N�C�b�N���V�s�ǉ��i����saveNum�𒴂���ƍŏ��ɓ��ꂽ���̂�������Ă����j
	/// </summary>
	/// <param name="_charaType">����L�����̎��</param>
	/// <param name="_recipe">�I�������f�ނ̔z��</param>
	void AddRecipe(int _charaType,std::vector<int> _recipe);
};
