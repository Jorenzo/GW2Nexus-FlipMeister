#include "pch.h"

HTTPRequestHandle GW2API::Request(EntryData* Entry, const APIEndPointDefinition& Endpoint, const std::string& ExtraData)
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

    return Entry->HTTPClient->QueueRequest(FullRequest);
  }
  catch (const std::exception& e)
  {
    Log(Entry, CRITICAL, "Unknown exception performing HTTP call: \n%s", e.what());
  }

  return HTTPREQUEST_HANDLE_INVALID;
}

bool GW2API::GetPayload(EntryData* Entry, HTTPRequestHandle handle, std::string& outData)
{
  if (!Entry->HTTPClient->IsRequestDone(handle))
    return false;

  outData = Entry->HTTPClient->GetResponse(handle);
  Entry->HTTPClient->CleanupRequest(handle);
  return true;
}
