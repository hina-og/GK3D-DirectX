#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include <DirectXMath.h>

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

			mapData_[y][x].pos_ = { (float)x - (WIDTH / 2) , 0 , (float)y - (HEIGHT / 2) };
			mapData_[y][x].select_ = false;
			mapData_[y][x].onPlayer_ = false;
		}
	}
}

void Stage::Update()
{

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			mapData_[y][x].select_ = false;
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

	XMVECTOR mouseFrontPos = { _pos.x,_pos.y };
	XMFLOAT3 mousePos;
	XMStoreFloat3(&mousePos, mouseFrontPos);
	mousePos.z = 0;
	mouseFrontPos = XMLoadFloat3(&mousePos);

	mousePos.z = 1;
	XMVECTOR mouseBackPos = XMLoadFloat3(&mousePos);

	mouseFrontPos = XMVector3TransformCoord(mouseFrontPos, invVP * invProj * invView);
	mouseBackPos = XMVector3TransformCoord(mouseBackPos, invVP * invProj * invView);

	XMFLOAT3 returnValue;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{

			RayCastData data;
			//レイの発射位置（マウス位置参照）

			XMStoreFloat3(&data.start, mouseFrontPos);
			XMStoreFloat3(&data.dir, XMVector3Normalize(mouseBackPos - mouseFrontPos));
			//正しいワールド座標を事前にセット！
			Transform ftrans;
			ftrans.position_ = {
				transform_.position_.x + mapData_[y][x].pos_.x,
				transform_.position_.y + mapData_[y][x].pos_.y,
				transform_.position_.z + mapData_[y][x].pos_.z
			};
			ftrans.rotate_ = transform_.rotate_;
			Model::SetTransform(mapData_[y][x].tileModel_, ftrans);

			Model::RayCast(mapData_[y][x].tileModel_, &data);
			if (data.hit)
			{
				mapData_[y][x].select_ = true;
				returnValue = mapData_[y][x].pos_;
				return returnValue;
			}
		}
	}

	return returnValue;
}
