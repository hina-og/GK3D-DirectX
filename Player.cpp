#include "Player.h"

Player::Player(GameObject* parent)
	: GameObject(parent, "Player")
{
}

void Player::Initialize()
{
	unit_ = Instantiate<Unit>(this);
}

void Player::Update()
{
}

void Player::Draw()
{
}

void Player::Release()
{
}


