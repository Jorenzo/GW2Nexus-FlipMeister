#pragma once

struct ItemData
{
  unsigned int ID;
  std::string Name;
  std::string IconUrl;
  std::string TextureID;
};
inline void to_json(nlohmann::json& j, const ItemData& item) {
  j = nlohmann::json{
    { "id", item.ID },
  { "name", item.Name },
  { "icon", item.IconUrl },
  };
}
inline void from_json(const nlohmann::json& j, ItemData& item) {
  j.at("id").get_to(item.ID);
  j.at("name").get_to(item.Name);
  j.at("icon").get_to(item.IconUrl);

  item.TextureID = "TEX_";
  item.TextureID += std::to_string(item.ID);
}

class ItemDataModule
{
public:
  ItemDataModule(EntryData* entry);
  bool                                    RequestItemData(unsigned int id, ItemData& item);
  void                                    Update();
private:
  void                                    SyncItems();
  std::map<unsigned int, ItemData>        Items;
  std::vector<unsigned int>               QueuedIDs;
  EntryData*                              Entry;
  Timer                                   UpdateTimer;
};