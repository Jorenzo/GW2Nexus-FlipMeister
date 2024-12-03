#pragma once

struct EntryData;

struct SettingsData
{
  std::string APIKey = "";
};
inline void to_json(nlohmann::json& j, const SettingsData& data) {
  j = nlohmann::json{
    { "APIKey", data.APIKey },
  };
}
inline void from_json(const nlohmann::json& j, SettingsData& data) {
  j.at("APIKey").get_to(data.APIKey);
}

struct AccountData
{
  std::string Name;
};
inline void to_json(nlohmann::json& j, const AccountData& data) {
  j = nlohmann::json{
    { "name", data.Name },
  };
}
inline void from_json(const nlohmann::json& j, AccountData& data) {
  j.at("name").get_to(data.Name);
}

class Settings
{
public:
  Settings(EntryData* entry);
  void Init();
  std::string GetAPIKey() const { return Data.APIKey; }
  void SetAPIKey(std::string key);
  std::string GetConnectedAccount() const { return ConnectedAccount; }
  bool HasValidAPIKey() { return !ConnectedAccount.empty(); }
private:
  void WriteSettings();
  void ReadSettings();
  void ConnectAccount();
  SettingsData Data;
  std::string ConnectedAccount = "";

  EntryData* Entry = nullptr;
};