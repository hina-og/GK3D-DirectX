#include "Movie.h"
#include <dshow.h>
#include <wrl/client.h>
#include <iostream>

using Microsoft::WRL::ComPtr;
#pragma comment(lib, "Strmiids.lib")

namespace Movie
{
    //ウィンドウハンドル
    HWND	hWnd_;

    //ロード済みの動画データ一覧
    std::vector<MovieData*> _datas;

    //COM初期化済みフラグ
    static bool g_isCOMInitialized = false;

    void Initialize()
    {
        AllRelease();

        if (!g_isCOMInitialized)
        {
            HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
            if (FAILED(hr))
            {
                std::cerr << "COM初期化失敗\n";
                return;
            }
            g_isCOMInitialized = true;
        }
    }

    void SetVideoWindowHandle(HWND hwnd)
    {
        hWnd_ = hwnd;
    }

    int Load(const std::string& fileName)
    {
        //既に読み込み済みがないか探す
        for (int i = 0; i < _datas.size(); i++)
        {
            if (_datas[i] != nullptr && _datas[i]->fileName == fileName)
            {
                return i; //既存のハンドルを返す
            }
        }

        //新規読み込み
        MovieData* pData = new MovieData;
        pData->fileName = fileName;

        // DirectShow Graph作成
        ComPtr<IGraphBuilder> pGraph;
        HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pGraph));
        if (FAILED(hr))
        {
            delete pData;
            return -1;
        }

        ComPtr<IMediaControl> pControl;
        hr = pGraph.As(&pControl);
        if (FAILED(hr))
        {
            delete pData;
            return -1;
        }

        ComPtr<IMediaEvent> pEvent;
        hr = pGraph.As(&pEvent);
        if (FAILED(hr))
        {
            delete pData;
            return -1;
        }

        //ファイル読み込み
        hr = pGraph->RenderFile(std::wstring(fileName.begin(), fileName.end()).c_str(), NULL);
        if (FAILED(hr))
        {
            delete pData;
            return -1;
        }


        //動画のビデオウィンドウnの設定
        hr = pGraph.As(&pData->pWindow);
        if (SUCCEEDED(hr) && pData->pWindow != nullptr && hWnd_ != nullptr)
        {
            pData->pWindow->put_Owner((OAHWND)hWnd_);
            pData->pWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

            RECT rc;
            GetClientRect(hWnd_, &rc);
            pData->pWindow->SetWindowPosition(0, 0, rc.right, rc.bottom);

            pData->pWindow->put_Visible(OATRUE);
        }

        pData->pGraph = pGraph;
        pData->pControl = pControl;
        pData->pEvent = pEvent;

        //空きスロットを探す
        for (int i = 0; i < _datas.size(); i++)
        {
            if (_datas[i] == nullptr)
            {
                _datas[i] = pData;
                return i;
            }
        }

        //追加する
        _datas.push_back(pData);
        return (int)(_datas.size() - 1);
    }

    void Play(int handle)
    {
        if (handle < 0 || handle >= _datas.size()) return;
        if (_datas[handle] != nullptr && _datas[handle]->pControl)
        {
            _datas[handle]->pControl->Run();
        }
    }

    void Stop(int handle)
    {
        if (handle < 0 || handle >= _datas.size()) return;
        if (_datas[handle] != nullptr && _datas[handle]->pControl)
        {
            _datas[handle]->pControl->Stop();
        }
    }

    bool Movie::IsPlaying(int handle)
    {
        if (handle < 0 || handle >= _datas.size()) return false;
        if (_datas[handle] == nullptr || !_datas[handle]->pControl) return false;

        OAFilterState state;
        if (SUCCEEDED(_datas[handle]->pControl->GetState(0, &state)))
        {
            return state == State_Running;
        }
        return false;
    }

    bool Movie::IsEnded(int handle)
    {
        if (handle < 0 || handle >= _datas.size()) return true;
        if (_datas[handle] == nullptr || !_datas[handle]->pEvent) return true;

        long evCode = 0;
        if (SUCCEEDED(_datas[handle]->pEvent->WaitForCompletion(1, &evCode)))
        {
            return evCode == EC_COMPLETE;
        }
        return false;
    }

    bool Movie::IsAnyMoviePlaying()
    {
        for (auto* data : _datas)
        {
            if (data && data->pControl)
            {
                OAFilterState state;
                if (SUCCEEDED(data->pControl->GetState(0, &state)) && state == State_Running)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void Release(int handle)
    {
        if (handle < 0 || handle >= _datas.size()) return;
        if (_datas[handle] == nullptr) return;

        //動画の停止
        if (_datas[handle]->pControl)
        {
            _datas[handle]->pControl->Stop();
        }

        _datas[handle]->pGraph.Reset();
        _datas[handle]->pControl.Reset();
        _datas[handle]->pEvent.Reset();

        delete _datas[handle];
        _datas[handle] = nullptr;
    }

    void AllRelease()
    {
        for (int i = 0; i < _datas.size(); i++)
        {
            Release(i);
        }
        _datas.clear();

        if (g_isCOMInitialized)
        {
            CoUninitialize();
            g_isCOMInitialized = false;
        }
    }

    void SetPosition(int handle, XMFLOAT3 pos)
    {
        if (handle < 0 || handle >= _datas.size()) return;
        if (_datas[handle] != nullptr)
        {
            _datas[handle]->transform.position_ = pos;
        }
    }

    void SetScale(int handle, XMFLOAT3 scale)
    {
        if (handle < 0 || handle >= _datas.size()) return;
        if (_datas[handle] != nullptr)
        {
            _datas[handle]->transform.scale_ = scale;
        }
    }

    void SetTransform(int handle, Transform& transform)
    {
        if (handle < 0 || handle >= _datas.size()) return;
        if (_datas[handle] != nullptr)
        {
            _datas[handle]->transform = transform;
        }
    }

    XMMATRIX GetMatrix(int handle)
    {
        if (handle < 0 || handle >= _datas.size()) return XMMatrixIdentity();
        if (_datas[handle] == nullptr) return XMMatrixIdentity();

        return _datas[handle]->transform.GetWorldMatrix();
    }

    XMFLOAT3 GetMoviePosition(int handle)
    {
        if (handle < 0 || handle >= _datas.size()) return XMFLOAT3(0, 0, 0);
        if (_datas[handle] == nullptr) return XMFLOAT3(0, 0, 0);

        return _datas[handle]->transform.position_;
    }
}