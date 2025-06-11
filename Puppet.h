#pragma once
#include <vector>
#include <sstream>
#include "Engine/GameObject.h"
#include "Engine/Time.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Engine/Audio.h"

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
public:
	struct Pos
	{
		int x;
		int y;
	};

	enum DIRECTION
	{
		UP = 0,
		LEFT,
		DOWN,
		RIGHT
	};

	DIRECTION dir_;

	std::vector<Pos> range_;
	
	bool isAttack_;
	bool isAlive_;//生きているか

	std::vector<Puppet*> inRangeChara_;

	void ControlHP (int _addNum)
	{
		status_.hp_ += _addNum;
		Audio::Play(hAttackSE_);
	}

	std::vector<Pos> GetAttackTiles(DIRECTION _dir)
	{
		std::vector<Pos> result;
		for (int i = 0; i < range_.size(); i++)
		{
			Pos rotated = rotate(range_[i], _dir);
			Pos target = { rotated.x, rotated.y };
			result.push_back(target);
		}
		return result;
	}

	void SetDirection(DIRECTION _dir)
	{
		dir_ = _dir;
	}

	void SetDirection(int _dir)
	{
		switch (_dir)
		{
		case 0:
			dir_ = UP;
			break;
		case 1:
			dir_ = LEFT;
			break;
		case 2:
			dir_ = DOWN;
			break;
		case 3:
			dir_ = RIGHT;
			break;
		default:
			break;
		}
	}

	void FacingDirection()
	{
		switch (dir_)
		{
		case Puppet::UP:
			transform_.rotate_.y = 180.0;
			break;
		case Puppet::LEFT:
			transform_.rotate_.y = 90.0;
			break;
		case Puppet::DOWN:
			transform_.rotate_.y = 0.0;
			break;
		case Puppet::RIGHT:
			transform_.rotate_.y = 270.0;
			break;
		default:
			break;
		}
	}

	void Move(DIRECTION _dir)
	{
		if (hModel_ != modelList_[RUN])
		{
			hModel_ = hModel_ = modelList_[RUN];
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
		if (_type == "Mushrrom") return CHARA_TYPE::MUSHROOM;
		if (_type == "Slime") return CHARA_TYPE::SLIME;
		if (_type == "Golem") return CHARA_TYPE::GOLEM;
		if (_type == "Ghost") return CHARA_TYPE::GHOST;
	}

	int GetPower() { return status_.power_; }
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
	int modelList_[STATE_END];
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
		float speed_;//移動速度や攻撃速度
		//--------------------
	};
	Status status_;

	void Initialize() {};
	void Update() 
	{
		FacingDirection();

		if(status_.hp_ > 0)
		{
			if (isAttack_)
			{
				if (hModel_ != modelList_[ATTACK])
				{
					hModel_ = modelList_[ATTACK];
					Model::SetAnimFrame(hModel_, 1, animData_.totalAttackFrame_, animData_.attackSpeed_);
				}
				if (Model::GetAnimFrame(hModel_) >= animData_.attack_ && !attacked_)
					Attack();
			}
			else if (hModel_ != modelList_[RUN])
			{
				hModel_ = modelList_[RUN];
				Model::SetAnimFrame(hModel_, 15, animData_.eRun_, animData_.runSpeed_);
			}
		}


		if (hModel_ == modelList_[ATTACK] && Model::GetAnimFrame(hModel_) >= animData_.totalAttackFrame_)
		{
			isAttack_ = false;
			attacked_ = false;
		}

		Die();
	};
	void Draw() {};
	void Release() {};

	void LoadStatus(int _type)
	{
		CsvReader csv("GameData\\PuppetData.csv");

		for (int line = 1;line < csv.GetLines();line++)
		{
			if (_type + 1 == line)
			{
				SetStatus(csv, line);
			}
		}

	}

	void SetStatus(CsvReader _csv, int _line)
	{
		enum Read_Data
		{
			Name = 0,
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
			
			std::string str = _csv.GetString(_line, column);
			Pos readRange;
			char brace1, comma, brace2;

			std::stringstream ss(str);
			ss >> brace1 >> readRange.x >> comma >> readRange.y >> brace2;
			if (brace1 == '{' && comma == '|' && brace2 == '}') 
			{
				range_.push_back(readRange);
			}

		}

		modelList_[STAND] = Model::Load("Model\\" + status_.name_ + "\\" + status_.name_ + ".fbx");
		assert(modelList_[STAND] >= 0);
		modelList_[RUN] = Model::Load("Model\\" + status_.name_ + "\\" + status_.name_ + "_Run.fbx");
		assert(modelList_[RUN] >= 0);
		modelList_[ATTACK] = Model::Load("Model\\" + status_.name_ + "\\" + status_.name_ + "_Attack.fbx");
		assert(modelList_[ATTACK] >= 0);
	}

	Pos rotate(Pos _pos, int _dir)
	{
		switch (_dir)
		{
		case DIRECTION::UP:
			return { _pos.x,  _pos.y };
		case DIRECTION::RIGHT:
			return { -_pos.y, _pos.x };
		case DIRECTION::DOWN:
			return { -_pos.x, -_pos.y };
		case DIRECTION::LEFT:
			return { _pos.y,  -_pos.x };
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

	virtual void Attack() {};

};

