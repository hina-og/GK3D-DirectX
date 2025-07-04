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
	//カメラ位置・向きの初期化
	Camera::SetPosition(Camera::ReadInitData("Position"));
	Camera::SetTarget(Camera::ReadInitData("Target"));


	//タイルを１つずつ初期化
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			mapData_[y][x].hTile_ = Model::Load("Model\\Tile.fbx");
			assert(mapData_[y][x].hTile_ >= 0);


			//中央が{0,0}になるように
			mapData_[y][x].pos_ = { (float)x - (WIDTH / 2) , 0 , (float)y - (HEIGHT / 2) };
			mapData_[y][x].select_ = false;
			mapData_[y][x].onPlayer_ = false;
		}
	}

	//ステージの左下と右上のタイルのデータを取得
	beginData_ = mapData_[0][0];
	endData_ = mapData_[HEIGHT - 1][WIDTH - 1];


	spawnLine_ = endData_.pos_.z + TILE_SIZE / 2;//ステージのタイル上にスポーンしてほしいからここで / 2してる
	wallLine_ = beginData_.pos_.z - TILE_SIZE / 2;//ここも上と同じようにタイル上にいるようにしてほしいから / 2

	//地面（タイルではない見た目だけの存在）
	hGround_ = Model::Load("Model\\Ground.fbx");
	assert(hGround_ >= 0);

	//手前の壁
	hWall_ = Model::Load("Model\\Wall.fbx");
	assert(hWall_ >= 0);
	Transform wallTrans;
	wallTrans.position_ = { 0,0,wallLine_ };
	Model::SetTransform(hWall_, wallTrans);

	//選択しているタイルをわかりやすくするための目印
	hTileSign_ = Model::Load("Model\\TileSign.fbx");
	assert(hTileSign_ >= 0);
	Transform signTrans;
	signTrans.position_ = mapData_[0]->pos_ ;
	Model::SetTransform(hTileSign_, signTrans);

	isSelect_ = false;
	isZooming_ = false;
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

	//マウスホイールを下に回転したときズーム前の位置に戻す処理（ズームしている場合のみ）
	if (Input::GetMouseMove().z < 0 && isZooming_)
	{
		isZooming_ = false;
		float zoomTime = 0.1;
		Camera::ZoomBack(zoomTime);
	}
	//マウスホイールを上に回転したときズームする処理（ズームしていない場合のみ）
	if (Input::GetMouseMove().z > 0 && !isZooming_)
	{
		isZooming_ = true;
		float zoomDist = 10.0, zoomTime = 0.1;
		Camera::Zoom(zoomDist, zoomTime);
	}

	//ズームしている場合にカメラ移動が可能
	if (isZooming_)
	{
		XMFLOAT3 cameraPos = Camera::GetPosition();
		XMFLOAT3 targetPos = Camera::GetTarget();
		float cameraMoveDist = 0.1;
		if (Input::IsKey(DIK_W) && targetPos.z < endData_.pos_.z)
		{
			cameraPos.z += cameraMoveDist;
			targetPos.z += cameraMoveDist;
		}
		if (Input::IsKey(DIK_A) && targetPos.x > beginData_.pos_.x)
		{
			cameraPos.x -= cameraMoveDist;
			targetPos.x -= cameraMoveDist;
		}
		if (Input::IsKey(DIK_S) && targetPos.z > beginData_.pos_.z)
		{
			cameraPos.z -= cameraMoveDist;
			targetPos.z -= cameraMoveDist;
		}
		if (Input::IsKey(DIK_D) && targetPos.x < endData_.pos_.x)
		{
			cameraPos.x += cameraMoveDist;
			targetPos.x += cameraMoveDist;
		}
		Camera::SetPosition(cameraPos);
		Camera::SetTarget(targetPos);
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
			Model::SetTransform(mapData_[y][x].hTile_, ftrans);
			Model::Draw(mapData_[y][x].hTile_);
		}
	}
	Model::Draw(hGround_);
	Model::Draw(hWall_);
	if (isSelect_)
	{
		Model::Draw(hTileSign_);
	}
}

void Stage::Release()
{
}

bool Stage::HasPlayer(XMFLOAT2 _num)
{
	return (mapData_[(int)_num.y][(int)_num.x].select_ &&//選択している？
			mapData_[(int)_num.y][(int)_num.x].onPlayer_);//プレイヤーキャラが乗っている？
}

XMFLOAT3 Stage::SelectTilePosition(XMFLOAT2 _mousePos)
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

	XMVECTOR mouseFrontPos = { _mousePos.x,_mousePos.y };
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

			XMStoreFloat3(&data.start, mouseFrontPos);
			XMStoreFloat3(&data.dir, XMVector3Normalize(mouseBackPos - mouseFrontPos));


			Model::RayCast(mapData_[y][x].hTile_, &data);
			if (data.hit)
			{
				mapData_[y][x].select_ = true;
				returnValue = mapData_[y][x].pos_;

				Transform signTrans;
				signTrans.position_ = returnValue;
				Model::SetTransform(hTileSign_, signTrans);
				isSelect_ = true;
				return returnValue;
			}
		}
	}
	isSelect_ = false;
	return returnValue;
}

XMFLOAT2 Stage::SelectTileNumber(XMFLOAT2 _mousePos)
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

	XMVECTOR mouseFrontPos = { _mousePos.x,_mousePos.y };
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


			Model::RayCast(mapData_[y][x].hTile_, &data);
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

bool Stage::SelectTile(XMFLOAT2 _mousePos, XMFLOAT2& _outNum, XMFLOAT3& _outPos)
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

	XMVECTOR mouseFrontPos = { _mousePos.x, _mousePos.y };
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

			Model::RayCast(mapData_[y][x].hTile_, &data);
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
