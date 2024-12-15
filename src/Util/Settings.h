#pragma once

struct EntryData;

struct SettingsData
{
  std::string APIKey = "";
  bool ShowQuickAccessIcon = true;
  bool AutoUpdateTradingPost = true;
  int AutoUpdateTradingPostSeconds = 180;
  int AutoUpdatePriceWatchSeconds = 60;
  bool TrackerCalculateUndercut = false;
  int TrackerUndercutValue = 1;
};
inline void to_json(nlohmann::json& j, const SettingsData& data) 
{
  j = nlohmann::json{
    { "APIKey", data.APIKey },
    { "ShowQuickAccessIcon", data.ShowQuickAccessIcon },
    { "AutoUpdateTradingPost", data.AutoUpdateTradingPost },
    { "AutoUpdateTradingPostSeconds", data.AutoUpdateTradingPostSeconds },
    { "AutoUpdatePriceWatchSeconds", data.AutoUpdatePriceWatchSeconds },
    { "TrackerCalculateUndercut", data.TrackerCalculateUndercut },
    { "TrackerUndercutValue", data.TrackerUndercutValue },
  };
}
inline void from_json(const nlohmann::json& j, SettingsData& data) 
{
  if(j.contains("APIKey"))
    j.at("APIKey").get_to(data.APIKey);
  if(j.contains("ShowQuickAccessIcon"))
    j.at("ShowQuickAccessIcon").get_to(data.ShowQuickAccessIcon);
  if (j.contains("AutoUpdateTradingPost"))
    j.at("AutoUpdateTradingPost").get_to(data.AutoUpdateTradingPost);
  if (j.contains("AutoUpdateTradingPostSeconds"))
    j.at("AutoUpdateTradingPostSeconds").get_to(data.AutoUpdateTradingPostSeconds);
  if (j.contains("AutoUpdatePriceWatchSeconds"))
    j.at("AutoUpdatePriceWatchSeconds").get_to(data.AutoUpdatePriceWatchSeconds);
  if (j.contains("TrackerCalculateUndercut"))
    j.at("TrackerCalculateUndercut").get_to(data.TrackerCalculateUndercut);
  if (j.contains("TrackerUndercutValue"))
    j.at("TrackerUndercutValue").get_to(data.TrackerUndercutValue);
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
  std::string GetConnectedAccount() const { return ConnectedAccount; }
  bool HasValidAPIKey() { return !ConnectedAccount.empty(); }
  bool ShowQuickAccessIcon() const { return Data.ShowQuickAccessIcon; }
  bool AutoUpdateTradingPost() const { return Data.AutoUpdateTradingPost; }
  int AutoUpdateTradingPostSeconds() const { return Data.AutoUpdateTradingPostSeconds; }
  int AutoUpdatePriceWatchSeconds() const { return Data.AutoUpdatePriceWatchSeconds; }
  bool TrackerCalculateUndercut() const { return Data.TrackerCalculateUndercut; }
  int TrackerUndercutValue() const { return Data.TrackerUndercutValue; }
  void SetTrackerCalculateUndercut(bool value);
  void SetTrackerUndercutValue(int value);
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
  CurrencyInputField              TrackerUndercutInputField;
};