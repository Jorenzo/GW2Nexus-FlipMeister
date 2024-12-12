#include "pch.h"


CommerceDataModule::CommerceDataModule(EntryData* entry)
{
  Entry = entry;
  PullCurrentBuys();
  PullHistoryBuys();
  PricesTimer.SetNow();
}

void CommerceDataModule::Update()
{
  if (PricesTimer.GetSecondsPassed() > 5.0 && SyncPricesHandle == HTTPREQUEST_HANDLE_INVALID)
  {
    RequestSyncPrices();
  }
  if (SyncPricesHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncPrices();
  }
  if (SyncHistoryBuysHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncHistoryBuys();
  }
  if (SyncCurrentBuysHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncCurrentBuys();
  }
}

void CommerceDataModule::PullCurrentBuys()
{
  if (SyncCurrentBuysHandle != HTTPREQUEST_HANDLE_INVALID)
    return;

  SyncCurrentBuysHandle = GW2API::Request(Entry, API_COMMERCE_TRANSACTIONS_CURRENT_BUYS);
}

void CommerceDataModule::PullHistoryBuys()
{
  if (SyncHistoryBuysHandle != HTTPREQUEST_HANDLE_INVALID)
    return;

  SyncHistoryBuysHandle = GW2API::Request(Entry, API_COMMERCE_TRANSACTIONS_HISTORY_BUYS);
}

void CommerceDataModule::TrySyncCurrentBuys()
{
  std::string payload = "";
  if (GW2API::GetPayload(Entry, SyncCurrentBuysHandle, payload))
  {
    if (!payload.empty())
    {
      CurrentBuys.clear();

      nlohmann::json Json = nlohmann::json::parse(payload);
      std::vector<TransactionData> data = Json.get<std::vector<TransactionData>>();

      for (int i = 0; i < data.size(); i++)
      {
        TransactionData& transaction = data[i];
        bool Merged = false;
        for (int j = 0; j < CurrentBuys.size(); j++)
        {
          TransactionData& savedTransaction = CurrentBuys[j];
          if (savedTransaction.ItemID == transaction.ItemID && savedTransaction.Price == transaction.Price)
          {
            savedTransaction.Quantity += transaction.Quantity;
            Merged = true;
            break;
          }
        }

        if (!Merged)
          CurrentBuys.push_back(transaction);
      }
    }

    SyncCurrentBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}

void CommerceDataModule::TrySyncHistoryBuys()
{
  std::string payload = "";
  if (GW2API::GetPayload(Entry, SyncHistoryBuysHandle, payload))
  {
    if (!payload.empty())
    {
      HistoryBuys.clear();

      nlohmann::json Json = nlohmann::json::parse(payload);
      std::vector<TransactionData> data = Json.get<std::vector<TransactionData>>();
      for (int i = 0; i < data.size(); i++)
      {
        TransactionData& transaction = data[i];
        bool Merged = false;
        for (int j = 0; j < HistoryBuys.size(); j++)
        {
          TransactionData& savedTransaction = HistoryBuys[j];
          if (savedTransaction.ItemID == transaction.ItemID && savedTransaction.Price == transaction.Price)
          {
            savedTransaction.Quantity += transaction.Quantity;
            Merged = true;
            break;
          }
        }

        if (!Merged)
          HistoryBuys.push_back(transaction);
      }
    }

    SyncHistoryBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}

int CommerceDataModule::GetSellPrice(unsigned int id)
{
  auto it = CurrentPrices.find(id);
  if (it != CurrentPrices.end())
  {
    return it->second.Sells.UnitPrice;
  }

  if (std::find(PriceWatch.begin(), PriceWatch.end(), id) == PriceWatch.end())
  {
    PriceWatch.push_back(id);
  }
  return 0;
}


void CommerceDataModule::TrySyncPrices()
{
  std::string payload = "";
  if (GW2API::GetPayload(Entry, SyncPricesHandle, payload))
  {
    if (!payload.empty())
    {
      nlohmann::json Json = nlohmann::json::parse(payload);
      for (const auto& item_json : Json)
      {
        PriceData price = item_json.get<PriceData>();
        CurrentPrices[price.ItemID] = price;
      }

    }
    SyncPricesHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}

void CommerceDataModule::RequestSyncPrices()
{
  PricesTimer.SetNow();
  if (PriceWatch.size() > 0)
  {
    std::string ids = "?ids=";
    for (int i = 0; i < PriceWatch.size(); ++i)
    {
      if (i != 0)
        ids += ",";

      ids += std::to_string(PriceWatch[i]);
    }

    SyncPricesHandle = GW2API::Request(Entry, API_COMMERCE_PRICES, ids);
  }
}