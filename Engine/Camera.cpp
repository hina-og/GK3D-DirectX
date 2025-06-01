#include "camera.h"
#include "Direct3D.h"
#include "./Time.h"

const XMFLOAT3 INIT_POSITION{ 0,10,0 };
const XMFLOAT3 INIT_TARGET{ 0,0,0 };

XMFLOAT3 _position;
XMFLOAT3 _initPosition;
XMFLOAT3 _target;
XMMATRIX _view;
XMMATRIX _proj;
XMMATRIX _billBoard;

bool zooming = false;//ズーム中か
float zoomElapsed = 0.0f;//ズーム開始からの経過時間
float zoomDuration = 0.0f;//ズーム完了までの時間
float zoomTotalDist = 0.0f;//ズームで進む距離
XMVECTOR zoomStartPos = XMVectorZero();
XMVECTOR zoomDirection = XMVectorZero();

bool shaking = false;
float shakeElapsed = 0.0f;
float shakeDuration = 0.0f;
float shakeMagnitude = 0.0f;
XMFLOAT3 shakeOffset = { 0, 0, 0 };

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
	_position = INIT_POSITION;	//カメラの位置
	_target = INIT_TARGET;	//カメラの焦点

	//プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//更新（ビュー行列作成）
void Camera::Update()
{

	if (zooming)//ズーム中
	{
		zoomElapsed += Time::GetDeltaTime();
		float t = zoomElapsed / zoomDuration;

		if (t >= 1.0f)
		{
			t = 1.0f;
			zooming = false;
		}

		XMVECTOR movedPos = XMVectorAdd(zoomStartPos, XMVectorScale(zoomDirection, zoomTotalDist * t));
		XMStoreFloat3(&_position, movedPos);
	}

	if (shaking)
	{
		shakeElapsed += Time::GetDeltaTime();
		float t = shakeElapsed / shakeDuration;

		if (t >= 1.0f)
		{
			shaking = false;
			shakeOffset = { 0, 0, 0 };
		}
		else
		{
			// 減衰付きノイズ（sinでも可）
			float decay = 1.0f - t;
			shakeOffset.x = ((rand() % 200 - 100) / 100.0f) * shakeMagnitude * decay;
			shakeOffset.y = ((rand() % 200 - 100) / 100.0f) * shakeMagnitude * decay;
			shakeOffset.z = ((rand() % 200 - 100) / 100.0f) * shakeMagnitude * decay;
		}
	}

	XMVECTOR camPos = XMVectorSet(_position.x + shakeOffset.x,
		_position.y + shakeOffset.y,
		_position.z + shakeOffset.z, 0);
	XMVECTOR camTarget = XMVectorSet(_target.x, _target.y, _target.z, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	_view = XMMatrixLookAtLH(camPos, camTarget, up);


	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);
}

//焦点を設定
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }

//位置を設定
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }

//焦点を取得
XMFLOAT3 Camera::GetTarget() { return _target; }

//位置を取得
XMFLOAT3 Camera::GetPosition() { return _position; }

XMVECTOR Camera::GetPositionXMVECTOR() { return XMLoadFloat3(&_position); }

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return _view; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }

void Camera::Zoom(float _dist, float _time)
{
	_initPosition = _position;

	zooming = true;
	zoomElapsed = 0.0f;
	zoomTotalDist = _dist;
	zoomDuration = _time;

	zoomStartPos = XMLoadFloat3(&_position);
	XMVECTOR vecTarget = XMLoadFloat3(&_target);
	zoomDirection = XMVector3Normalize(XMVectorSubtract(vecTarget, zoomStartPos));
}

void Camera::ZoomBack(float _time)
{
	zooming = true;
	zoomElapsed = 0.0f;
	zoomDuration = _time;

	zoomStartPos = XMLoadFloat3(&_position);
	XMVECTOR initPosVec = XMLoadFloat3(&_initPosition);
	zoomDirection = XMVector3Normalize(XMVectorSubtract(initPosVec, zoomStartPos));

	//今の位置からズーム前の一までの移動距離を計算
	XMVECTOR distanceVec = XMVectorSubtract(initPosVec, zoomStartPos);
	zoomTotalDist = XMVectorGetX(XMVector3Length(distanceVec));
}

void Camera::StartShake(float magnitude, float duration)
{
	shaking = true;
	shakeElapsed = 0.0f;
	shakeDuration = duration;
	shakeMagnitude = magnitude;
	shakeOffset = { 0, 0, 0 };
}

