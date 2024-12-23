#pragma once

struct DeliveryItemData
{
  unsigned int ItemID = 0;
  unsigned int Quantity = 0;
};
inline void to_json(nlohmann::json& j, const DeliveryItemData& item) 
{
  j = nlohmann::json
  {
    { "id", item.ItemID },
    { "count", item.Quantity },
  };
}
inline void from_json(const nlohmann::json& j, DeliveryItemData& item) 
{
  j.at("id").get_to(item.ItemID);
  j.at("count").get_to(item.Quantity);
}

struct DeliveryData
{
  int Coins = 0;
  std::vector<DeliveryItemData> Items;
};

inline void to_json(nlohmann::json& j, const DeliveryData& delivery) 
{
  j = nlohmann::json
  {
    { "coins", delivery.Coins },
    { "items", delivery.Items },
  };
}
inline void from_json(const nlohmann::json& j, DeliveryData& delivery) 
{
  j.at("coins").get_to(delivery.Coins);
  j.at("items").get_to(delivery.Items);
}

struct TransactionData
{
  unsigned int ItemID = 0;
  int Price = 0;
  unsigned int Quantity = 0;
};
inline void to_json(nlohmann::json& j, const TransactionData& transaction) 
{
  j = nlohmann::json
  {
    { "item_id", transaction.ItemID },
    { "price", transaction.Price },
    { "quantity", transaction.Quantity },
  };
}
inline void from_json(const nlohmann::json& j, TransactionData& transaction) 
{
  if(j.contains("item_id"))
    j.at("item_id").get_to(transaction.ItemID);
  if(j.contains("price"))
    j.at("price").get_to(transaction.Price);
  if(j.contains("quantity"))
    j.at("quantity").get_to(transaction.Quantity);
}

struct PriceObject
{
  int UnitPrice = 0;
};
inline void to_json(nlohmann::json& j, const PriceObject& price) 
{
  j = nlohmann::json
  {
    { "unit_price", price.UnitPrice },
  };
}
inline void from_json(const nlohmann::json& j, PriceObject& price) 
{
  j.at("unit_price").get_to(price.UnitPrice);
}

struct PriceData
{
  unsigned int ItemID = 0;
  PriceObject Sells = {};
  PriceObject Buys = {};
};
inline void to_json(nlohmann::json& j, const PriceData& price) 
{
  j = nlohmann::json
  {
    { "id", price.ItemID },
    { "sells", price.Sells },
    { "buys", price.Buys },
  };
}
inline void from_json(const nlohmann::json& j, PriceData& price) 
{
  j.at("id").get_to(price.ItemID);
  if(j.contains("sells"))
    j.at("sells").get_to(price.Sells);
  if(j.contains("buys"))
    j.at("buys").get_to(price.Buys);
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
  const DeliveryData*                     GetDeliveryData() const { return &Delivery; }
  int                                     GetSellPrice(unsigned int id);
  void                                    PullCurrentBuys();
  void                                    PullCurrentSells();
  void                                    PullHistoryBuys();
  void                                    PullHistorySells();
  void                                    PullDelivery();
  bool                                    IsUpdatingPrices() const { return SyncPricesHandle != HTTPREQUEST_HANDLE_INVALID || NewItemsInPriceWatch; }
  bool                                    IsUpdatingCurrenBuys() const { return SyncCurrentBuysHandle != HTTPREQUEST_HANDLE_INVALID; }
  bool                                    IsUpdatingCurrenSells() const { return SyncCurrentSellsHandle != HTTPREQUEST_HANDLE_INVALID; }
  bool                                    IsUpdatingHistoryBuys() const { return SyncHistoryBuysHandle != HTTPREQUEST_HANDLE_INVALID; }
  bool                                    IsUpdatingHistorySells() const { return SyncHistorySellsHandle != HTTPREQUEST_HANDLE_INVALID; }
  bool                                    IsUpdatingDelivery() const { return SyncDeliveryHandle != HTTPREQUEST_HANDLE_INVALID; }
private:
  void                                    RequestSyncPrices();
  void                                    TrySyncPrices();
  void                                    TrySyncCurrentBuys();
  void                                    TrySyncCurrentSells();
  void                                    TrySyncHistoryBuys();
  void                                    TrySyncHistorySells();
  void                                    TrySyncDelivery();
  std::vector<TransactionData>            CurrentBuys;
  std::vector<TransactionData>            CurrentSells;
  std::vector<TransactionData>            HistoryBuys;
  std::vector<TransactionData>            HistorySells;
  std::map<unsigned int, PriceData>       CurrentPrices;
  std::vector<unsigned int>               PriceWatch;
  DeliveryData                            Delivery;
  Addon*                                  FAddon = nullptr;
  Timer                                   PricesTimer;
  HTTPRequestHandle                       SyncCurrentBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncCurrentSellsHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncHistoryBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncHistorySellsHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncPricesHandle = HTTPREQUEST_HANDLE_INVALID;
  HTTPRequestHandle                       SyncDeliveryHandle = HTTPREQUEST_HANDLE_INVALID;
  bool                                    NewItemsInPriceWatch = false;
  unsigned int                            SyncHistoryBuysPageIndex = 0;
  unsigned int                            SyncHistorySellsPageIndex = 0;
  unsigned int                            SyncCurrentBuysPageIndex = 0;
  unsigned int                            SyncCurrentSellsPageIndex = 0;
};