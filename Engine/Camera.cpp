#include "camera.h"
#include "Direct3D.h"
#include "./Time.h"

XMFLOAT3 _position;
XMFLOAT3 _initPosition;
XMFLOAT3 _target;
XMFLOAT3 _initTarget;
XMMATRIX _view;
XMMATRIX _proj;
XMMATRIX _billBoard;

bool zooming = false;//�Y�[������
float zoomElapsed = 0.0f;//�Y�[���J�n����̌o�ߎ���
float zoomDuration = 0.0f;//�Y�[�������܂ł̎���

float positionZoomTotalDist = 0.0f;//�Y�[���Ői�ދ���
XMVECTOR positionZoomStartPos = XMVectorZero();
XMVECTOR positionZoomDirection = XMVectorZero();

float targetZoomTotalDist = 0.0f;//�Y�[���Ői�ދ���
XMVECTOR targetZoomStartPos = XMVectorZero();
XMVECTOR targetZoomDirection = XMVectorZero();

bool shaking = false;
float shakeElapsed = 0.0f;
float shakeDuration = 0.0f;
float shakePower = 0.0f;
XMFLOAT3 shakeOffset = { 0, 0, 0 };

//�������i�v���W�F�N�V�����s��쐬�j
void Camera::Initialize()
{
	_initPosition = ReadInitData("Position");
	_initTarget = ReadInitData("Target");

	_position = _initPosition;
	_target = _initTarget;

	//�v���W�F�N�V�����s��
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//�X�V�i�r���[�s��쐬�j
void Camera::Update()
{

	if (zooming)
	{
		zoomElapsed += Time::GetDeltaTime();
		float t = zoomElapsed / zoomDuration;

		if (t >= 1.0f)
		{
			t = 1.0f;
			zooming = false;
		}

		XMVECTOR movedPos = XMVectorAdd(positionZoomStartPos, XMVectorScale(positionZoomDirection, positionZoomTotalDist * t));
		XMStoreFloat3(&_position, movedPos);

		XMVECTOR movedTarget = XMVectorAdd(targetZoomStartPos, XMVectorScale(targetZoomDirection, targetZoomTotalDist * t));
		XMStoreFloat3(&_target, movedTarget);

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
			float decay = 1.0f - t;
			shakeOffset.x = ((rand() % 200 - 100) / 100.0f) * shakePower * decay * 0.1;
			shakeOffset.y = ((rand() % 200 - 100) / 100.0f) * shakePower * decay * 0.1;
			shakeOffset.z = ((rand() % 200 - 100) / 100.0f) * shakePower * decay * 0.1;
		}
	}

	XMVECTOR camPos = XMVectorSet(_position.x + shakeOffset.x,
		_position.y + shakeOffset.y,
		_position.z + shakeOffset.z, 0);
	XMVECTOR camTarget = XMVectorSet(_target.x, _target.y, _target.z, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	_view = XMMatrixLookAtLH(camPos, camTarget, up);


	//�r���{�[�h�s��
	//�i��ɃJ�����̕��������悤�ɉ�]������s��B�p�[�e�B�N���ł����g��Ȃ��j
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);
}

//�œ_��ݒ�
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }

//�ʒu��ݒ�
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }

//�œ_���擾
XMFLOAT3 Camera::GetTarget() { return _target; }

//�ʒu���擾
XMFLOAT3 Camera::GetPosition() { return _position; }

XMVECTOR Camera::GetPositionXMVECTOR() { return XMLoadFloat3(&_position); }

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix() { return _view; }

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//�r���{�[�h�p��]�s����擾
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }

void Camera::Zoom(float _dist, float _time)
{
	_initPosition = _position;
	_initTarget = _target;

	zooming = true;
	zoomElapsed = 0.0f;
	positionZoomTotalDist = _dist;
	zoomDuration = _time;

	positionZoomStartPos = XMLoadFloat3(&_position);
	XMVECTOR vecTarget = XMLoadFloat3(&_target);
	positionZoomDirection = XMVector3Normalize(XMVectorSubtract(vecTarget, positionZoomStartPos));
}

void Camera::ZoomBack(float _time)
{
	zooming = true;
	zoomElapsed = 0.0f;
	zoomDuration = _time;

	positionZoomStartPos = XMLoadFloat3(&_position);
	XMVECTOR initPosVec = XMLoadFloat3(&_initPosition);
	positionZoomDirection = XMVector3Normalize(XMVectorSubtract(initPosVec, positionZoomStartPos));
	//���̈ʒu����Y�[���O�̈�܂ł̈ړ��������v�Z
	XMVECTOR distanceVec = XMVectorSubtract(initPosVec, positionZoomStartPos);
	positionZoomTotalDist = XMVectorGetX(XMVector3Length(distanceVec));

	targetZoomStartPos = XMLoadFloat3(&_target);
	XMVECTOR initTargetVec = XMLoadFloat3(&_initTarget);
	targetZoomDirection = XMVector3Normalize(XMVectorSubtract(initTargetVec, targetZoomStartPos));
	XMVECTOR targetDistanceVec = XMVectorSubtract(initTargetVec, targetZoomStartPos);
	targetZoomTotalDist = XMVectorGetX(XMVector3Length(targetDistanceVec));
}

void Camera::StartShake(float _power, float _time)
{
	shaking = true;
	shakeElapsed = 0.0f;
	shakePower = _power;
	shakeDuration = _time;
	shakeOffset = { 0, 0, 0 };
}

XMFLOAT3 Camera::ReadInitData(std::string _key)
{
	std::string buffer;
	buffer.resize(100);
	DWORD charsRead = GetPrivateProfileString("CAMERA", _key.c_str(), "0.0,10.0,0.0", &buffer[0], static_cast<DWORD>(buffer.size()), ".\\setup.ini");
	buffer.resize(charsRead);//���ۂɓǂݍ��񂾕������ŃT�C�Y����

	XMFLOAT3 result;
	sscanf_s(buffer.c_str(), "%f,%f,%f", &result.x, &result.y, &result.z);

	return result;
}

