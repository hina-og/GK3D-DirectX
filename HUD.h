#pragma once
#include <string>
#include "Engine/GameObject.h"
#include "Engine/Text.h"

//チュートリアルのHP
const int TUTORIAL_HP{ 999 };

class HUD
	: public GameObject
{
	//csvの上から順番
	enum IMAGE_ROW
	{
		BASE_HP = 1,
		HP,
		BASE_TIME,
		TIME,
		DIRECTION,
	};

	int maxHP;//耐久の最大値

	int hHitPoint_;//残りのHP（緑のところ）
	int hBaseHitPoint_;//HPのベース画像
	Transform transformHP_;//HPの表示位置
	
	int hTimeBase_;//時間の後ろに表示するやつ
	Text* timeText;//時間のテキスト
	Transform transformTime_;//時間の後ろのやつの位置
	Transform transformTimeText_;//時間のテキストの位置

	int hModelDirection_;//キャラを設置するときの矢印
	Transform transformArrow_;//矢印の表示位置

	//チュートリアルか
	bool isTutorial;
public:
	int HP_;
	float time_;

	HUD(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	//チュートリアル用
	void TutorialInitialize();
	//HPの初期化
	void InitHP(int _hp);
	//体力に引数の値を足す（引く場合はマイナスの値を入れる）
	void AddHP(int _hp);
	void TimeUpdate();
	//時間を文字列に変換する
	std::string ToMinutesString();
	void SetDirection(int _dir);
};

