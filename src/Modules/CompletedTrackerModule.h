#pragma once

struct CompletedItem
{
  unsigned int ItemID = 0;
  unsigned int Quantity = 0;
  int BuyPrice = 0;
  int SellPrice = 0;

  bool operator==(const CompletedItem& other) const 
  {
    return ItemID == other.ItemID && Quantity == other.Quantity && BuyPrice == other.BuyPrice && SellPrice == other.SellPrice;
  }
};
inline void to_json(nlohmann::json& j, const CompletedItem& item)
{
  j = nlohmann::json
  {
    { "item_id", item.ItemID },
    { "quantity", item.Quantity },
    { "buy_price", item.BuyPrice },
    { "sell_price", item.SellPrice },
  };
}
inline void from_json(const nlohmann::json& j, CompletedItem& item)
{
  j.at("item_id").get_to(item.ItemID);
  j.at("quantity").get_to(item.Quantity);
  j.at("buy_price").get_to(item.BuyPrice);
  j.at("sell_price").get_to(item.SellPrice);
}

class CompletedTrackerModule
{
public:
  CompletedTrackerModule(EntryData* entry);
  const std::vector<CompletedItem>* GetCompletedItems() const { return &CompletedItems; }
  void AddCompletedItem(const CompletedItem& item);
  void EditItem(int index, const CompletedItem& item);
  void RemoveCompletedItem(int index);
private:
  void ReadFromFile();
  void WriteToFile();
  EntryData*                Entry;
  std::vector<CompletedItem>  CompletedItems;
};