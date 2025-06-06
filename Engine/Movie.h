#pragma once

#include <vector>
#include <string>
#include <dshow.h>
#include <wrl/client.h> // Microsoft::WRL::ComPtrを使う場合
#include "Transform.h"

//-----------------------------------------------------------
// 動画を管理する
//-----------------------------------------------------------
namespace Movie
{
    // 動画情報
    struct MovieData
    {
        std::string fileName;

        // DirectShowのインターフェイス
        Microsoft::WRL::ComPtr<IGraphBuilder> pGraph;
        Microsoft::WRL::ComPtr<IMediaControl> pControl;
        Microsoft::WRL::ComPtr<IMediaEvent> pEvent;
        Microsoft::WRL::ComPtr<IVideoWindow> pWindow;

        // 切り抜き範囲（必要なら）
        RECT rect;

        // アルファ（将来的に透過処理するなら）
        float alpha;

        // 位置・スケールなどの変換情報
        Transform transform;

        MovieData() : alpha(1.0f), rect{ 0,0,0,0 }
        {
        }
    };

    // 初期化（COM初期化など）
    void Initialize();

    // ウィンドウハンドルをセット（動画表示先ウィンドウ）
    void SetVideoWindowHandle(HWND hwnd);

    // 動画をロード（ファイル名を指定）
    // 同じファイル名がすでにロード済みならその番号を返す
    int Load(const std::string& fileName);

    // 動画の再生開始
    void Play(int handle);

    // 動画の停止
    void Stop(int handle);

    // 動画を解放
    void Release(int handle);

    // 全ての動画を解放
    void AllRelease();

    // 動画の位置設定
    void SetPosition(int handle, XMFLOAT3 pos);

    // 動画のスケール設定
    void SetScale(int handle, XMFLOAT3 scale);

    // ワールド行列の設定
    void SetTransform(int handle, Transform& transform);

    // ワールド行列の取得
    XMMATRIX GetMatrix(int handle);

    // 動画の位置取得
    XMFLOAT3 GetMoviePosition(int handle);
}
