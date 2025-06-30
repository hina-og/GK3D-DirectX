#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/Time.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Engine/Audio.h"
#include "Engine/VFX.h"

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
	enum SOUND_TYPE
	{
		ATTACK = 0,
		DEAD,
		END
	};

	const float effectPosY = 0.5;
public:

	enum DIRECTION
	{
		DOWN = 0,
		LEFT,
		UP,
		RIGHT
	};

	DIRECTION dir_;

	std::vector<XMINT2> range_;
	std::vector<XMFLOAT3> rangePos_;
	std::vector<EmitterData> particle_;


	
	bool isAttack_;
	bool isAlive_;//生きているか

	std::vector<Puppet*> inRangeChara_;

	void ControlHP (int _addNum)
	{
		status_.hp_ += _addNum;
		Audio::Play(hAttackSE_);
	}

	std::vector<XMINT2> GetAttackTiles()
	{
		return range_;
	}

	void SetDirection(DIRECTION _dir)
	{
		dir_ = _dir;

		for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
		{
			range_[rangeNum] = rotate(range_[rangeNum], dir_);
		}
	}

	void SetDirection(int _dir)
	{
		switch (_dir)
		{
		case 0:
			dir_ = DOWN;
			break;
		case 1:
			dir_ = LEFT;
			break;
		case 2:
			dir_ = UP;
			break;
		case 3:
			dir_ = RIGHT;
			break;
		default:
			break;
		}
		for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
		{
			range_[rangeNum] = rotate(range_[rangeNum], dir_);
		}
	}

	void FacingDirection()
	{
		transform_.rotate_.y = dir_ * 90;
	}

	void Move(DIRECTION _dir)
	{
		if (hModel_ != modelList_[RUN])
		{
			hModel_ = modelList_[RUN];
			Model::SetAnimFrame(hModel_, 0, animData_.totalRunFrame_, animData_.runSpeed_);
		}
		else if (animData_.eRun_ <= Model::GetAnimFrame(hModel_))
		{
			Model::SetAnimFrame(hModel_, animData_.sRun_, animData_.eRun_, animData_.runSpeed_);
		}
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

	int CharacterType(std::string _type)
	{
		if (_type == "Mouse") return CHARA_TYPE::MOUSE;
		if (_type == "Zombie") return CHARA_TYPE::ZOMBIE;
		if (_type == "Mushrom") return CHARA_TYPE::MUSHROOM;
		if (_type == "Slime") return CHARA_TYPE::SLIME;
		if (_type == "Golem") return CHARA_TYPE::GOLEM;
		if (_type == "Ghost") return CHARA_TYPE::GHOST;
	}

	int GetPower() { return status_.power_; }
	int GetHitPoint() { return status_.hp_; }
	float GetSpeed() { return status_.speed_; }
	std::string GetText() { return text_; }


	virtual void Initialize() {};
	void Update()
	{
		FacingDirection();


		if (status_.hp_ > 0)
		{
			if (isAttack_)
			{
				if (hModel_ != modelList_[CHARA_STATE::ATTACK])
				{
					hModel_ = modelList_[CHARA_STATE::ATTACK];
					Model::SetAnimFrame(hModel_, 0, animData_.totalAttackFrame_, animData_.attackSpeed_);
				}
				if (Model::GetAnimFrame(hModel_) >= animData_.attack_ && !attacked_)
					Attack();
			}
			else if (hModel_ != modelList_[CHARA_STATE::RUN])
			{
				hModel_ = modelList_[CHARA_STATE::STAND];
			}
		}


		if (hModel_ == modelList_[CHARA_STATE::ATTACK] && Model::GetAnimFrame(hModel_) >= animData_.totalAttackFrame_)
		{
			isAttack_ = false;
			attacked_ = false;
		}

		for (int rangeNum = 0; rangeNum < range_.size(); rangeNum++)
		{
			rangePos_[rangeNum] = { transform_.position_.x + range_[rangeNum].x,effectPosY,transform_.position_.z + range_[rangeNum].y };
		}

		Die();
	};
	void Draw()
	{
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	};
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

	AnimationData animData_;
	int hModel_;
	int modelList_[CHARA_STATE::STATE_END];
	int hPict_;
	int rangePict_;
	bool attacked_;

	int hAttackSE_;

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
	Status status_;
	std::string text_;

	

	void LoadStatus(int _type)
	{
		CsvReader csv("GameData\\PuppetData.csv");

		for (int line = 0;line < csv.GetLines();line++)
		{
			//1行目説明のデータがあるから+1
			if (csv.GetString(line, 0) != "name" &&
				_type + 1 == line)
			{
				SetStatus(csv, line);
			}
		}

		hModel_ = modelList_[STAND];
		Model::SetAnimFrame(hModel_, 1, animData_.totalRunFrame_, animData_.runSpeed_);

		dir_ = DIRECTION::UP;

		isAlive_ = true;
		isAttack_ = false;
		attacked_ = false;

		for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
		{
			EmitterData data;
			particle_.push_back(data);
		}


	}

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
				SetParticle(csv, line);
			}
		}
	}

	void SetStatus(CsvReader _csv, int _line)
	{
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

	void SetParticle(CsvReader _csv, int _line)
	{
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


		for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
		{
			particle_[rangeNum] = data;
		}
	}

	XMINT2 rotate(XMINT2 _pos, int _dir)
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

	void Die() 
	{
		if (status_.hp_ < 1 && isAlive_)
		{
			isAlive_ = false;
			hModel_ = Model::Load("Model\\Garbage.fbx");
			assert(hModel_ >= 0);
		}
		
	}


	//キャラごとに変えられるようにしている（バフ・デバフなど）
	virtual void Attack() {};

};

