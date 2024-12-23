#include "pch.h"

const int PageSize = 200;

CommerceDataModule::CommerceDataModule(Addon* addon) :
  PricesTimer()
{
  FAddon = addon;
  PullCurrentBuys();
  PullCurrentSells();
  PullHistoryBuys();
  PullHistorySells();
  PullDelivery();
  PricesTimer.SetNow();
}

void CommerceDataModule::Update()
{
  if ((PricesTimer.GetSecondsPassed() > FAddon->GetSettings()->AutoUpdatePriceWatchSeconds() || (NewItemsInPriceWatch && PricesTimer.GetSecondsPassed() > 5.0)) && SyncPricesHandle == HTTPREQUEST_HANDLE_INVALID)
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
  if (SyncHistorySellsHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncHistorySells();
  }
  if (SyncCurrentBuysHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncCurrentBuys();
  }
  if (SyncCurrentSellsHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncCurrentSells();
  }  
  if (SyncDeliveryHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TrySyncDelivery();
  }
}

std::string GetPageRequest(int page)
{
  std::ostringstream oss;
  oss << "&page_size=" << PageSize << "&page=" << page;
  return oss.str();
}

void CommerceDataModule::PullCurrentBuys()
{
  if (SyncCurrentBuysHandle != HTTPREQUEST_HANDLE_INVALID)
    return;

  SyncCurrentBuysHandle = GW2API::Request(FAddon, API_COMMERCE_TRANSACTIONS_CURRENT_BUYS, GetPageRequest(SyncCurrentBuysPageIndex));
}

void CommerceDataModule::PullCurrentSells()
{
  if (SyncCurrentSellsHandle != HTTPREQUEST_HANDLE_INVALID)
    return;

  SyncCurrentSellsHandle = GW2API::Request(FAddon, API_COMMERCE_TRANSACTIONS_CURRENT_SELLS, GetPageRequest(SyncCurrentSellsPageIndex));
}

void CommerceDataModule::PullHistoryBuys()
{
  if (SyncHistoryBuysHandle != HTTPREQUEST_HANDLE_INVALID)
    return;

  SyncHistoryBuysHandle = GW2API::Request(FAddon, API_COMMERCE_TRANSACTIONS_HISTORY_BUYS, GetPageRequest(SyncHistoryBuysPageIndex));
}

void CommerceDataModule::PullHistorySells()
{
  if (SyncHistorySellsHandle != HTTPREQUEST_HANDLE_INVALID)
    return;

  SyncHistorySellsHandle = GW2API::Request(FAddon, API_COMMERCE_TRANSACTIONS_HISTORY_SELLS, GetPageRequest(SyncHistorySellsPageIndex));
}

void CommerceDataModule::PullDelivery()
{
  if (SyncDeliveryHandle != HTTPREQUEST_HANDLE_INVALID)
    return;

  SyncDeliveryHandle = GW2API::Request(FAddon, API_COMMERCE_DELIVERY);
}

void CommerceDataModule::TrySyncCurrentBuys()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, SyncCurrentBuysHandle, payload))
  {
    if (!payload.empty())
    {
      if (SyncCurrentBuysPageIndex == 0)
        CurrentBuys.clear();

      try
      {
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

        if (data.size() == PageSize)
        {
          SyncCurrentBuysPageIndex++;
          SyncCurrentBuysHandle = HTTPREQUEST_HANDLE_INVALID;
          PullCurrentBuys();
          return;
        }
      }
      catch (const nlohmann::json::exception&)
      {
        FAddon->Log(WARNING, "Can not create TransactionData - Current Buys: %s", payload.c_str());
      }
    }

    SyncCurrentBuysPageIndex = 0;
    SyncCurrentBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}

