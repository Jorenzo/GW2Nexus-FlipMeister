#include "pch.h"

HTTPRequestHandle GW2API::Request(Addon* addon, const APIEndPointDefinition& Endpoint, const std::string& ExtraData)
{
  std::string payload = "";
  try
  {
    std::string FullRequest = GW2_API + Endpoint.EndPoint;
    if (Endpoint.NeedsAccess)
    {
      if (addon && addon->GetSettings())
      {
        if (!addon->GetSettings()->GetAPIKey().empty())
        {
          FullRequest += "?access_token=";
          FullRequest += addon->GetSettings()->GetAPIKey();
        }
        else
          addon->Log(CRITICAL, "Trying to reach endpoint '%s' with API Key, but API Key is empty", FullRequest.c_str());
      }
      else
      {
        addon->Log(CRITICAL, "Trying to reach endpoint '%s' but Entry or Settings are nullptr", FullRequest.c_str());
      }
    }
    if (ExtraData != "")
    {
      FullRequest += ExtraData;
    }
    //addon->Log(DEBUG, "Requesting: %s", FullRequest.c_str());

    return addon->GetHTTPClient()->QueueRequest(FullRequest);
  }
  catch (const std::exception& e)
  {
    addon->Log(CRITICAL, "Unknown exception performing HTTP call: \n%s", e.what());
  }

  return HTTPREQUEST_HANDLE_INVALID;
}

bool GW2API::GetPayload(Addon* addon, HTTPRequestHandle handle, std::string& outData)
{
  if (!addon->GetHTTPClient()->IsRequestDone(handle))
    return false;

  outData = addon->GetHTTPClient()->GetResponse(handle);
  addon->GetHTTPClient()->CleanupRequest(handle);
  return true;
}
