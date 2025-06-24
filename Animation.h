#pragma once
#include <wtypes.h>
#include <string>
#include "Engine/GameObject.h"

class Animation
{
	float x_;
	float y_;
	RECT rect_;
	int hPict_;

	float speed_;
	float addSpeed_;

	bool doLoop_;
	bool doReverse_;
	bool doStart_;

	int nowFrame_;//今のフレーム
	int totalFrame_;//全アニメーションフレーム

	bool isDrawAfterAnimation;//アニメーションが終わっても描画をするか
public:
	Animation();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_fileName">：画像のパス</param>
	/// <param name="_x">：描画するX座標</param>
	/// <param name="_y">：描画するY座標</param>
	/// <param name="_w">：１フレームの横幅</param>
	/// <param name="_h">：１フレームの縦幅</param>
	/// <param name="_loop">：終わったら繰り返すか</param>
	/// <param name="_frame">：全フレーム数</param>
	/// <param name="_endDraw">：終わっても描画をするか</param>
	Animation(std::string _fileName,int _x,int _y,int _w,int _h,bool _loop,int _frame,bool _endDraw);
	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	/// <param name="_fileName">：画像のパス</param>
	/// <param name="_x">：描画するX座標</param>
	/// <param name="_y">：描画するY座標</param>
	/// <param name="_w">：１フレームの横幅</param>
	/// <param name="_h">：１フレームの縦幅</param>
	/// <param name="_loop">：終わったら繰り返すか</param>
	/// <param name="_frame">：全フレーム数</param>
	/// <param name="_endDraw">：終わっても描画をするか</param>
	void Initialize(std::string _fileName, int _x, int _y, int _w, int _h, bool _loop, int _frame, bool _endDraw);
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// アニメーションを始める関数
	/// </summary>
	/// <param name="_nowFrame">アニメーションを始めるフレーム</param>
	void Start(int _nowFrame);
	void Start();
	void ChangeReverse();
	bool NowReverse();
	void Stop();
	void SetSpeed(float _speed);
	void SetPosition(XMFLOAT3 _pos);

	int GetFrame() { return nowFrame_; }
};

