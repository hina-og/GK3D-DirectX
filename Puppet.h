#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/Time.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Engine/Audio.h"
#include "Engine/VFX.h"

//キャラの種類
enum CHARA_TYPE
{
	MOUSE = 0,
	ZOMBIE,
	MUSHROOM,
	SLIME,
	GOLEM,
	GHOST,
	CHARA_END
};

//キャラの状態
enum CHARA_STATE
{
	STAND = 0,
	RUN,
	ATTACK,
	DEAD,
	STATE_END
};

class Puppet
	: public GameObject
{
	//音の種類
	enum SOUND_TYPE
	{
		ATTACK = 0,
		DEAD,
		SOUND_END
	};

	const float effectPosY = 0.5;//攻撃エフェクトが出る高さ（キャラの中心が0.5）
public:
	//向き
	enum DIRECTION
	{
		DOWN = 0,
		LEFT,
		UP,
		RIGHT
	};

	DIRECTION dir_;//キャラの向き

	std::vector<XMINT2> range_;//自分の位置が（0,0）の場合の攻撃範囲のマス
	std::vector<XMFLOAT3> rangePos_;//攻撃範囲の位置
	std::vector<EmitterData> particle_;//攻撃エフェクトの配列

	
	bool isAttack_;//攻撃しているか
	bool isAlive_;//生きているか

	std::vector<Puppet*> inRangeChara_;//攻撃範囲内にいるキャラの配列

	//キャラのHPを操作する関数（引数をマイナスにすれば減らせる）
	void ControlHP (int _addNum)
	{
		status_.hp_ += _addNum;
		//効果音再生
		Audio::Play(hAttackSE_);
	}

	//向きのセッター
	void SetDirection(int _dir)
	{
		dir_ = (Puppet::DIRECTION)_dir;

		//攻撃範囲も向きによって変える
		for (auto range : range_)
		{
			range = Rotate(range, dir_);
		}
	}

	//向きを設定
	void FacingDirection()
	{
		int directionAngle = 90;//向きは下から時計回りで90度ごと
		transform_.rotate_.y = dir_ * directionAngle;
	}

	//向いている方向に動く
	void Move(DIRECTION _dir)
	{
		
		if (hModel_ != modelList_[RUN])//モデルのアニメーションをRUNにする
		{
			hModel_ = modelList_[RUN];
			Model::SetAnimFrame(hModel_, 0, animData_.totalRunFrame_, animData_.runSpeed_);
		}
		else if (animData_.eRun_ <= Model::GetAnimFrame(hModel_))//RUNアニメーションが終わったら最初のフレームに戻る
		{
			Model::SetAnimFrame(hModel_, animData_.sRun_, animData_.eRun_, animData_.runSpeed_);
		}

		//位置を向いている方に動かす
		switch (dir_)
		{
		case Puppet::UP:
			transform_.position_.z += status_.speed_ * Time::GetDeltaTime();
			break;
		case Puppet::LEFT:
			transform_.position_.x -= status_.speed_ * Time::GetDeltaTime();
			break;
		case Puppet::DOWN:
			transform_.position_.z -= status_.speed_ * Time::GetDeltaTime();
			break;
		case Puppet::RIGHT:
			transform_.position_.x += status_.speed_ * Time::GetDeltaTime();
			break;
		default:
			break;
		}
	}

	//攻撃力のゲッター
	int GetPower() { return status_.power_; }
	//HPのゲッター
	int GetHitPoint() { return status_.hp_; }
	//速度のゲッター
	float GetSpeed() { return status_.speed_; }
	//キャラの説明文のゲッター
	std::string GetText() { return text_; }
	//攻撃範囲のゲッター
	std::vector<XMINT2> GetAttackTiles()
	{
		return range_;
	}

	//初期化
	virtual void Initialize() {};
	//更新
	void Update()
	{
		//向きの設定
		FacingDirection();


		//HPが残ってる
		if (status_.hp_ > 0)
		{
			//攻撃状態である
			if (isAttack_)
			{
				//攻撃アニメーションにする
				if (hModel_ != modelList_[CHARA_STATE::ATTACK])
				{
					hModel_ = modelList_[CHARA_STATE::ATTACK];
					Model::SetAnimFrame(hModel_, 0, animData_.totalAttackFrame_, animData_.attackSpeed_);
				}
				//攻撃するフレームになったら攻撃する
				if (Model::GetAnimFrame(hModel_) >= animData_.attack_ && !attacked_)
					Attack();
			}
			//攻撃状態じゃない＆走っていない場合はデフォルトのモデルにする
			else if (hModel_ != modelList_[CHARA_STATE::RUN])
			{
				hModel_ = modelList_[CHARA_STATE::STAND];
			}
		}

		//攻撃アニメーションが終わったら攻撃状態を解除
		if (hModel_ == modelList_[CHARA_STATE::ATTACK] && 
			Model::GetAnimFrame(hModel_) >= animData_.totalAttackFrame_)
		{
			isAttack_ = false;
			attacked_ = false;
		}

		for (int rangeNum = 0; rangeNum < range_.size(); rangeNum++)
		{
			//攻撃範囲の位置をキャラの位置を基準に更新する
			rangePos_[rangeNum] = { 
				transform_.position_.x + range_[rangeNum].x,
				effectPosY,
				transform_.position_.z + range_[rangeNum].y 
			};
		}

		Die();
	};
	//描画
	void Draw()
	{
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	};
	//開放
	void Release() {};
protected:


	struct AnimationData
	{
		int sRun_;//Runアニメーションのループ開始フレーム
		int eRun_;//Runアニメーションのループ終了フレーム
		int runSpeed_;//Runアニメーションの速度
		int totalRunFrame_;//Runアニメーションのフレーム数
		int attack_;//攻撃するフレーム
		int attackSpeed_;//Attackアニメーションの速度
		int totalAttackFrame_;//Runアニメーションのフレーム数
	};

	AnimationData animData_;//アニメーションのデータ
	int hModel_;//表示するモデルのハンドル
	int modelList_[CHARA_STATE::STATE_END];//モデルを変更しやすくするための配列
	bool attacked_;//アニメーションの1ループの中で既に攻撃したか
	int hAttackSE_;//攻撃時の効果音

	struct Status
	{
		//-----ステータス-----
		/*　あとでパラメータは読み込むようにする　*/
		std::string name_;
		int hp_;//体力
		int cost_;//召喚コスト
		int power_;//攻撃力
		float speed_;//移動速度
		//--------------------
	};
	Status status_;//キャラのステータス情報
	std::string text_;//キャラの説明文

	
	//ステータスの読み込み
	void LoadStatus(int _type)
	{
		CsvReader csv("GameData\\PuppetData.csv");

		for (int line = 0;line < csv.GetLines();line++)
		{
			//1行目説明のデータがあるから + 1
			if (csv.GetString(line, 0) != "name" &&
				_type + 1 == line)
			{
				SetStatus(csv, line);
			}
		}

		//------いろいろ初期状態にする------
		hModel_ = modelList_[STAND];
		Model::SetAnimFrame(hModel_, 0, animData_.totalRunFrame_, animData_.runSpeed_);

		dir_ = DIRECTION::UP;

		isAlive_ = true;
		isAttack_ = false;
		attacked_ = false;
		//----------------------------------

		//攻撃範囲のマスの数エフェクトを用意する
		for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
		{
			EmitterData data;
			particle_.push_back(data);
		}
	}

	//エフェクト情報の読み込み
	void LoadParticle(int _type)
	{
		CsvReader csv("Particle\\PuppetParticleData.csv");

		for (int line = 0;line < csv.GetLines();line++)
		{
			//1行目説明、2行目デフォルトのデータがあるから+2
			if (csv.GetString(line,0) != "name"   &&
				csv.GetString(line,0) != "default"&& 
				_type + 2 == line)
			{
				for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
				{
					particle_[rangeNum] = SetParticle(csv, line);;
				}
			}
		}
	}

	//ステータスのセッター
	void SetStatus(CsvReader _csv, int _line)
	{
		//csvに入っているデータ
		enum Read_Data
		{
			Name = 0,
			Text,
			Hp,
			Cost,
			Power,
			Speed,
			StartRunFrame,
			EndRunFrame,
			RunSpeed,
			TotalRunFrame,
			AttackFrame,
			AttackSpeed,
			TotalAttackFrame,
			AttackSE,
			Range,
		};

		status_.name_ = _csv.GetString(_line, Name);
		text_ = _csv.GetString(_line, Text);
		status_.hp_ = _csv.GetInt(_line, Hp);
		status_.cost_ = _csv.GetInt(_line, Cost);
		status_.power_ = _csv.GetInt(_line, Power);
		status_.speed_ = _csv.GetFloat(_line, Speed);
		animData_ = { 
			_csv.GetInt(_line, StartRunFrame), 
			_csv.GetInt(_line, EndRunFrame),
			_csv.GetInt(_line, RunSpeed),
			_csv.GetInt(_line, TotalRunFrame), 
			_csv.GetInt(_line, AttackFrame),
			_csv.GetInt(_line, AttackSpeed),
			_csv.GetInt(_line, TotalAttackFrame) 
		};

		hAttackSE_ = Audio::Load("Sounds\\SE\\" + _csv.GetString(_line, AttackSE) + ".wav",false,5);
		assert(hAttackSE_ >= 0);

		for (int column = Range;column < _csv.GetColumns(_line);column++)
		{
			XMFLOAT2 readRange;
			readRange = _csv.GetFloat2(_line, column);

			XMINT2 pushData;
			pushData = { (int)readRange.x,(int)readRange.y };

			if (range_.size() > 0)
			{
				if (range_.back().x == pushData.x && range_.back().y == pushData.y)
				{
					break;
				}
			}

			range_.push_back(pushData);
			rangePos_.push_back({ transform_.position_.x + pushData.x,effectPosY,transform_.position_.z + pushData.y });

		}

		modelList_[CHARA_STATE::STAND] = Model::Load("Model\\" + status_.name_ + "\\" + status_.name_ + ".fbx");
		assert(modelList_[CHARA_STATE::STAND] >= 0);
		modelList_[CHARA_STATE::RUN] = Model::Load("Model\\" + status_.name_ + "\\" + status_.name_ + "_Run.fbx");
		assert(modelList_[CHARA_STATE::RUN] >= 0);
		modelList_[CHARA_STATE::ATTACK] = Model::Load("Model\\" + status_.name_ + "\\" + status_.name_ + "_Attack.fbx");
		assert(modelList_[CHARA_STATE::ATTACK] >= 0);
	}

	//エフェクトのセッター
	EmitterData SetParticle(CsvReader _csv, int _line)
	{
		//csvに入っているデータ
		enum Read_Data
		{
			Name = 0,
			Folder,
			File,
			Position,
			PositionRand,
			Delay,
			Number,
			LifeTime,
			Gravity,
			Direction,
			DirectionRand,
			Speed,
			SpeedRand,
			Accel,
			Size,
			SizeRand,
			Scale,
			Color,
			DeltaColor,
			Spin,
			Rotate,
			RotateRand,
			IsBillBoard,
		};

		EmitterData data;
		data.textureFileName = "Particle\\" + _csv.GetString(_line, Folder) + "\\" + _csv.GetString(_line, File) + ".png";
		data.position = _csv.GetFloat3(_line, Position);
		data.positionRnd = _csv.GetFloat3(_line, PositionRand);
		data.delay = _csv.GetInt(_line, Delay);
		data.number = _csv.GetInt(_line, Number);
		data.lifeTime = _csv.GetInt(_line, LifeTime);
		data.gravity = _csv.GetFloat(_line, Gravity);
		data.direction = _csv.GetFloat3(_line, Direction);
		data.directionRnd = _csv.GetFloat3(_line, DirectionRand);
		data.speed = _csv.GetFloat(_line, Speed);
		data.speedRnd = _csv.GetFloat(_line, SpeedRand);
		data.accel = _csv.GetFloat(_line, Accel);
		data.size = _csv.GetFloat2(_line, Size);
		data.sizeRnd = _csv.GetFloat2(_line, SizeRand);
		data.scale = _csv.GetFloat2(_line, Scale);
		data.color = _csv.GetFloat4(_line, Color);
		data.deltaColor = _csv.GetFloat4(_line, DeltaColor);
		data.spin = _csv.GetFloat3(_line, Spin);
		data.rotate = _csv.GetFloat3(_line, Rotate);
		data.rotateRnd = _csv.GetFloat3(_line, RotateRand);
		data.isBillBoard = _csv.GetInt(_line, IsBillBoard);


		return data;
	}

	//攻撃範囲のマスを向きによって変える関数
	XMINT2 Rotate(XMINT2 _pos, int _dir)
	{
		switch (_dir)
		{
		case DIRECTION::UP:
			return { _pos.x,  _pos.y };
		case DIRECTION::RIGHT:
			return { _pos.y,  -_pos.x };
		case DIRECTION::DOWN:
			return { -_pos.x, -_pos.y };
		case DIRECTION::LEFT:
			return { -_pos.y, _pos.x };
		}
	}

	//死んでいるときの処理
	void Die() 
	{
		//HPがなくてisAliveがtrueのときに
		if (status_.hp_ < 1 && isAlive_)
		{
			//エフェクトを出す
			EmitterData deadParticle_;
			CsvReader csv("Particle\\PuppetParticleData.csv");
			deadParticle_ = SetParticle(csv, CHARA_TYPE::CHARA_END + 2);
			deadParticle_.position = transform_.position_;
			VFX::Start(deadParticle_);

			//生きていない状態にする
			isAlive_ = false;

			//モデルをゴミ山にする
			hModel_ = Model::Load("Model\\Garbage.fbx");
			assert(hModel_ >= 0);
		}
		
	}


	//キャラごとに変えられるようにしている（バフ・デバフなど）
	virtual void Attack() {};

};

