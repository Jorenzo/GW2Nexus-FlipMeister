#pragma once

class TradingPostUI
{
public:
  TradingPostUI(Addon* addon);
  void                        Open();
  void                        Render();
private:
  Addon*                      FAddon = nullptr;
  bool                        Visible = false;
};