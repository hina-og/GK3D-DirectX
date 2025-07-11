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
		END_TEXT,
		MENU_TEXT,
	};

	int maxHP_;//耐久の最大値

	int hHitPoint_;//残りのHP（緑のところ）
	int hBaseHitPoint_;//HPのベース画像
	Transform transformHP_;//HPの表示位置
	
	int hTimeBase_;//時間の後ろに表示する画像
	Text* timeText_;//時間のテキスト
	Transform transformTime_;//時間の後ろの画像の位置
	Transform transformTimeText_;//時間のテキストの位置

	int hModelDirection_;//キャラを設置するときの矢印
	Transform transformArrow_;//矢印の表示位置

	/*チュートリアル関係*/
	bool isTutorial_;//チュートリアルか
	Text* endText_;//チュートリアル終了操作テキスト
	XMFLOAT3 endTextPos_;//チュートリアル終了操作の位置
	Text* resetText_;//リセット操作のテキスト
	XMFLOAT3 resetTextPos_;//リセット操作の位置
public:
	int HP_;//体力
	float time_;//時間

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

