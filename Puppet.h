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
	bool isAlive_;//�����Ă��邩

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
	}

	int GetPower() { return status_.power_; }

protected:

	struct AnimationData
	{
		int sRun_;//Run�A�j���[�V�����̃��[�v�J�n�t���[��
		int eRun_;//Run�A�j���[�V�����̃��[�v�I���t���[��
		int runSpeed_;//Run�A�j���[�V�����̑��x
		int totalRunFrame_;//Run�A�j���[�V�����̃t���[����
		int attack_;//�U������t���[��
		int attackSpeed_;//Attack�A�j���[�V�����̑��x
		int totalAttackFrame_;//Run�A�j���[�V�����̃t���[����
	};

	AnimationData animData_;
	int hModel_;
	int modelList_[STATE_END];
	int hPict_;
	int rangePict_;
	bool attacked_;

	struct Status
	{
		//-----�X�e�[�^�X-----
		/*�@���ƂŃp�����[�^�͓ǂݍ��ނ悤�ɂ���@*/
		std::string name_;
		int hp_;//�̗�
		int cost_;//�����R�X�g
		int power_;//�U����
		float speed_;//�ړ����x��U�����x
		//--------------------
	};
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
		}
	}

	void SetStatus(CsvReader _csv, int _line)
	{
		status_.name_ = _csv.GetString(_line, 0);
		status_.hp_ = _csv.GetInt(_line, 1);
		status_.cost_ = _csv.GetInt(_line, 2);
		status_.power_ = _csv.GetInt(_line, 3);
		status_.speed_ = _csv.GetFloat(_line, 4);
		animData_ = { _csv.GetInt(_line, 5), _csv.GetInt(_line, 6), _csv.GetInt(_line, 7), _csv.GetInt(_line, 8), _csv.GetInt(_line, 9), _csv.GetInt(_line, 10), _csv.GetInt(_line, 11) };


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

