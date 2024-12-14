#pragma once

class TradingPostUI
{
public:
  TradingPostUI(Addon* addon);
  void                        Open();
  void                        Render();
private:
  void                        RenderTransactionsTable(const std::vector<TransactionData>* transactions);
  Addon*                      FAddon = nullptr;
  bool                        Visible = false;
  Timer                       BuyingRefreshTimer;
  Timer                       BoughtRefreshTimer;
};