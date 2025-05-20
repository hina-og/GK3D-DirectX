#pragma once
#include "Engine/GameObject.h"

enum class Difficulty
{
    Easy,
    Normal,
    Hard,
    Despair
};

class SetUp
{
public:
    static Difficulty currentDifficulty;
};

