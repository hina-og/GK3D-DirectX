#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"

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

			mapData_[y][x].pos_ = { /*(float)x,0,(float)y*/(float)x - (WIDTH / 2) , 0 , (float)y - (HEIGHT / 2)};
			mapData_[y][x].select_ = false;
			mapData_[y][x].onPlayer_ = false;
		}
	}
}

void Stage::Update()
{
	if (Input::IsMouseButtonDown(LEFT_CLICK))
	{
		XMMATRIX matView = Camera::GetViewMatrix();
		XMMATRIX matProj = Camera::GetProjectionMatrix();

		float w = Direct3D::screenWidth_ / 2;
		float h = Direct3D::screenHeight_ / 2;

		//ビューポート行列
		XMMATRIX vp =
		{
			w,0,0,0,
			0,-h,0,0,
			0,0,1,0,
			w,h,0,1
		};

		XMMATRIX invView = XMMatrixInverse(nullptr, matView);
		XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);

		XMVECTOR mouseFrontPos = { Input::GetMousePosition().x,Input::GetMousePosition().y };
		XMFLOAT3 mousePos;
		XMStoreFloat3(&mousePos, mouseFrontPos);
		mousePos.z = 0;
		mouseFrontPos = XMLoadFloat3(&mousePos);

		mousePos.z = 1;
		XMVECTOR mouseBackPos = XMLoadFloat3(&mousePos);

		mouseFrontPos = XMVector3TransformCoord(mouseFrontPos, invVP * invProj * invView);
		mouseBackPos = XMVector3TransformCoord(mouseBackPos, invVP * invProj * invView);

		RayCastData data;
		//レイの発射位置（マウス位置参照）
		XMStoreFloat3(&data.start, mouseFrontPos);
		XMStoreFloat3(&data.dir, mouseBackPos - mouseFrontPos);
		for (int y = 0;y < HEIGHT;y++)
		{
			for (int x = 0;x < WIDTH;x++)
			{
				Model::RayCast(mapData_[y][x].tileModel_, &data);
				if (data.hit)
				{
					return;
				}
			}
		}
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
			else
			{
				int o = 0;
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
