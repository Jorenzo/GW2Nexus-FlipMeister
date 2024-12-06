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
  if (PricesTimer.GetSecondsPassed() > 5.0)
  {
    SyncPrices();
  }
}

void CommerceDataModule::PullCurrentBuys()
{
  std::string payload = GW2API::Request(Entry, API_COMMERCE_TRANSACTIONS_CURRENT_BUYS);

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
}

void CommerceDataModule::PullHistoryBuys()
{
  std::string payload = GW2API::Request(Entry, API_COMMERCE_TRANSACTIONS_HISTORY_BUYS);

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


void CommerceDataModule::SyncPrices()
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

    std::string payload = GW2API::Request(Entry, API_COMMERCE_PRICES, ids);

    if (!payload.empty())
    {
      nlohmann::json Json = nlohmann::json::parse(payload);
      for (const auto& item_json : Json)
      {
        PriceData price = item_json.get<PriceData>();
        CurrentPrices[price.ItemID] = price;
      }
    }
  }
}