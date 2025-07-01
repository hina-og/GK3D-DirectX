#include "Animation.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"
#include "Engine/CsvReader.h"

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
	doReverse_ = false;

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
	doReverse_ = false;
	Image::SetRect(hPict_, 0, 0, rect_.top, rect_.bottom);
}

void Animation::Initialize(int _dataNumber)
{
	CsvReader csv;
	csv.Load("ImageData\\AnimationData.csv");

	hPict_ = Image::Load("Image\\" + csv.GetString(_dataNumber, NAME) + "Anim.png");
	x_ = csv.GetInt(_dataNumber, X);
	y_ = csv.GetInt(_dataNumber, Y);
	rect_ = { csv.GetInt(_dataNumber,W),csv.GetInt(_dataNumber,H) };
	doLoop_ = csv.GetInt(_dataNumber, LOOP);
	totalFrame_ = csv.GetInt(_dataNumber, FRAME);

	speed_ = csv.GetFloat(_dataNumber, SPEED);
	addSpeed_ = 0;
	doStart_ = false;
	isDrawAfterAnimation = csv.GetInt(_dataNumber, END_DRAW);
	doReverse_ = false;
	Image::SetRect(hPict_, 0, 0, rect_.top, rect_.bottom);
}

void Animation::Update()
{
	if (!doReverse_)
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
	}
	else
	{
		if (nowFrame_ == 0)
		{
			if (doLoop_)
			{
				nowFrame_ = totalFrame_;
			}
			else
			{
				doStart_ = false;
			}
		}
	}

	Image::SetRect(hPict_, rect_.left * nowFrame_, 0, rect_.left, rect_.top);

	if (doStart_)
	{
		if (addSpeed_ >= 1.0)
		{
			addSpeed_ -= 1.0;
			if (doReverse_)
				nowFrame_--;
			else
				nowFrame_++;
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
	if (!doReverse_)
		nowFrame_ = 0;
	else
		nowFrame_ = totalFrame_;
	doStart_ = true;
}

void Animation::ChangeReverse()
{
	doReverse_ = !doReverse_;
}

bool Animation::NowReverse()
{
	return doReverse_;
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
