#pragma once
#include "Engine/GameObject.h"

class SetUp
    : public GameObject
{
public:
    SetUp(GameObject* parent);
    void Initialize();
    void Update();
    void Draw();
    void Release();
};

