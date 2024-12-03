#pragma once
#pragma comment(lib, "urlmon.lib")

namespace HTTPClient {
  inline static std::string GetRequest(std::string url) {
    const char* cUrl = url.c_str();
    std::wstring wUrl(cUrl, cUrl + strlen(cUrl));
    IStream* stream;
    HRESULT result = URLOpenBlockingStreamW(0, wUrl.c_str(), &stream, 0, 0);
    if (result != 0) {
      return "";
    }

    const unsigned long chunkSize = 128;
    char buffer[chunkSize];
    unsigned long bytesRead;
    std::stringstream strStream;

    stream->Read(buffer, chunkSize, &bytesRead);
    while (bytesRead > 0) {
      strStream.write(buffer, (long long)bytesRead);
      stream->Read(buffer, chunkSize, &bytesRead);
    }
    stream->Release();
    std::string response = strStream.str();
    return response;
  }

  //Thanks ChatGPT also Delta pls fix ur API thanks :)
  std::pair<std::string, std::string> inline static SplitRemoteFromEndpoint(const std::string& endpoint) {
    size_t protocolEnd = endpoint.find("://");
    size_t start = (protocolEnd != std::string::npos) ? protocolEnd + 3 : 0; // Skip "://" if present

    size_t pathStart = endpoint.find('/', start); // Find where the path starts
    if (pathStart == std::string::npos) {
      // No path found, assume the entire remaining string is the remote
      return { endpoint.substr(start), "" };
    }

    // Split into remote and remainder
    return { endpoint.substr(0, pathStart), endpoint.substr(pathStart) };
  }
}