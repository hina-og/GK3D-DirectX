#pragma once
#include "Engine/GameObject.h"
#include "SelectStage.h"
#include "Button.h"

class TitleScene :
    public GameObject
{
    int hPict_;

    bool canStart_;

    SelectStage* selectStage_;
public:
    TitleScene(GameObject* parent);
    void Initialize();
    void Update();
    void Draw();
    void Release();
};

