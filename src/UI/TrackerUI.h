#pragma once

class TrackerUI
{
public:
  TrackerUI(Addon* addon);

  void                    Render();
private:
  Addon *                 FAddon = nullptr;
  int                     RemovedIndex = -1;
};