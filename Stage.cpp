#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

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
			mapData_[y][x].hPict_ = Image::Load("Image\\empty.png");
			assert(mapData_[y][x].hPict_ >= 0);

			mapData_[y][x].pos_ = { x * 64.0f + ((1280.0f - (64.0f * WIDTH)) / 2.0f) - 416, y * 64.0f + 8.0f , 0 };
			mapData_[y][x].select_ = false;
			mapData_[y][x].onPlayer_ = false;
		}
	}
}

void Stage::Update()
{
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
		transform_.position_.y -= 0.01;
	}
	if (Input::IsKey(DIK_W))
	{
		transform_.position_.y += 0.01;
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
				ftrans.position_ = { transform_.position_.x + mapData_[y][x].pos_.x / 1280 , transform_.position_.y + mapData_[y][x].pos_.y / 720  ,0 };
				//transform_.position_ = { transform_.position_.x + mapData_[y][x].pos_.x , transform_.position_.y + mapData_[y][x].pos_.y  ,0 };
				ftrans.scale_ = { 0.1,0.1,0.1 };
				Image::SetTransform(mapData_[y][x].hPict_, ftrans);
				//Image::Draw(mapData_[y][x].hPict_);
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
