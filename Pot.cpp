#include "Pot.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

Pot::Pot(GameObject* parent)
	: GameObject(parent, "Pot")
{
}

void Pot::Initialize()
{
	hPict_ = Image::Load("Image\\pot.png");
	assert(hPict_ >= 0);
}

void Pot::Update()
{
}

void Pot::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void Pot::Release()
{
}
