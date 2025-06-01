#pragma once
#include "Engine/GameObject.h"
#include "SelectStage.h"
#include "Button.h"

class TitleScene :
    public GameObject
{
    int hPict_;


    bool canStart_;

    bool isFading = false;
    float fadeTimer = 0.0f;
    const float fadeDuration = 1.0f;
    float g_alpha = 1.0f;

    SelectStage* selectStage_;
public:
    TitleScene(GameObject* parent);
    void Initialize();
    void Update();
    void Draw();
    void Release();
};

