#include "pch.h"

Timer::Timer()
{
  SetNow();
}

void Timer::SetNow()
{
  StartTime = std::chrono::high_resolution_clock::now();
}

double Timer::GetSecondsPassed()
{
  return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - StartTime).count();
}

double Timer::GetMiliSecondsPassed()
{
  return GetSecondsPassed() * 1000.0;
}