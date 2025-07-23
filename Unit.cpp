#include "Unit.h"
#include "Engine/Camera.h"
#include "Stage.h"
#include "PuppetFactory.h"

const float defaultShakePower{ 1.0f };//読み込めなかった場合のカメラが揺れる強さ
const float defaultShakeTime{ 100.0f };//読み込めなかった場合のカメラが揺れる時間（ミリ秒）

Unit::Unit()
{
}

Unit::Unit(GameObject* parent)
	: GameObject(parent, "Unit")
{
}

void Unit::Initialize()
{
	//カメラが揺れる強さの読み込み
	shakePower_ = GetPrivateProfileInt("Unit", "camerashake_power", defaultShakePower, ".\\config.ini");
	//カメラが揺れる時間の読み込み
	shakeTime_ = (float)GetPrivateProfileInt("Unit", "camerashake_time", defaultShakeTime, ".\\config.ini") / 1000;//ミリ秒から変換
}

void Unit::Update()
{
	for (int p = 0; p < puppet_.size(); p++)
	{
		//キャラが死んでいたら配列から削除
		if (puppet_[p]->IsDead())
		{
			puppet_.erase(puppet_.begin() + p);
		}
	}
}

void Unit::Draw()
{
}

void Unit::Release()
{
	//開放処理
	for (auto puppet : puppet_)
	{
		puppet->KillMe();
	}
	puppet_.clear();
}

void Unit::AddCharacter(XMFLOAT3 _pos, int _type, int _dir)
{
	//キャラの配列に追加
	puppet_.push_back(CreateInitPuppetByType(_type, this));

	//追加したキャラの位置と向きを設定
	puppet_.back()->SetPosition(_pos);
	puppet_.back()->SetDirection(_dir);
}

std::vector<Puppet*> Unit::GetPuppetArray()
{
	return puppet_;
}

void Unit::InvaderMove()
{
	for (auto puppet : puppet_)
	{
		//攻撃状態でなければ動く
		if (!puppet->isAttack_)
			puppet->Move(puppet->dir_);

		if (!puppet->isAlive_)
			puppet->KillMe();
	}
}

void Unit::InRange(std::vector<Puppet*> _puppet)
{
	for(auto myPuppet : puppet_)
	{
		myPuppet->inRangeChara_.clear();//攻撃範囲内にいるキャラの配列をきれいにする
		myPuppet->isAttack_ = false;//攻撃していない状態にする
		std::vector<XMINT2> attackTiles = myPuppet->GetAttackTiles();//攻撃範囲の配列

		for(auto enemy : _puppet)
		{
			//対象が既に死んでいたら次のループ
			if (!enemy->isAlive_)
				continue;

			XMFLOAT3 enemyPos = enemy->GetPosition();//このループで調べている敵の位置

			for(auto range : attackTiles)
			{
				//攻撃範囲の中心の位置
				XMFLOAT3 attackPos = {
					myPuppet->GetPosition().x + range.x,
					myPuppet->GetPosition().z + range.y,
					0
				};

				//攻撃範囲の位置と敵の位置の距離
				float distx = abs(attackPos.x - enemyPos.x),
					  disty = abs(attackPos.y - enemyPos.z);

				//敵が攻撃範囲内にいる場合
				if (distx < TILE_SIZE / 2 &&
					disty < TILE_SIZE / 2)
				{
					//攻撃している状態にして
					myPuppet->isAttack_ = true;
					//攻撃範囲内にいるキャラの配列に敵を追加
					myPuppet->inRangeChara_.push_back(enemy);
				}
			}

			if (myPuppet->isAttack_)
				break;
		}
	}
}

void Unit::PastLine(float _z, int& _durability)
{
	for (int p = 0; p < puppet_.size(); p++)
	{
		//生きていて壁に到達した場合
		if (puppet_[p]->GetPosition().z < _z && !puppet_[p]->IsDead())
		{
			_durability -= puppet_[p]->GetPower();//耐久値から攻撃力の値を引く
			puppet_[p]->KillMe();//到達したキャラは消す
			puppet_.erase(puppet_.begin() + p);//配列からも消す

			Camera::StartShake(shakePower_, shakeTime_);//カメラの振動開始
		}
	}
}

