#pragma once
#include <windows.h>

namespace Time
{
    //初期化
    void Initialize();

    //deltaTimeの更新
    void Update();

    //前フレームとの経過時間（秒）
    float GetDeltaTime();

    //アプリ開始からの経過時間（秒）
    float Elapsed();

};