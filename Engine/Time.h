#pragma once
#include <windows.h>

namespace Time
{
    //������
    void Initialize();

    //deltaTime�̍X�V
    void Update();

    //�O�t���[���Ƃ̌o�ߎ��ԁi�b�j
    float GetDeltaTime();

    //�A�v���J�n����̌o�ߎ��ԁi�b�j
    float Elapsed();

};