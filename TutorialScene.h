#pragma once
#include "Engine/GameObject.h"

class TutorialScene :
    public GameObject
{
    int hMovie_;
public:
    TutorialScene(GameObject* parent);
    void Initialize();
    void Update();
    void Draw();
    void Release();
};
