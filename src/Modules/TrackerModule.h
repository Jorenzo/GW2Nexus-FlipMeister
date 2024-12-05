#pragma once

struct TrackedItem
{
  unsigned int ItemID = 0;
  unsigned int Quantity = 0;
  int BuyPrice = 0;

  bool operator==(const TrackedItem& other) const 
  {
    return ItemID == other.ItemID && Quantity == other.Quantity && BuyPrice == other.BuyPrice;
  }
};
inline void to_json(nlohmann::json& j, const TrackedItem& item) {
  j = nlohmann::json
  {
    { "item_id", item.ItemID },
    { "quantity", item.Quantity },
    { "buy_price", item.BuyPrice },
  };
}
inline void from_json(const nlohmann::json& j, TrackedItem& item) {
  j.at("item_id").get_to(item.ItemID);
  j.at("quantity").get_to(item.Quantity);
  j.at("buy_price").get_to(item.BuyPrice);
}

class TrackerModule
{
public:
  TrackerModule(EntryData* entry);
  const std::vector<TrackedItem>* GetTrackedItems() const { return &TrackedItems; }
  void TrackItem(const TrackedItem& item);
  void EditItem(int index, const TrackedItem& item);
  void RemoveTrackedItem(int index);
private:
  void ReadFromFile();
  void WriteToFile();
  EntryData*                Entry;
  std::vector<TrackedItem>  TrackedItems;
};