void CommerceDataModule::TrySyncCurrentSells()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, SyncCurrentSellsHandle, payload))
  {
    if (!payload.empty())
    {
      if (SyncCurrentSellsPageIndex == 0)
        CurrentSells.clear();

      try
      {
        nlohmann::json Json = nlohmann::json::parse(payload);
        std::vector<TransactionData> data = Json.get<std::vector<TransactionData>>();

        for (int i = 0; i < data.size(); i++)
        {
          TransactionData& transaction = data[i];
          bool Merged = false;
          for (int j = 0; j < CurrentSells.size(); j++)
          {
            TransactionData& savedTransaction = CurrentSells[j];
            if (savedTransaction.ItemID == transaction.ItemID && savedTransaction.Price == transaction.Price)
            {
              savedTransaction.Quantity += transaction.Quantity;
              Merged = true;
              break;
            }
          }

          if (!Merged)
            CurrentSells.push_back(transaction);
        }

        if (data.size() == PageSize)
        {
          SyncCurrentSellsPageIndex++;
          SyncCurrentSellsHandle = HTTPREQUEST_HANDLE_INVALID;
          PullCurrentSells();
          return;
        }
      }
      catch (const nlohmann::json::exception&)
      {
        FAddon->Log(WARNING, "Can not create TransactionData - Current Sells: %s", payload.c_str());
      }
    }

    SyncCurrentSellsPageIndex = 0;
    SyncCurrentSellsHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}

void CommerceDataModule::TrySyncHistoryBuys()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, SyncHistoryBuysHandle, payload))
  {
    if (!payload.empty())
    {
      if(SyncHistoryBuysPageIndex == 0)
        HistoryBuys.clear();

      try
      {
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

        if (data.size() == PageSize)
        {
          SyncHistoryBuysPageIndex++;
          SyncHistoryBuysHandle = HTTPREQUEST_HANDLE_INVALID;
          PullHistoryBuys();
          return;
        }
      }
      catch (const nlohmann::json::exception&)
      {
        FAddon->Log(WARNING, "Can not create TransactionData - History Buys: %s", payload.c_str());
      }
    }

    SyncHistoryBuysPageIndex = 0;
    SyncHistoryBuysHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}

void CommerceDataModule::TrySyncHistorySells()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, SyncHistorySellsHandle, payload))
  {
    if (!payload.empty())
    {
      if (SyncHistorySellsPageIndex == 0)
        HistorySells.clear();

      try
      {
        nlohmann::json Json = nlohmann::json::parse(payload);
        std::vector<TransactionData> data = Json.get<std::vector<TransactionData>>();
        for (int i = 0; i < data.size(); i++)
        {
          TransactionData& transaction = data[i];
          bool Merged = false;
          for (int j = 0; j < HistorySells.size(); j++)
          {
            TransactionData& savedTransaction = HistorySells[j];
            if (savedTransaction.ItemID == transaction.ItemID && savedTransaction.Price == transaction.Price)
            {
              savedTransaction.Quantity += transaction.Quantity;
              Merged = true;
              break;
            }
          }

          if (!Merged)
            HistorySells.push_back(transaction);
        }

        if (data.size() == PageSize)
        {
          SyncHistorySellsPageIndex++;
          SyncHistorySellsHandle = HTTPREQUEST_HANDLE_INVALID;
          PullHistorySells();
          return;
        }
      }
      catch (const nlohmann::json::exception&)
      {
        FAddon->Log(WARNING, "Can not create TransactionData - History Sells: %s", payload.c_str());
      }
    }

    SyncHistorySellsPageIndex = 0;
    SyncHistorySellsHandle = HTTPREQUEST_HANDLE_INVALID;
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
    NewItemsInPriceWatch = true;
  }
  return 0;
}


void CommerceDataModule::TrySyncPrices()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, SyncPricesHandle, payload))
  {
    if (!payload.empty())
    {
      try
      {
        nlohmann::json Json = nlohmann::json::parse(payload);
        for (const auto& item_json : Json)
        {
          PriceData price = item_json.get<PriceData>();
          CurrentPrices[price.ItemID] = price;
        }
      }
      catch (const nlohmann::json::exception&)
      {
        FAddon->Log(WARNING, "Can not create PriceData: %s", payload.c_str());
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

    SyncPricesHandle = GW2API::Request(FAddon, API_COMMERCE_PRICES, ids);
    NewItemsInPriceWatch = false;
  }
}

void CommerceDataModule::TrySyncDelivery()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, SyncDeliveryHandle, payload))
  {
    if (!payload.empty())
    {
      try
      {
        nlohmann::json Json = nlohmann::json::parse(payload);
        Delivery = Json.get<DeliveryData>();
      }
      catch (const nlohmann::json::exception&)
      {
        FAddon->Log(WARNING, "Can not create DeliveryData: %s", payload.c_str());
      }
    }

    SyncDeliveryHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}