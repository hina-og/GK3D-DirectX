#pragma once
#include "Button.h"
#include "Puppet.h"
#include "Engine/CsvReader.h"
#include "Engine/Text.h"

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
};

class PuppetStorage
	: public GameObject
{
	
	int hTable_;

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

