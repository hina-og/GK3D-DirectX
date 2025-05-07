#pragma once
#include "Button.h"
#include "Puppet.h"

struct Storage
{
	int num;
	int type;
	int x, y;
	Button button;
	std::string name;
};

class PuppetStorage
	: public GameObject
{
	Storage puppetList_[CHARA_TYPE::CHARA_END];
public:
	PuppetStorage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void AddStorage(int _type);
};

