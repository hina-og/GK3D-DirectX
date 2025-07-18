#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"
#include "Mushroom.h"
#include "Slime.h"
#include "Golem.h"
#include "Ghost.h"

//�v���C���[��G�ɒǉ�����N���X
class Unit
	: public GameObject
{
	//�L�����̔z��
	std::vector<Puppet*> puppet_;
	//�J�������h��鋭��
	int shakePower_;
	//�J�������h��鎞�ԁi�~���b�j
	float shakeTime_;
public:
	Unit();
	Unit(GameObject* parent);
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�J��
	void Release();
	/// <summary>
	/// �L�����̒ǉ�
	/// </summary>
	/// <param name="_pos">�L�����̈ʒu</param>
	/// <param name="_type">�L�����̎��</param>
	/// <param name="_dir">�L�����̌���</param>
	void AddCharacter(XMFLOAT3 _pos, int _type, Puppet::DIRECTION _dir);
	/// <summary>
	/// �L�����̒ǉ�
	/// </summary>
	/// <param name="_pos">�L�����̈ʒu</param>
	/// <param name="_type">�L�����̎��</param>
	/// <param name="_dir">�L�����̌���</param>
	void AddCharacter(XMFLOAT3 _pos, int _type, int _dir);
	//�L�����̔z��̃Q�b�^�[
	std::vector<Puppet*> GetPuppetArray();
	//�ォ���O�ɐN�����Ă��铮��()
	void InvaderMove();
	//�U���͈͂ɓ������Ƃ��̏���
	void InRange(std::vector<Puppet*> _puppet);
	//�ǂ܂œ��B�����Ƃ��̏���
	void PastLine(float _z, int& _durability);
};

