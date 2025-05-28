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
	startLine_ = mapData_[HEIGHT-1][0].pos_.z + 1;
	endLine_ = mapData_[0][0].pos_.z - 0.5;

	hGround_ = Model::Load("Model\\Ground.fbx");
	assert(hGround_ >= 0);
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
			Transform ftrans;
			ftrans.position_ = {
				transform_.position_.x + mapData_[y][x].pos_.x,
				transform_.position_.y + mapData_[y][x].pos_.y,
				transform_.position_.z + mapData_[y][x].pos_.z
			};
			ftrans.rotate_ = transform_.rotate_;
			Model::SetTransform(mapData_[y][x].tileModel_, ftrans);
			Model::Draw(mapData_[y][x].tileModel_);
		}
	}
	Model::Draw(hGround_);
}

void Stage::Release()
{
}

bool Stage::HasPlayer(XMFLOAT2 _num)
{
	return (mapData_[(int)_num.y][(int)_num.x].select_ &&mapData_[(int)_num.y][(int)_num.x].onPlayer_);
}

XMFLOAT3 Stage::SelectTilePosition(XMFLOAT2 _pos)
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

XMFLOAT2 Stage::SelectTileNumber(XMFLOAT2 _pos)
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

	XMFLOAT2 returnValue = { 0,0 };

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{

			RayCastData data;
			//レイの発射位置（マウス位置参照）

			XMStoreFloat3(&data.start, mouseFrontPos);
			XMStoreFloat3(&data.dir, XMVector3Normalize(mouseBackPos - mouseFrontPos));


			Model::RayCast(mapData_[y][x].tileModel_, &data);
			if (data.hit)
			{
				mapData_[y][x].select_ = true;
				returnValue = { (float)x,(float)y };
				return returnValue;
			}
		}
	}

	return returnValue;
}

bool Stage::SelectTile(XMFLOAT2 _screenPos, XMFLOAT2& _outNum, XMFLOAT3& _outPos)
{

	XMMATRIX matView = Camera::GetViewMatrix();
	XMMATRIX matProj = Camera::GetProjectionMatrix();

	float w = Direct3D::screenWidth_ / 2;
	float h = Direct3D::screenHeight_ / 2;

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

	XMVECTOR mouseFrontPos = { _screenPos.x, _screenPos.y };
	XMFLOAT3 mousePos;
	XMStoreFloat3(&mousePos, mouseFrontPos);
	mousePos.z = 0;
	mouseFrontPos = XMLoadFloat3(&mousePos);
	mousePos.z = 1;
	XMVECTOR mouseBackPos = XMLoadFloat3(&mousePos);

	mouseFrontPos = XMVector3TransformCoord(mouseFrontPos, invVP * invProj * invView);
	mouseBackPos = XMVector3TransformCoord(mouseBackPos, invVP * invProj * invView);

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			RayCastData data;
			XMStoreFloat3(&data.start, mouseFrontPos);
			XMStoreFloat3(&data.dir, XMVector3Normalize(mouseBackPos - mouseFrontPos));

			Model::RayCast(mapData_[y][x].tileModel_, &data);
			if (data.hit)
			{
				mapData_[y][x].select_ = true;
				_outNum = { (float)x, (float)y };
				_outPos = mapData_[y][x].pos_;
				return true;
			}
		}
	}

	return false;
}

void Stage::PlaceCharacter(XMFLOAT2 _num)
{
	if (mapData_[(int)_num.y][(int)_num.x].select_)
	{
		mapData_[(int)_num.y][(int)_num.x].onPlayer_ = true;
	}
}
