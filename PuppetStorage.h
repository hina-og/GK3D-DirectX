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
	//キャラの説明文の改行する文字数
	const int CHARA_TEXT_WIDTH_NUM{ 8 };

	//PuppetStorageData.csvの行
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

	//キャラのステータス表示の位置
	struct statusViewPosition
	{
		XMFLOAT3
			base,//ベース
			hp,//HP
			power,//攻撃力
			speed,//速度
			rangeView,//攻撃範囲
			text;//キャラの説明文
	};

	struct Storage
	{
		int num;//持っているキャラの数
		int type;//キャラの種類
		int x, y;//表示位置
		Button button;//ボタン
		std::string name;//キャラの名前
		Text numText;//持っているキャラの数のテキスト
		int textX, textY;//テキストの位置
		Puppet* puppet;//ステータス取得のためのポインタ
		bool isMade;//1つ以上作ったか
	};
	
	//ハンドル
	int hTable_;//後ろの木の板の画像
	int hSelectFrame_;//選択しているキャラの目印の画像
	int hStatusBase_;//ステータス表示のベースの画像

	Animation addAnim_;//キャラが追加された時のアニメーション

	//ステータス表示関係
	statusViewPosition viewPos_;//基準位置
	Text* hpText_;//HPのテキスト
	Text* powerText_;//攻撃力のテキスト
	Text* speedText_;//速度のテキスト
	RangeView* rangeView_;//攻撃範囲の見た目

public:
	Storage puppetList_[CHARA_TYPE::CHARA_END];//それぞれのキャラのストレージ
	int selectPuppetNumber;//選択しているキャラの番号

	PuppetStorage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	//画像読み込み
	void LoadImageData(CsvReader _csv);
	//ストレージにキャラを追加する
	bool AddStorage(int _type);
};

