#include "pch.h"

ItemDataModule::ItemDataModule(EntryData* entry)
{
  Entry = entry;
  UpdateTimer.SetNow();
}

void ItemDataModule::Update()
{
  if (UpdateTimer.GetSecondsPassed() > 2.0f)
  {
    UpdateTimer.SetNow();
    SyncItems();
  }
}

bool ItemDataModule::RequestItemData(unsigned int id, ItemData& data)
{
  auto it = Items.find(id);
  if (it != Items.end())
  {
    data = it->second;
    return true;
  }
  
  if (std::find(QueuedIDs.begin(), QueuedIDs.end(), id) == QueuedIDs.end())
  {
    QueuedIDs.push_back(id);
  }
  return false;
}

void ItemDataModule::SyncItems()
{
  if (QueuedIDs.size() > 0)
  {
    std::string ids = "?ids=";
    for (int i = 0; i < QueuedIDs.size(); ++i)
    {
      if (i != 0)
        ids += ",";

      ids += std::to_string(QueuedIDs[i]);
    }

    std::string payload = GW2API::Request(Entry, API_ITEMS, ids);

    if (!payload.empty())
    {
      nlohmann::json Json = nlohmann::json::parse(payload);
      for (const auto& item_json : Json)
      {
        ItemData item = item_json.get<ItemData>();
        Items[item.ID] = item;

        //request a texture
        std::pair<std::string, std::string> splitURL = HTTPClient::SplitRemoteFromEndpoint(item.IconUrl);
        Entry->APIDefs->LoadTextureFromURL(item.TextureID.c_str(), splitURL.first.c_str(), splitURL.second.c_str(), nullptr);
      }
    }

    QueuedIDs.clear();
  }
}
