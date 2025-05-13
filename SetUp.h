#pragma once
#include "Engine/GameObject.h"

enum class Difficulty
{
    Easy,
    Normal,
    Hard
};

class SetUp
{
public:
    static Difficulty currentDifficulty;
};

