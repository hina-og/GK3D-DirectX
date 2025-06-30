#pragma once
#include "Button.h"
#include "Engine/CsvReader.h"
#include "Engine/Text.h"
#include "Animation.h"
#include "Puppet.h"
#include "RangeView.h"



class PuppetStorage
	: public GameObject
{
	const int CHARA_TEXT_WIDTH_NUM{ 8 };

	enum IMAGE_ROW
	{
		TABLE = 1,
		PUPPET,
		PUPPET_NUM,
		STATUS,
		HP,
		POWER,
		SPEED,
		RANGE_VIEW,
		PUPPET_TEXT,
	};

	struct statusViewPosition
	{
		XMFLOAT3
			base,
			hp,
			power,
			speed,
			rangeView,
			text;
	};

	struct Storage
	{
		int num;
		int type;
		int x, y;
		Button button;
		std::string name;
		Text numText;
		int textX;
		int textY;
		Puppet* puppet;
		bool isMade;//1Ç¬Ç≈Ç‡çÏÇ¡ÇΩÇ©
	};
	
	int hTable_;
	Animation addAnim_;

	int selectFrame_;
	int hStatusBase_;

	statusViewPosition viewPos_;

	Text* hpText_;
	Text* powerText_;
	Text* speedText_;

	RangeView* rangeView_;

public:
	Storage puppetList_[CHARA_TYPE::CHARA_END];
	int selectPuppetNumber;

	PuppetStorage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void LoadImageData(CsvReader _csv);
	bool AddStorage(int _type);
};

