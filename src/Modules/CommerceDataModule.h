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
  const std::vector<TransactionData>*     GetCurrentBuys() const { return &CurrentBuys; };
  const std::vector<TransactionData>*     GetHistoryBuys() const { return &HistoryBuys; };
  int                                     GetSellPrice(unsigned int id);
  void                                    PullCurrentBuys();
  void                                    PullHistoryBuys();
private:
  void                                    RequestSyncPrices();
  void                                    TrySyncPrices();
  void                                    TrySyncCurrentBuys();
  void                                    TrySyncHistoryBuys();
  std::vector<TransactionData>            CurrentBuys;
  std::vector<TransactionData>            HistoryBuys;
  std::map<unsigned int, PriceData>       CurrentPrices;
  std::vector<unsigned int>               PriceWatch;
  Addon*                                  FAddon = nullptr;
  Timer                                   PricesTimer;
  HTTPRequestHandle                       SyncCurrentBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncHistoryBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncPricesHandle = HTTPREQUEST_HANDLE_INVALID;
};