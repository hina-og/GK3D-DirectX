#include "Enemy.h"
#include "Engine/Input.h"

Enemy::Enemy(GameObject* parent)
	: GameObject(parent, "Enemy")
{
}

void Enemy::Initialize()
{
	unit_ = Instantiate<Unit>(this);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}

void Enemy::Release()
{
}

