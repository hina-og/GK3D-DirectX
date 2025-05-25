#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/Time.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"

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
			transform_.rotate_.y = 270.0;
			break;
		case Puppet::DOWN:
			transform_.rotate_.y = 0.0;
			break;
		case Puppet::RIGHT:
			transform_.rotate_.y = 90.0;
			break;
		default:
			break;
		}
	}

	void Move(DIRECTION _dir)
	{
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
	}

	int GetPower() { return status_.power_; }

protected:

	int hModel_;
	int modelList_[STATE_END];
	int hPict_;
	int rangePict_;
	bool attacked_;

	struct AnimData
	{
		int sAttack_;//Attackアニメーションの始まり
		int eAttack_;//Attackアニメーションの終わり
		int attack_;//攻撃するフレーム
		
		int sRun_;//Runアニメーションのループ開始フレーム
		int eRun_;//Runアニメーションのループ終了フレーム
	};

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
	AnimData anim_;
	Status status_;

	void Initialize() {};
	void Update() {};
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
			/*switch (_type)
			{
			case MOUSE:
				SetStatus(csv, _type);
				break;
			case ZOMBIE:
				SetStatus(csv, _type);
				break;
			case MUSHROOM:
				SetStatus(csv, _type);
				break;
			case SLIME:
				SetStatus(csv, _type);
				break;
			case GOLEM:
				SetStatus(csv, _type);
				break;
			case GHOST:
				SetStatus(csv, _type);
				break;
			default:
				SetStatus(csv, MOUSE);
				break;
			}*/
		}
	}

	void SetStatus(CsvReader _csv, int _line)
	{
		status_.name_ = _csv.GetString(_line, 0);
		status_.hp_ = _csv.GetInt(_line, 1);
		status_.cost_ = _csv.GetInt(_line, 2);
		status_.power_ = _csv.GetInt(_line, 3);
		status_.speed_ = _csv.GetFloat(_line, 4);

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

	virtual void Attack() {};
};

