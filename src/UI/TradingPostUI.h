#pragma once

enum TradingPostTableType
{
  TradingPostTableType_Buying,
  TradingPostTableType_Selling,
  TradingPostTableType_Bought,
  TradingPostTableType_Sold
};

class TradingPostUI
{
public:
  TradingPostUI(Addon* addon);
  void                        Open();
  void                        Render();
private:
  void                        RenderTransactionsTable(const std::vector<TransactionData>* transactions, TradingPostTableType type);
  Addon*                      FAddon = nullptr;
  bool                        Visible = false;
  Timer                       BuyingRefreshTimer;
  Timer                       SellingRefreshTimer;
  Timer                       BoughtRefreshTimer;
  Timer                       SoldRefreshTimer;
};