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
	//残りの敵を計算して渡している
	hud->SetEnemyNum(spawnList_.size() - spawnedNum + enemy->unit_->GetPuppetArray().size());
	
	//BGMが再生されていない場合再生する
	if (!Audio::IsPlaying(hBGM_))
	{
		Audio::Play(hBGM_);
	}

	//マウス位置の取得
	int mouseX, mouseY;
	Input::GetMousePosition(mouseX, mouseY);

	//選択しているタイルを調べる
	XMFLOAT2 mousePos = { (float)mouseX, (float)mouseY };
	stage->SelectTilePosition(mousePos);


	//勝利条件を配列にする
	bool endCondition[END_CONDITIONS::END] =
	{
		(spawnedNum >= spawnList_.size() && enemy->unit_->GetPuppetArray().size() <= 0),//敵がすべて出現して、全滅したか
		(hud->time_ <= 0),																//時間がなくなったか
		(hud->HP_ <= 0)																	//体力がなくなったか	
	};

	if (!endCondition[END_CONDITIONS::All_Kill] &&//敵がいる
		!endCondition[END_CONDITIONS::Time_Up] &&//時間がある
		!endCondition[END_CONDITIONS::Dead])	  //体力がある
		//このとき続ける
	{
		for (int i = spawnedNum; i < spawnList_.size(); i++)
		{
			//敵の出現時間になったら敵を出す
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


		//ズームしていない場合はキャラの向きを選択
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


		//左クリックでキャラ配置
		if (Input::IsMouseButtonDown(LEFT_CLICK))
		{

			XMFLOAT2 tileNum = { -1, -1 };
			XMFLOAT3 tilePos = {};
			//レイキャストして当たったタイルに配置（タイルにキャラがいない場合）
			bool hit = stage->SelectTile(mousePos, tileNum, tilePos);
			if (hit && !stage->HasPlayer(tileNum) && material->isNotEmpty())
			{
				stage->PlaceCharacter(tileNum);
				player->unit_->AddCharacter(tilePos, material->GetSelectStragePuppet(), selectDir);
			}
		}
		//上から向かってくる
		enemy->unit_->InvaderMove();

		//攻撃範囲に入ったかの判定プレイヤー、エネミー両方
		player->unit_->InRange(enemy->unit_->GetPuppetArray());
		enemy->unit_->InRange(player->unit_->GetPuppetArray());

		//敵が壁まで到達した場合の処理
		enemy->unit_->PastLine(stage->wallLine_, hud->HP_);

		//素材入手の時間になったら
		if (GiveMaterialTime <= getMaterialTime)
		{
			//素材入手
			material->GetRandomMaterial();
			//タイマーをリセット
			getMaterialTime -= GiveMaterialTime;
		}

		//素材タイマー更新
		getMaterialTime += Time::GetDeltaTime();

		//時間が残っているなら時間更新
		if (isTimeStart && hud->time_ >= 0)
			hud->TimeUpdate();
		//準備完了
		if (isReady)
			isTimeStart = true;
	}
	else if (!isBattleEnd)
	{
		//体力が残っているならゲームクリア
		//残っていなければゲームオーバー
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


