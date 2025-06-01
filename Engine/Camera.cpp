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

bool zooming = false;//�Y�[������
float zoomElapsed = 0.0f;//�Y�[���J�n����̌o�ߎ���
float zoomDuration = 0.0f;//�Y�[�������܂ł̎���
float zoomTotalDist = 0.0f;//�Y�[���Ői�ދ���
XMVECTOR zoomStartPos = XMVectorZero();
XMVECTOR zoomDirection = XMVectorZero();

bool shaking = false;
float shakeElapsed = 0.0f;
float shakeDuration = 0.0f;
float shakeMagnitude = 0.0f;
XMFLOAT3 shakeOffset = { 0, 0, 0 };

//�������i�v���W�F�N�V�����s��쐬�j
void Camera::Initialize()
{
	_position = INIT_POSITION;	//�J�����̈ʒu
	_target = INIT_TARGET;	//�J�����̏œ_

	//�v���W�F�N�V�����s��
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//�X�V�i�r���[�s��쐬�j
void Camera::Update()
{

	if (zooming)//�Y�[����
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
			// �����t���m�C�Y�isin�ł��j
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

	//���̈ʒu����Y�[���O�̈�܂ł̈ړ��������v�Z
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

