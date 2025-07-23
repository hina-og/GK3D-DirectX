#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"
#include "Mushroom.h"
#include "Slime.h"
#include "Golem.h"
#include "Ghost.h"

//プレイヤーや敵に追加するクラス
class Unit
	: public GameObject
{
	//キャラの配列
	std::vector<Puppet*> puppet_;
	//カメラが揺れる強さ
	int shakePower_;
	//カメラが揺れる時間（ミリ秒）
	float shakeTime_;
public:
	Unit();
	Unit(GameObject* parent);
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//開放
	void Release();

	/// <summary>
	/// キャラの追加
	/// </summary>
	/// <param name="_pos">キャラの位置</param>
	/// <param name="_type">キャラの種類</param>
	/// <param name="_dir">キャラの向き</param>
	void AddCharacter(XMFLOAT3 _pos, int _type, int _dir);
	//キャラの配列のゲッター
	std::vector<Puppet*> GetPuppetArray();
	//上から手前に侵略してくる動き
	void InvaderMove();
	/// <summary>
	/// 攻撃範囲に入ったときの処理
	/// </summary>
	/// <param name="_puppet">攻撃対象のキャラの配列</param>
	void InRange(std::vector<Puppet*> _puppet);
	/// <summary>
	/// 壁まで到達したときの処理
	/// </summary>
	/// <param name="_z">壁のZ座標</param>
	/// <param name="_durability">壁の耐久値</param>
	void PastLine(float _z, int& _durability);
};

