#pragma once

class TradingPostUI
{
public:
  TradingPostUI(EntryData* entry);
  void                        Open();
  void                        Render();
private:
  EntryData*                  Entry = nullptr;
  bool                        Visible = false;
};