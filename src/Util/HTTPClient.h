#pragma once
#pragma comment(lib, "urlmon.lib")


using HTTPRequestHandle = int;
#define HTTPREQUEST_HANDLE_INVALID -1

class HTTPClient
{
public:

  HTTPClient(class Addon* addon);
  ~HTTPClient();
  HTTPRequestHandle QueueRequest(const std::string& url);
  void UpdateRequests();
  bool IsRequestDone(HTTPRequestHandle handle);
  std::string GetResponse(HTTPRequestHandle handle);
  void CleanupRequest(HTTPRequestHandle handle);

  std::pair<std::string, std::string> static SplitRemoteFromEndpoint(const std::string& endpoint);
private:
  struct RequestData {
    CURL* easy_handle;
    std::shared_ptr<std::string> response_data;
    bool is_done;
  };

  CURLM* multi_handle;
  HTTPRequestHandle next_handle_id;
  std::unordered_map<HTTPRequestHandle, RequestData> requests;

  static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

  class Addon* FAddon;
};