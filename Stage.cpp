#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent)
	: GameObject(parent,"Stage")
{
}

void Stage::Initialize()
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			mapData_[y][x].tileModel_ = Model::Load("Model\\Tile.fbx");
			assert(mapData_[y][x].tileModel_ >= 0);

			mapData_[y][x].pos_ = { (float)x , 0 , (float)y };
			mapData_[y][x].select_ = false;
			mapData_[y][x].onPlayer_ = false;
		}
	}
}

void Stage::Update()
{
	transform_.rotate_.y += 1.0;

	if (Input::IsKey(DIK_A))
	{
		transform_.position_.x -= 0.01;
	}
	if (Input::IsKey(DIK_D))
	{
		transform_.position_.x += 0.01;
	}
	if (Input::IsKey(DIK_S))
	{
		transform_.position_.z -= 0.01;
	}
	if (Input::IsKey(DIK_W))
	{
		transform_.position_.z += 0.01;
	}
}

void Stage::Draw()
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (!mapData_[y][x].select_)
			{
				Transform ftrans;
				ftrans.position_ = { transform_.position_.x + mapData_[y][x].pos_.x,transform_.position_.y + mapData_[y][x].pos_.y ,transform_.position_.z + mapData_[y][x].pos_.z };
				ftrans.rotate_ = transform_.rotate_;
				Model::SetTransform(mapData_[y][x].tileModel_, ftrans);
				Model::Draw(mapData_[y][x].tileModel_);
			}
		}
	}
}

void Stage::Release()
{
}

bool Stage::HasPlayer(XMFLOAT3 _pos)
{
	bool isSelected = false;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (mapData_[y][x].select_)
			{
				isSelected = true;
				//選んだマスにプレイヤーキャラがいるならtrue
				if (mapData_[y][x].onPlayer_)
					return true;
				else
					mapData_[y][x].onPlayer_ = true;
			}
		}
	}

	//ステージを選んでいない場合
	if (!isSelected)
	{
		return true;
	}
	return false;
}

XMFLOAT3 Stage::SelectTile(XMFLOAT3 _pos)
{
	XMFLOAT3 returnValue;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (mapData_[y][x].pos_.x - 32.0 <  _pos.x &&
				mapData_[y][x].pos_.x + 32.0 >= _pos.x &&
				mapData_[y][x].pos_.y - 32.0 <  _pos.y &&
				mapData_[y][x].pos_.y + 32.0 >= _pos.y)
			{
				mapData_[y][x].select_ = true;

				returnValue = mapData_[y][x].pos_;
			}
			else
			{
				mapData_[y][x].select_ = false;
			}
		}
	}

	return returnValue;
}
