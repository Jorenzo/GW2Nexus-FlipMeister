#pragma once

class CompletedTrackerUI
{
public:
  CompletedTrackerUI(Addon* addon);

  void                        Open();
  void                        Render();
private:
  Addon*                      FAddon = nullptr;
  int                         RemovedIndex = -1;
};