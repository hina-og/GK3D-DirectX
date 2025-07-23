#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/Time.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/Model.h"
#include "Engine/Audio.h"
#include "Engine/VFX.h"

//�L�����̎��
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

//�L�����̏��
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
	//���̎��
	enum SOUND_TYPE
	{
		ATTACK = 0,
		DEAD,
		SOUND_END
	};

	const float effectPosY = 0.5;//�U���G�t�F�N�g���o�鍂���i�L�����̒��S��0.5�j
public:
	//����
	enum DIRECTION
	{
		DOWN = 0,
		LEFT,
		UP,
		RIGHT
	};

	DIRECTION dir_;//�L�����̌���

	std::vector<XMINT2> range_;//�����̈ʒu���i0,0�j�̏ꍇ�̍U���͈͂̃}�X
	std::vector<XMFLOAT3> rangePos_;//�U���͈͂̈ʒu
	std::vector<EmitterData> particle_;//�U���G�t�F�N�g�̔z��

	
	bool isAttack_;//�U�����Ă��邩
	bool isAlive_;//�����Ă��邩

	std::vector<Puppet*> inRangeChara_;//�U���͈͓��ɂ���L�����̔z��

	//�L������HP�𑀍삷��֐��i�������}�C�i�X�ɂ���Ό��点��j
	void ControlHP (int _addNum)
	{
		status_.hp_ += _addNum;
		//���ʉ��Đ�
		Audio::Play(hAttackSE_);
	}

	//�����̃Z�b�^�[
	void SetDirection(int _dir)
	{
		dir_ = (Puppet::DIRECTION)_dir;

		//�U���͈͂������ɂ���ĕς���
		for (auto range : range_)
		{
			range = Rotate(range, dir_);
		}
	}

	//������ݒ�
	void FacingDirection()
	{
		int directionAngle = 90;//�����͉����玞�v����90�x����
		transform_.rotate_.y = dir_ * directionAngle;
	}

	//�����Ă�������ɓ���
	void Move(DIRECTION _dir)
	{
		
		if (hModel_ != modelList_[RUN])//���f���̃A�j���[�V������RUN�ɂ���
		{
			hModel_ = modelList_[RUN];
			Model::SetAnimFrame(hModel_, 0, animData_.totalRunFrame_, animData_.runSpeed_);
		}
		else if (animData_.eRun_ <= Model::GetAnimFrame(hModel_))//RUN�A�j���[�V�������I�������ŏ��̃t���[���ɖ߂�
		{
			Model::SetAnimFrame(hModel_, animData_.sRun_, animData_.eRun_, animData_.runSpeed_);
		}

		//�ʒu�������Ă�����ɓ�����
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

	//�U���͂̃Q�b�^�[
	int GetPower() { return status_.power_; }
	//HP�̃Q�b�^�[
	int GetHitPoint() { return status_.hp_; }
	//���x�̃Q�b�^�[
	float GetSpeed() { return status_.speed_; }
	//�L�����̐������̃Q�b�^�[
	std::string GetText() { return text_; }
	//�U���͈͂̃Q�b�^�[
	std::vector<XMINT2> GetAttackTiles()
	{
		return range_;
	}

	//������
	virtual void Initialize() {};
	//�X�V
	void Update()
	{
		//�����̐ݒ�
		FacingDirection();


		//HP���c���Ă�
		if (status_.hp_ > 0)
		{
			//�U����Ԃł���
			if (isAttack_)
			{
				//�U���A�j���[�V�����ɂ���
				if (hModel_ != modelList_[CHARA_STATE::ATTACK])
				{
					hModel_ = modelList_[CHARA_STATE::ATTACK];
					Model::SetAnimFrame(hModel_, 0, animData_.totalAttackFrame_, animData_.attackSpeed_);
				}
				//�U������t���[���ɂȂ�����U������
				if (Model::GetAnimFrame(hModel_) >= animData_.attack_ && !attacked_)
					Attack();
			}
			//�U����Ԃ���Ȃ��������Ă��Ȃ��ꍇ�̓f�t�H���g�̃��f���ɂ���
			else if (hModel_ != modelList_[CHARA_STATE::RUN])
			{
				hModel_ = modelList_[CHARA_STATE::STAND];
			}
		}

		//�U���A�j���[�V�������I�������U����Ԃ�����
		if (hModel_ == modelList_[CHARA_STATE::ATTACK] && 
			Model::GetAnimFrame(hModel_) >= animData_.totalAttackFrame_)
		{
			isAttack_ = false;
			attacked_ = false;
		}

		for (int rangeNum = 0; rangeNum < range_.size(); rangeNum++)
		{
			//�U���͈͂̈ʒu���L�����̈ʒu����ɍX�V����
			rangePos_[rangeNum] = { 
				transform_.position_.x + range_[rangeNum].x,
				effectPosY,
				transform_.position_.z + range_[rangeNum].y 
			};
		}

		Die();
	};
	//�`��
	void Draw()
	{
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	};
	//�J��
	void Release() {};
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

	AnimationData animData_;//�A�j���[�V�����̃f�[�^
	int hModel_;//�\�����郂�f���̃n���h��
	int modelList_[CHARA_STATE::STATE_END];//���f����ύX���₷�����邽�߂̔z��
	bool attacked_;//�A�j���[�V������1���[�v�̒��Ŋ��ɍU��������
	int hAttackSE_;//�U�����̌��ʉ�

	struct Status
	{
		//-----�X�e�[�^�X-----
		/*�@���ƂŃp�����[�^�͓ǂݍ��ނ悤�ɂ���@*/
		std::string name_;
		int hp_;//�̗�
		int cost_;//�����R�X�g
		int power_;//�U����
		float speed_;//�ړ����x
		//--------------------
	};
	Status status_;//�L�����̃X�e�[�^�X���
	std::string text_;//�L�����̐�����

	
	//�X�e�[�^�X�̓ǂݍ���
	void LoadStatus(int _type)
	{
		CsvReader csv("GameData\\PuppetData.csv");

		for (int line = 0;line < csv.GetLines();line++)
		{
			//1�s�ڐ����̃f�[�^�����邩�� + 1
			if (csv.GetString(line, 0) != "name" &&
				_type + 1 == line)
			{
				SetStatus(csv, line);
			}
		}

		//------���낢�돉����Ԃɂ���------
		hModel_ = modelList_[STAND];
		Model::SetAnimFrame(hModel_, 0, animData_.totalRunFrame_, animData_.runSpeed_);

		dir_ = DIRECTION::UP;

		isAlive_ = true;
		isAttack_ = false;
		attacked_ = false;
		//----------------------------------

		//�U���͈͂̃}�X�̐��G�t�F�N�g��p�ӂ���
		for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
		{
			EmitterData data;
			particle_.push_back(data);
		}
	}

	//�G�t�F�N�g���̓ǂݍ���
	void LoadParticle(int _type)
	{
		CsvReader csv("Particle\\PuppetParticleData.csv");

		for (int line = 0;line < csv.GetLines();line++)
		{
			//1�s�ڐ����A2�s�ڃf�t�H���g�̃f�[�^�����邩��+2
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

	//�X�e�[�^�X�̃Z�b�^�[
	void SetStatus(CsvReader _csv, int _line)
	{
		//csv�ɓ����Ă���f�[�^
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

	//�G�t�F�N�g�̃Z�b�^�[
	EmitterData SetParticle(CsvReader _csv, int _line)
	{
		//csv�ɓ����Ă���f�[�^
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

	//�U���͈͂̃}�X�������ɂ���ĕς���֐�
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

	//����ł���Ƃ��̏���
	void Die() 
	{
		//HP���Ȃ���isAlive��true�̂Ƃ���
		if (status_.hp_ < 1 && isAlive_)
		{
			//�G�t�F�N�g���o��
			EmitterData deadParticle_;
			CsvReader csv("Particle\\PuppetParticleData.csv");
			deadParticle_ = SetParticle(csv, CHARA_TYPE::CHARA_END + 2);
			deadParticle_.position = transform_.position_;
			VFX::Start(deadParticle_);

			//�����Ă��Ȃ���Ԃɂ���
			isAlive_ = false;

			//���f�����S�~�R�ɂ���
			hModel_ = Model::Load("Model\\Garbage.fbx");
			assert(hModel_ >= 0);
		}
		
	}


	//�L�������Ƃɕς�����悤�ɂ��Ă���i�o�t�E�f�o�t�Ȃǁj
	virtual void Attack() {};

};

