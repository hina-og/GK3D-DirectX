#include "Pot.h"
#include "Image.h"
#include "Engine/Input.h"

Pot::Pot(GameObject* parent)
	: GameObject(parent, "Pot")
{
}

void Pot::Initialize()
{
	hPict_ = Image::Load("Assets\\Model\\ZombieTex.png");
	assert(hPict_ >= 0);
}

void Pot::Update()
{
	if (Input::IsKey(DIK_A))
	{
		transform_.position_.x -= 0.01;
	}
	if (Input::IsKey(DIK_D))
	{
		transform_.position_.x += 0.01;
	}
	if (Input::IsKey(DIK_S))
	{
		transform_.position_.y -= 0.01;
	}
	if (Input::IsKey(DIK_W))
	{
		transform_.position_.y += 0.01;
	}
}

void Pot::Draw()
{
	Transform pictTrans;
	//transform_.position_ = { 0.0f / 1280.0f,400.0f / 720.0f,0.0f };
	transform_.scale_ = { 0.1,0.1,0.1 };
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);

}

void Pot::Release()
{
}
