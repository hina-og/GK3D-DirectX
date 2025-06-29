#pragma once
#include <string>

//-----------------------------------------------------------
//�T�E���h���Ǘ�����iXAudio�g�p�j
//-----------------------------------------------------------
namespace Audio
{
	//������
	void Initialize();

	//�T�E���h�t�@�C��(.wav�j�����[�h
	//���łɓ������O�̃t�@�C�������[�h�ς݂̏ꍇ�́A�����̃f�[�^�̔ԍ���Ԃ�
	//�����FfileName	�t�@�C����
	//�����FisLoop		���[�v�Đ����������Ȃ�true
	//�����FsvNum�@		�����ɖ炷�ő吔�i�ȗ��j
	//�ߒl�F���̃f�[�^�Ɋ��蓖�Ă�ꂽ�ԍ�
	int Load(std::string fileName, bool isLoop = false, int svNum = 1);

	//�Đ�
	//�����FID	�炵�����T�E���h�̔ԍ�
	void Play(int ID);

	//��~
	//�����FID	�~�߂����T�E���h�̔ԍ�
	void Stop(int ID);

	//�T�E���h�����Ă��邩�i�I�����Ă�����false�j
	//�����FID ���ׂ����T�E���h�̔ԍ�
	bool IsPlaying(int ID);

	//�V�[�����Ƃ̉���i���[�h�����T�E���h������j
	void Release();

	//�{�̂����
	void AllRelease();
};

