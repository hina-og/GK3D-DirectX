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
	enum IMAGE_ROW
	{
		TABLE = 1,
		PUPPET,
		PUPPETNUM,
		STATUS,
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
	};
	
	int hTable_;
	Animation addAnim_;

	int selectFrame_;
	int hStatusBase_;

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

