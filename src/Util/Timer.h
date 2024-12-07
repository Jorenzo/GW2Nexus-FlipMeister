#pragma once

class Timer
{
public:
  Timer();
  void SetNow();
  double GetSecondsPassed();
  double GetMiliSecondsPassed();
private:
  std::chrono::time_point<std::chrono::high_resolution_clock>     StartTime;
};