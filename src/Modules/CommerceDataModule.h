#pragma once

struct TransactionData
{
  unsigned int ItemID = 0;
  int Price = 0;
  unsigned int Quantity = 0;
};
inline void to_json(nlohmann::json& j, const TransactionData& transaction) {
  j = nlohmann::json
  {
    { "item_id", transaction.ItemID },
    { "price", transaction.Price },
    { "quantity", transaction.Quantity },
  };
}
inline void from_json(const nlohmann::json& j, TransactionData& transaction) {
  j.at("item_id").get_to(transaction.ItemID);
  j.at("price").get_to(transaction.Price);
  j.at("quantity").get_to(transaction.Quantity);
}

struct PriceObject
{
  int UnitPrice = 0;
};
inline void to_json(nlohmann::json& j, const PriceObject& price) {
  j = nlohmann::json
  {
    { "unit_price", price.UnitPrice },
  };
}
inline void from_json(const nlohmann::json& j, PriceObject& price) {
  j.at("unit_price").get_to(price.UnitPrice);
}

struct PriceData
{
  unsigned int ItemID = 0;
  PriceObject Sells = {};
};
inline void to_json(nlohmann::json& j, const PriceData& price) {
  j = nlohmann::json
  {
    { "id", price.ItemID },
    { "sells", price.Sells },
  };
}
inline void from_json(const nlohmann::json& j, PriceData& price) {
  j.at("id").get_to(price.ItemID);
  j.at("sells").get_to(price.Sells);
}

class CommerceDataModule
{
public:
  CommerceDataModule(Addon* addon);
  void                                    Update();
  const std::vector<TransactionData>*     GetCurrentBuys() const { return &CurrentBuys; }
  const std::vector<TransactionData>*     GetCurrentSells() const { return &CurrentSells; }
  const std::vector<TransactionData>*     GetHistoryBuys() const { return &HistoryBuys; }
  const std::vector<TransactionData>*     GetHistorySells() const { return &HistorySells; }
  int                                     GetSellPrice(unsigned int id);
  void                                    PullCurrentBuys();
  void                                    PullCurrentSells();
  void                                    PullHistoryBuys();
  void                                    PullHistorySells();
  bool                                    IsUpdatingPrices() const { return SyncPricesHandle != HTTPREQUEST_HANDLE_INVALID || NewItemsInPriceWatch; }
  bool                                    IsUpdatingCurrenBuys() const { return SyncCurrentBuysHandle != HTTPREQUEST_HANDLE_INVALID; }
  bool                                    IsUpdatingCurrenSells() const { return SyncCurrentSellsHandle != HTTPREQUEST_HANDLE_INVALID; }
  bool                                    IsUpdatingHistoryBuys() const { return SyncHistoryBuysHandle != HTTPREQUEST_HANDLE_INVALID; }
  bool                                    IsUpdatingHistorySells() const { return SyncHistorySellsHandle != HTTPREQUEST_HANDLE_INVALID; }
private:
  void                                    RequestSyncPrices();
  void                                    TrySyncPrices();
  void                                    TrySyncCurrentBuys();
  void                                    TrySyncCurrentSells();
  void                                    TrySyncHistoryBuys();
  void                                    TrySyncHistorySells();
  std::vector<TransactionData>            CurrentBuys;
  std::vector<TransactionData>            CurrentSells;
  std::vector<TransactionData>            HistoryBuys;
  std::vector<TransactionData>            HistorySells;
  std::map<unsigned int, PriceData>       CurrentPrices;
  std::vector<unsigned int>               PriceWatch;
  Addon*                                  FAddon = nullptr;
  Timer                                   PricesTimer;
  HTTPRequestHandle                       SyncCurrentBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncCurrentSellsHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncHistoryBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncHistorySellsHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncPricesHandle = HTTPREQUEST_HANDLE_INVALID;
  bool                                    NewItemsInPriceWatch = false;
};