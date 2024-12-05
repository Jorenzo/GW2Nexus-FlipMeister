#pragma once

class CompletedTrackerUI
{
public:
  CompletedTrackerUI(EntryData* entry);

  void                        Open();
  void                        Render();
private:
  EntryData*                  Entry = nullptr;
  int                         RemovedIndex = -1;
  bool                        Visible = false;
};