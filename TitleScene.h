#pragma once
#include "Engine/GameObject.h"
#include "Button.h"
class TitleScene :
    public GameObject
{
    int hPict_;
    Button easy_;
    Button normal_;
    Button hard_;
public:
    TitleScene(GameObject* parent);
    void Initialize();
    void Update();
    void Draw();
    void Release();
};

