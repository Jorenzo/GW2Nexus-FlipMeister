#pragma once

struct EntryData;

struct SettingsData
{
  std::string APIKey = "";
  bool ShowQuickAccessIcon = true;
};
inline void to_json(nlohmann::json& j, const SettingsData& data) 
{
  j = nlohmann::json{
    { "APIKey", data.APIKey },
    { "ShowQuickAccessIcon", data.ShowQuickAccessIcon },
  };
}
inline void from_json(const nlohmann::json& j, SettingsData& data) 
{
  if(j.contains("APIKey"))
    j.at("APIKey").get_to(data.APIKey);
  if(j.contains("ShowQuickAccessIcon"))
    j.at("ShowQuickAccessIcon").get_to(data.ShowQuickAccessIcon);
}

struct AccountData
{
  std::string Name;
};
inline void to_json(nlohmann::json& j, const AccountData& data) 
{
  j = nlohmann::json{
    { "name", data.Name },
  };
}
inline void from_json(const nlohmann::json& j, AccountData& data) 
{
  j.at("name").get_to(data.Name);
}

class Settings
{
public:
  Settings(Addon* addon);
  void Init();
  void Update();
  void Render();
  std::string GetAPIKey() const { return Data.APIKey; }
  void SetAPIKey(std::string key);
  std::string GetConnectedAccount() const { return ConnectedAccount; }
  bool HasValidAPIKey() { return !ConnectedAccount.empty(); }
private:
  void WriteSettings();
  void ReadSettings();
  void RequestConnectAccount();
  void TryConnectAccount();
  SettingsData Data;
  std::string ConnectedAccount = "";
  Addon* FAddon = nullptr;
  HTTPRequestHandle ConnectedAccountHandle = HTTPREQUEST_HANDLE_INVALID;
  char APIInputBuffer[128];
};