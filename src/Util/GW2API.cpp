#include "pch.h"

std::string GW2API::Request(EntryData* Entry, const APIEndPointDefinition& Endpoint, const std::string& ExtraData)
{
  std::string payload = "";
  try
  {
    std::string FullRequest = GW2_API + Endpoint.EndPoint;
    if (Endpoint.NeedsAccess)
    {
      if (Entry && Entry->Settings)
      {
        if (!Entry->Settings->GetAPIKey().empty())
        {
          FullRequest += "?access_token=";
          FullRequest += Entry->Settings->GetAPIKey();
        }
        else
          Log(Entry, CRITICAL, "Trying to reach endpoint '%s' with API Key, but API Key is empty", FullRequest.c_str());
      }
      else
      {
        Log(Entry, CRITICAL, "Trying to reach endpoint '%s' but Entry or Settings are nullptr", FullRequest.c_str());
      }
    }
    if (ExtraData != "")
    {
      FullRequest += ExtraData;
    }

    payload = HTTPClient::GetRequest(FullRequest);
    int retries = 0;
    while (payload.empty() && retries < 3)
    {
      payload = HTTPClient::GetRequest(FullRequest);
      retries++;
    }


    if (!payload.empty())
    {
      if (retries > 0)
      {
        Log(Entry, WARNING, "Found payload for '%s' after %i retries:", FullRequest.c_str(), retries);
      }
    }
    else
    {
      Log(Entry, CRITICAL, "Payload for '%s' turned up empty after %i retries:", FullRequest.c_str(), retries);
    }
  }
  catch (const std::exception& e)
  {
    Log(Entry, CRITICAL, "Unknown exception performing HTTP call: \n%s", e.what());
  }

  return payload;
}