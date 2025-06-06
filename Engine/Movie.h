#pragma once

#include <vector>
#include <string>
#include <dshow.h>
#include <wrl/client.h> // Microsoft::WRL::ComPtr���g���ꍇ
#include "Transform.h"

//-----------------------------------------------------------
// ������Ǘ�����
//-----------------------------------------------------------
namespace Movie
{
    // ������
    struct MovieData
    {
        std::string fileName;

        // DirectShow�̃C���^�[�t�F�C�X
        Microsoft::WRL::ComPtr<IGraphBuilder> pGraph;
        Microsoft::WRL::ComPtr<IMediaControl> pControl;
        Microsoft::WRL::ComPtr<IMediaEvent> pEvent;
        Microsoft::WRL::ComPtr<IVideoWindow> pWindow;

        // �؂蔲���͈́i�K�v�Ȃ�j
        RECT rect;

        // �A���t�@�i�����I�ɓ��ߏ�������Ȃ�j
        float alpha;

        // �ʒu�E�X�P�[���Ȃǂ̕ϊ����
        Transform transform;

        MovieData() : alpha(1.0f), rect{ 0,0,0,0 }
        {
        }
    };

    // �������iCOM�������Ȃǁj
    void Initialize();

    // �E�B���h�E�n���h�����Z�b�g�i����\����E�B���h�E�j
    void SetVideoWindowHandle(HWND hwnd);

    // ��������[�h�i�t�@�C�������w��j
    // �����t�@�C���������łɃ��[�h�ς݂Ȃ炻�̔ԍ���Ԃ�
    int Load(const std::string& fileName);

    // ����̍Đ��J�n
    void Play(int handle);

    // ����̒�~
    void Stop(int handle);

    // ��������
    void Release(int handle);

    // �S�Ă̓�������
    void AllRelease();

    // ����̈ʒu�ݒ�
    void SetPosition(int handle, XMFLOAT3 pos);

    // ����̃X�P�[���ݒ�
    void SetScale(int handle, XMFLOAT3 scale);

    // ���[���h�s��̐ݒ�
    void SetTransform(int handle, Transform& transform);

    // ���[���h�s��̎擾
    XMMATRIX GetMatrix(int handle);

    // ����̈ʒu�擾
    XMFLOAT3 GetMoviePosition(int handle);
}
