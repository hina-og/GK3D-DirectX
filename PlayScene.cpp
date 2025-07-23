#include "PlayScene.h"
#include "Engine/Input.h"
#include "Engine/Time.h"
#include "Engine/Camera.h"
#include "SetUp.h"
#include "Engine/CsvReader.h"
#include "Engine/Text.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Engine/Audio.h"
#include "File.h"
#include "PuppetFactory.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	hBGM_ = Audio::Load("Sounds\\BGM\\BGM.wav");
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);

	stage = Instantiate<Stage>(this);
	player = Instantiate<Player>(this);
	enemy = Instantiate<Enemy>(this);
	material = Instantiate<MaterialTable>(this);
	hud = Instantiate<HUD>(this);



	CsvReader levelData;

	std::string str = SetUp::currentDifficulty;
	levelData.Load(str);

	hud->time_ = levelData.GetFloat(STAGE_DATA, TIME);
	hud->InitHP(levelData.GetInt(STAGE_DATA, HP));
	material->GiveMaterial(levelData.GetInt(STAGE_DATA, GIVE_MATERIAL_NUM));

	for (int line = SPAWN_DATA;line < levelData.GetLines();line++)
	{
		SpawnData sData;
		Puppet p;
		sData.type_ = GetCharacterTypeFromName(levelData.GetString(line, SPAWN_NAME));
		sData.time_ = levelData.GetFloat(line, SPAWN_TIME);
		sData.line_ = levelData.GetInt(line, SPAWN_LINE);

		spawnList_.push_back(sData);
	}
	spawnedNum = 0;

	isTimeStart = false;

	getMaterialTime = 0.0;

	isReady = false;
	isBattleEnd = false;

	selectDir = Puppet::UP;
}

void PlayScene::Update()
{
	//�c��̓G���v�Z���ēn���Ă���
	hud->SetEnemyNum(spawnList_.size() - spawnedNum + enemy->unit_->GetPuppetArray().size());
	
	//BGM���Đ�����Ă��Ȃ��ꍇ�Đ�����
	if (!Audio::IsPlaying(hBGM_))
	{
		Audio::Play(hBGM_);
	}

	//�}�E�X�ʒu�̎擾
	int mouseX, mouseY;
	Input::GetMousePosition(mouseX, mouseY);

	//�I�����Ă���^�C���𒲂ׂ�
	XMFLOAT2 mousePos = { (float)mouseX, (float)mouseY };
	stage->SelectTilePosition(mousePos);


	//����������z��ɂ���
	bool endCondition[END_CONDITIONS::END] =
	{
		(spawnedNum >= spawnList_.size() && enemy->unit_->GetPuppetArray().size() <= 0),//�G�����ׂďo�����āA�S�ł�����
		(hud->time_ <= 0),																//���Ԃ��Ȃ��Ȃ�����
		(hud->HP_ <= 0)																	//�̗͂��Ȃ��Ȃ�����	
	};

	if (!endCondition[END_CONDITIONS::All_Kill] &&//�G������
		!endCondition[END_CONDITIONS::Time_Up] &&//���Ԃ�����
		!endCondition[END_CONDITIONS::Dead])	  //�̗͂�����
		//���̂Ƃ�������
	{
		for (int i = spawnedNum; i < spawnList_.size(); i++)
		{
			//�G�̏o�����ԂɂȂ�����G���o��
			if (hud->time_ < spawnList_[i].time_)
			{
				XMFLOAT3 pos = { 
					(float)(spawnList_[i].line_ - (WIDTH / 2 + 1)),
					0,
					stage->spawnLine_
				};
				enemy->unit_->AddCharacter(pos, spawnList_[i].type_, Puppet::DOWN);
				spawnedNum++;
			}
		}


		//�Y�[�����Ă��Ȃ��ꍇ�̓L�����̌�����I��
		if (!stage->IsZooming())
		{
			if (Input::IsKeyDown(DIK_S))
			{
				selectDir = Puppet::DOWN;
			}
			else if (Input::IsKeyDown(DIK_A))
			{
				selectDir = Puppet::LEFT;
			}
			else if (Input::IsKeyDown(DIK_W))
			{
				selectDir = Puppet::UP;
			}
			else if (Input::IsKeyDown(DIK_D))
			{
				selectDir = Puppet::RIGHT;
			}
		}
		hud->SetDirection(selectDir);


		//���N���b�N�ŃL�����z�u
		if (Input::IsMouseButtonDown(LEFT_CLICK))
		{

			XMFLOAT2 tileNum = { -1, -1 };
			XMFLOAT3 tilePos = {};
			//���C�L���X�g���ē��������^�C���ɔz�u�i�^�C���ɃL���������Ȃ��ꍇ�j
			bool hit = stage->SelectTile(mousePos, tileNum, tilePos);
			if (hit && !stage->HasPlayer(tileNum) && material->isNotEmpty())
			{
				stage->PlaceCharacter(tileNum);
				player->unit_->AddCharacter(tilePos, material->GetSelectStragePuppet(), selectDir);
			}
		}
		//�ォ��������Ă���
		enemy->unit_->InvaderMove();

		//�U���͈͂ɓ��������̔���v���C���[�A�G�l�~�[����
		player->unit_->InRange(enemy->unit_->GetPuppetArray());
		enemy->unit_->InRange(player->unit_->GetPuppetArray());

		//�G���ǂ܂œ��B�����ꍇ�̏���
		enemy->unit_->PastLine(stage->wallLine_, hud->HP_);

		//�f�ޓ���̎��ԂɂȂ�����
		if (GiveMaterialTime <= getMaterialTime)
		{
			//�f�ޓ���
			material->GetRandomMaterial();
			//�^�C�}�[�����Z�b�g
			getMaterialTime -= GiveMaterialTime;
		}

		//�f�ރ^�C�}�[�X�V
		getMaterialTime += Time::GetDeltaTime();

		//���Ԃ��c���Ă���Ȃ玞�ԍX�V
		if (isTimeStart && hud->time_ >= 0)
			hud->TimeUpdate();
		//��������
		if (isReady)
			isTimeStart = true;
	}
	else if (!isBattleEnd)
	{
		//�̗͂��c���Ă���Ȃ�Q�[���N���A
		//�c���Ă��Ȃ���΃Q�[���I�[�o�[
		if (hud->HP_ > 0)
		{
			Instantiate<GameClear>(this);
		}
		else
		{
			Instantiate<GameOver>(this);
		}
		isBattleEnd = true;
	}
}

void PlayScene::Draw()
{
	isReady = true;
}

void PlayScene::Release()
{
	Image::AllRelease();
}


