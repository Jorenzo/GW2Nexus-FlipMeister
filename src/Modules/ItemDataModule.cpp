#include "pch.h"

ItemDataModule::ItemDataModule(Addon* addon)
{
  FAddon = addon;
  UpdateTimer.SetNow();
}

void ItemDataModule::Update()
{
  if (UpdateTimer.GetSecondsPassed() > 2.0f && SyncItemsRequestHandle == HTTPREQUEST_HANDLE_INVALID)
  {
    UpdateTimer.SetNow();
    RequestSyncItems();
  }
  if (SyncItemsRequestHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncItems();
  }
}

bool ItemDataModule::RequestItemData(unsigned int id, ItemData& data)
{
  auto it = Items.find(id);
  if (it != Items.end())
  {
    if (it->second.first) //is a valid item
    {
      data = it->second.second;
      return true;
    }
    return false;
  }
  
  if (std::find(QueuedIDs.begin(), QueuedIDs.end(), id) == QueuedIDs.end())
  {
    QueuedIDs.push_back(id);
  }
  return false;
}

void ItemDataModule::TrySyncItems()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, SyncItemsRequestHandle, payload))
  {
    if (!payload.empty())
    {
      nlohmann::json Json = nlohmann::json::parse(payload);
      for (const auto& item_json : Json)
      {
        try
        {
          ItemData item = item_json.get<ItemData>();
          Items[item.ID] = std::pair<bool, ItemData>(true, item);

          //request a texture
          std::pair<std::string, std::string> splitURL = HTTPClient::SplitRemoteFromEndpoint(item.IconUrl);
          FAddon->GetAPI()->LoadTextureFromURL(item.TextureID.c_str(), splitURL.first.c_str(), splitURL.second.c_str(), nullptr);
        }
        catch (const nlohmann::json::exception& e)
        {
          FAddon->Log(WARNING, "Can not create ItemData: %s", e.what());
        }
      }

      //validate if we have the items now
      for (int i = 0; i < ProcessedIDs.size(); ++i)
      {
        auto it = Items.find(ProcessedIDs[i]);
        if (it == Items.end())
        {
          //item not in map, add an invalid item so we don't request it again
          Items[ProcessedIDs[i]] = std::pair<bool, ItemData>(false, ItemData());
        }
      }
      ProcessedIDs.clear();
    }
    else
      FAddon->Log(WARNING, "Found empty ItemData payload");

    SyncItemsRequestHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}

void ItemDataModule::RequestSyncItems()
{
  //If for some reason we found an empty payload, requeue these items again in the hope we find them this time.
  QueuedIDs.insert(QueuedIDs.end(), ProcessedIDs.begin(), ProcessedIDs.end());
 
  if (QueuedIDs.size() > 0 || ProcessedIDs.size())
  {
    std::string ids = "?ids=";
    for (int i = 0; i < QueuedIDs.size(); ++i)
    {
      if (i != 0)
        ids += ",";

      ids += std::to_string(QueuedIDs[i]);
    }

    SyncItemsRequestHandle = GW2API::Request(FAddon, API_ITEMS, ids);

    ProcessedIDs = QueuedIDs;
    QueuedIDs.clear();
  }
}
