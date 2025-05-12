#include "Time.h"

namespace Time
{
    // íËã`ÇÕÇ±Ç±Åi1âÒÇæÇØÅj
    LARGE_INTEGER frequency;
    LARGE_INTEGER lastTime;
    LARGE_INTEGER startTime;
    float deltaTime;
    bool initialized;

    void Time::Initialize()
    {
        deltaTime = 0.0f;
        initialized = false;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&startTime);
        lastTime = startTime;
    }

    void Time::Update()
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        if (!initialized)
        {
            QueryPerformanceFrequency(&frequency);
            startTime = currentTime;
            lastTime = currentTime;
            initialized = true;
        }

        deltaTime = static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
        lastTime = currentTime;
    }

    float Time::GetDeltaTime()
    {
        return deltaTime;
    }

    float Time::Elapsed()
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return static_cast<float>(currentTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
    }
}