#pragma once

struct APIEndPointDefinition
{
  APIEndPointDefinition(const char* ep, bool na) : EndPoint(ep), NeedsAccess(na) { }
  std::string EndPoint;
  bool NeedsAccess;
};

#define GW2_API "https://api.guildwars2.com/v2/"

#define API_COMMERCE_TRANSACTIONS_CURRENT_BUYS APIEndPointDefinition("commerce/transactions/current/buys", true)
#define API_COMMERCE_TRANSACTIONS_HISTORY_BUYS APIEndPointDefinition("commerce/transactions/history/buys", true)
#define API_COMMERCE_PRICES APIEndPointDefinition("commerce/prices", false)
#define API_ACCOUNT APIEndPointDefinition("account", true)
#define API_ITEMS APIEndPointDefinition("items", false)

class GW2API
{
public:
  static HTTPRequestHandle Request(EntryData* Entry, const APIEndPointDefinition& Endpoint, const std::string& ExtraData = "");
  static bool GetPayload(EntryData* Entry, HTTPRequestHandle handle, std::string& outData);
private:
};