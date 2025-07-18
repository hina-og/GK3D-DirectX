#pragma once
#include "Engine/GameObject.h"
#include "SelectStage.h"
#include "Button.h"
#include "Engine/SceneManager.h"
#include "Engine/Text.h"

class TitleScene :
    public GameObject
{
    //�n���h��
    int hBackGround_;//�w�i
    int hMovie_;//����
    int hOperation_;//����
    int hLogo_;//���S

    bool canSelect_;//�X�e�[�W�I�����ł����Ԃ��i���S���o�Ă���Ԃ�false�j

    SelectStage* selectStage_;
    SceneManager* pSM;

    Text* buttonText_;//�n�߂�{�^���̃e�L�X�g
public:
    TitleScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};