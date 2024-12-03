#pragma once

class TrackerUI
{
public:
  TrackerUI(EntryData* entry);

  void Render();
private:
  EntryData* Entry;
  int removedIndex = -1;
};