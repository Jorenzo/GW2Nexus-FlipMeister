#include "pch.h"

HTTPClient::HTTPClient(Addon* addon)
{
  FAddon = addon;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  multi_handle = curl_multi_init();
  next_handle_id = 1;
}

HTTPClient::~HTTPClient()
{
  for (auto&[handle, data] : requests) 
  {
    curl_multi_remove_handle(multi_handle, data.easy_handle);
    curl_easy_cleanup(data.easy_handle);
  }
  curl_multi_cleanup(multi_handle);
  curl_global_cleanup();
}

HTTPRequestHandle HTTPClient::QueueRequest(const std::string& url)
{
  CURL* easy_handle = curl_easy_init();
  if (!easy_handle) 
  {
    throw std::runtime_error("Failed to create CURL easy handle");
  }

  std::shared_ptr<std::string> response_data = std::make_shared<std::string>();
  curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, response_data.get());

  RequestData request_data = { easy_handle, response_data, false };
  requests[next_handle_id] = request_data;

  curl_multi_add_handle(multi_handle, easy_handle);

  return next_handle_id++;
}

void HTTPClient::UpdateRequests() 
{
  int still_running;
  curl_multi_perform(multi_handle, &still_running);

  int msgs_in_queue;
  CURLMsg* msg;

  while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue))) 
  {
    if (msg->msg == CURLMSG_DONE) 
    {
      for (auto&[handle, data] : requests)
      {
        if (data.easy_handle == msg->easy_handle)
        {
          data.is_done = true;
          break;
        }
      }
    }
  }
}

bool HTTPClient::IsRequestDone(HTTPRequestHandle handle) 
{
  auto it = requests.find(handle);
  if (it == requests.end()) 
  {
    FAddon->Log(CRITICAL, "HTTPClient::IsRequestDone: Invalid Handle %i", handle);
    return false;
  }

  return it->second.is_done;
}

std::string HTTPClient::GetResponse(HTTPRequestHandle handle)
{
  auto it = requests.find(handle);
  if (it == requests.end()) 
  {
    FAddon->Log(CRITICAL, "HTTPClient::GetResponse: Invalid Handle %i", handle);
    return "";
  }

  if (!it->second.is_done) 
  {
    FAddon->Log(CRITICAL, "HTTPClient::GetResponse: Request not yet complete! Are you checking if its done?");
    return "";
  }

  return *(it->second.response_data);
}

void HTTPClient::CleanupRequest(HTTPRequestHandle handle)
{
  auto it = requests.find(handle);
  if (it == requests.end()) 
  {
    FAddon->Log(CRITICAL, "HTTPClient::CleanupRequest: Invalid Handle %i", handle);
    return;
  }

  curl_multi_remove_handle(multi_handle, it->second.easy_handle);
  curl_easy_cleanup(it->second.easy_handle);
  requests.erase(it);
}

size_t HTTPClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) 
{
  size_t total_size = size * nmemb;
  userp->append((char*)contents, total_size);
  return total_size;
}

//Thanks ChatGPT also Delta pls fix ur API thanks :)
std::pair<std::string, std::string> HTTPClient::SplitRemoteFromEndpoint(const std::string& endpoint) 
{
  size_t protocolEnd = endpoint.find("://");
  size_t start = (protocolEnd != std::string::npos) ? protocolEnd + 3 : 0; // Skip "://" if present

  size_t pathStart = endpoint.find('/', start); // Find where the path starts
  if (pathStart == std::string::npos)
  {
    // No path found, assume the entire remaining string is the remote
    return { endpoint.substr(start), "" };
  }

  // Split into remote and remainder
  return { endpoint.substr(0, pathStart), endpoint.substr(pathStart) };
}