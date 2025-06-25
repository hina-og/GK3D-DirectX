#pragma once
#include "Engine/GameObject.h"
#include "SelectStage.h"
#include "Button.h"
#include "Engine/SceneManager.h"

class TitleScene :
    public GameObject
{
    int hPict_;
    int hMovie_;
    int hOperationButton_;

    SelectStage* selectStage_;

    SceneManager* pSM;
public:
    TitleScene(GameObject* parent);
    void Initialize();
    void Update();
    void Draw();
    void Release();
};