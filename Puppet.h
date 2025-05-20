#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/Time.h"
#include "Engine/Input.h"

enum CHARA_TYPE
{
	MOUSE = 0,
	ZOMBIE,
	MUSHROOM,
	SLIME,
	GOLEM,
	GHOST,
	FAILURE,
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
		hp_ += _addNum;
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
			transform_.position_.z += speed_ * Time::GetDeltaTime();
			break;
		case Puppet::LEFT:
			transform_.position_.x -= speed_ * Time::GetDeltaTime();
			break;
		case Puppet::DOWN:
			transform_.position_.z -= speed_ * Time::GetDeltaTime();
			break;
		case Puppet::RIGHT:
			transform_.position_.x += speed_ * Time::GetDeltaTime();
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

	int GetPower() { return power_; }

protected:

	int hModel_;
	int modelList_[STATE_END];
	int hPict_;
	int rangePict_;
	bool attacked_;


	//-----ステータス-----
	/*　あとでパラメータは読み込むようにする　*/
	int hp_;//体力
	int cost_;//召喚コスト
	int power_;//攻撃力
	float speed_;//移動速度や攻撃速度
	//--------------------



	void Initialize() {};
	void Update() {};
	void Draw() {};
	void Release() {};

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

