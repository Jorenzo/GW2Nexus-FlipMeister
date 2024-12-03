#include "pch.h"

Settings::Settings(EntryData* entry)
{
  Entry = entry;
}

void Settings::Init()
{
  ReadSettings();
  ConnectAccount();
}

void Settings::SetAPIKey(std::string key)
{
  Data.APIKey = key;
  WriteSettings();
  ConnectAccount();
}


void Settings::WriteSettings()
{
  nlohmann::json SettingsJson = Data;

  std::string path = Entry->APIDefs->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\settings.json";

  std::filesystem::path dirPath = std::filesystem::path(path).parent_path();

  // Check if the directory exists, and create it if it doesn't
  if (!std::filesystem::exists(dirPath)) {
    if (!std::filesystem::create_directories(dirPath)) {
      Log(Entry, WARNING, "Failed to write settings file at '%s'", path.c_str());
    }
  }


  try {
    std::ofstream outFile(path);
    if (!outFile.is_open()) {
      throw std::ios_base::failure("Failed to open file");
    }
    outFile << SettingsJson.dump(4);
    outFile.close();
  }
  catch (const std::exception& e) {
    Log(Entry, WARNING, "Failed to write settings at file '%s'\n%s", path.c_str(), e.what());
  }
}

void Settings::ReadSettings()
{
  std::string path = Entry->APIDefs->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\settings.json";
  std::ifstream inFile(path);

  if (!inFile) {
    Log(Entry, WARNING, "Failed to open settings file at '%s'", path.c_str());
    return;
  }

  nlohmann::json jsonObject;
  try
  {
    inFile >> jsonObject;
  }
  catch (const nlohmann::json::parse_error& e) {
    Log(Entry, WARNING, "Failed to parse JSON in settings file '%s'\n %s", path.c_str(), e.what());
    return;
  }

  inFile.close();

  try
  {
    Data = jsonObject.get<SettingsData>();
  }
  catch (const nlohmann::json::type_error& e) {
    Log(Entry, WARNING, "JSON structure mismatch in settings file '%s'\n %s", path.c_str(), e.what());
    return;
  }
}

void Settings::ConnectAccount()
{
  std::string payload = GW2API::Request(Entry, API_ACCOUNT);

  if (!payload.empty())
  {
    nlohmann::json Json = nlohmann::json::parse(payload);
    AccountData data = Json.get<AccountData>();
    ConnectedAccount = data.Name;
  }
}
