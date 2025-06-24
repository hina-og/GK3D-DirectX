#pragma once
#include <wtypes.h>
#include <string>
#include "Engine/GameObject.h"

class Animation
{
	float x_;
	float y_;
	RECT rect_;
	int hPict_;

	float speed_;
	float addSpeed_;

	bool doLoop_;
	bool doReverse_;
	bool doStart_;

	int nowFrame_;//���̃t���[��
	int totalFrame_;//�S�A�j���[�V�����t���[��

	bool isDrawAfterAnimation;//�A�j���[�V�������I����Ă��`������邩
public:
	Animation();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_fileName">�F�摜�̃p�X</param>
	/// <param name="_x">�F�`�悷��X���W</param>
	/// <param name="_y">�F�`�悷��Y���W</param>
	/// <param name="_w">�F�P�t���[���̉���</param>
	/// <param name="_h">�F�P�t���[���̏c��</param>
	/// <param name="_loop">�F�I�������J��Ԃ���</param>
	/// <param name="_frame">�F�S�t���[����</param>
	/// <param name="_endDraw">�F�I����Ă��`������邩</param>
	Animation(std::string _fileName,int _x,int _y,int _w,int _h,bool _loop,int _frame,bool _endDraw);
	/// <summary>
	/// �A�j���[�V�����̏�����
	/// </summary>
	/// <param name="_fileName">�F�摜�̃p�X</param>
	/// <param name="_x">�F�`�悷��X���W</param>
	/// <param name="_y">�F�`�悷��Y���W</param>
	/// <param name="_w">�F�P�t���[���̉���</param>
	/// <param name="_h">�F�P�t���[���̏c��</param>
	/// <param name="_loop">�F�I�������J��Ԃ���</param>
	/// <param name="_frame">�F�S�t���[����</param>
	/// <param name="_endDraw">�F�I����Ă��`������邩</param>
	void Initialize(std::string _fileName, int _x, int _y, int _w, int _h, bool _loop, int _frame, bool _endDraw);
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// �A�j���[�V�������n�߂�֐�
	/// </summary>
	/// <param name="_nowFrame">�A�j���[�V�������n�߂�t���[��</param>
	void Start(int _nowFrame);
	void Start();
	void ChangeReverse();
	bool NowReverse();
	void Stop();
	void SetSpeed(float _speed);
	void SetPosition(XMFLOAT3 _pos);

	int GetFrame() { return nowFrame_; }
};

