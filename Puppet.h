#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/Time.h"

enum CHARA_TYPE
{
	MOUSE = 0,
	ZOMBIE,
	FAILURE,
	CHARA_END
};



class Puppet
	: public GameObject
{
protected:

	

	struct Pos
	{
		int x;
		int y;
	};

	int hModel_;
	int hPict_;
	int rangePict_;

	

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

	bool InWindow(XMFLOAT3 _pos)
	{
		if (0 <= _pos.y && _pos.y <= 720 &&
			0 <= _pos.x && _pos.x <= 1280)
		{
			return true;
		}
		return false;
	}

	virtual void Attack() 
	{

	};

public:
	enum DIRECTION
	{
		UP = 0,
		DOWN,
		LEFT,
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

	void Move(DIRECTION _dir)
	{
		switch (dir_)
		{
		case Puppet::UP:
			transform_.position_.z += speed_ * Time::GetDeltaTime();
			transform_.rotate_.y = 180.0;
			break;
		case Puppet::DOWN:
			transform_.position_.z -= speed_ * Time::GetDeltaTime();
			transform_.rotate_.y = 0.0;
			break;
		case Puppet::LEFT:
			transform_.position_.x -= speed_ * Time::GetDeltaTime();
			transform_.rotate_.y = 240.0;
			break;
		case Puppet::RIGHT:
			transform_.position_.x += speed_ * Time::GetDeltaTime();
			transform_.rotate_.y = 120.0;
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
};

