#pragma once

class TrackerUI
{
public:
  TrackerUI(EntryData* entry);

  void                    Render();
private:
  EntryData*              Entry = nullptr;
  int                     RemovedIndex = -1;
};