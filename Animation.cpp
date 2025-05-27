#include "Animation.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

Animation::Animation()
{
}

Animation::Animation(std::string _fileName, int _x, int _y, int _w, int _h, bool _loop, int _frame, bool _endDraw)
{
	hPict_ = Image::Load(_fileName);
	x_ = _x;
	y_ = _y;
	rect_ = { _w,_h };
	doLoop_ = _loop;
	totalFrame_ = _frame;

	speed_ = 1.0f;
	addSpeed_ = 0;
	doStart_ = false;
	isDrawAfterAnimation = _endDraw;

	Image::SetRect(hPict_, 0, 0, rect_.top, rect_.bottom);
}

void Animation::Initialize(std::string _fileName, int _x, int _y, int _w, int _h, bool _loop, int _frame, bool _endDraw)
{
	hPict_ = Image::Load(_fileName);
	x_ = _x;
	y_ = _y;
	rect_ = { _w,_h };
	doLoop_ = _loop;
	totalFrame_ = _frame;

	speed_ = 1.0f;
	addSpeed_ = 0;
	doStart_ = false;
	isDrawAfterAnimation = _endDraw;

	Image::SetRect(hPict_, 0, 0, rect_.top, rect_.bottom);
}

void Animation::Update()
{
	if (nowFrame_ == totalFrame_)
	{
		if (doLoop_)
		{
			nowFrame_ = 0;
		}
		else
		{
			doStart_ = false;
		}
	}

	Image::SetRect(hPict_, rect_.left * nowFrame_, 0, rect_.top, rect_.left);


	if(doStart_)
	{
		if (addSpeed_ >= 1.0)
		{
			addSpeed_ -= 1.0;
			nowFrame_ += 1;
		}
		addSpeed_ += speed_;
	}
}

void Animation::Draw()
{
	if(doStart_)
	{
		Image::SetPosition(hPict_, { (float)(x_ / Direct3D::screenWidth_),(float)-(y_ / Direct3D::screenHeight_),0 });
		Image::Draw(hPict_);
	}
}

void Animation::Release()
{
}

void Animation::Start(int _nowFrame)
{
	nowFrame_ = _nowFrame;
	doStart_ = true;
}

void Animation::Start()
{
	nowFrame_ = 0;
	doStart_ = true;
}

void Animation::Stop()
{
	doStart_ = false;
}

void Animation::SetSpeed(float _speed)
{
	speed_ = _speed;
}

void Animation::SetPosition(XMFLOAT3 _pos)
{
	x_ = _pos.x;
	y_ = _pos.y;
}
