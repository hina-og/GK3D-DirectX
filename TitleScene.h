#pragma once
#include "Engine/GameObject.h"
#include "SelectStage.h"
#include "Button.h"
#include "Engine/SceneManager.h"
#include "Engine/Text.h"

class TitleScene :
    public GameObject
{
    //ハンドル
    int hBackGround_;//背景
    int hMovie_;//動画
    int hOperation_;//操作
    int hLogo_;//ロゴ

    bool canSelect_;//ステージ選択ができる状態か（ロゴが出ている間はfalse）

    SelectStage* selectStage_;
    SceneManager* pSM;

    Text* buttonText_;//始めるボタンのテキスト
public:
    TitleScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};