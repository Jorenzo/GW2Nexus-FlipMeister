#include "pch.h"

Settings::Settings(Addon* addon)
{
  FAddon = addon;
}

void Settings::Init()
{
  ReadSettings();
  
  strncpy_s(APIInputBuffer, sizeof(APIInputBuffer), Data.APIKey.c_str(), _TRUNCATE);
  APIInputBuffer[sizeof(APIInputBuffer) - 1] = '\0';
  
  RequestConnectAccount();
}

void Settings::Update()
{
  if (ConnectedAccountHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    TryConnectAccount();
  }
}

void Settings::Render()
{
  ImGui::Separator();

  if (ImGui::InputText("Enter API Key", APIInputBuffer, IM_ARRAYSIZE(APIInputBuffer), ImGuiInputTextFlags_Password))
  {
    SetAPIKey(APIInputBuffer);
  }

  if (HasValidAPIKey())
  {
    ImGui::PushStyleColor(ImGuiCol_Text, COL_GREEN);
    ImGui::Text("Connected Account: %s", GetConnectedAccount().c_str());
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Text, COL_RED);
    ImGui::Text("No or Invalid API Key!");
  }
  ImGui::PopStyleColor();
  ImGui::Separator();
  if (ImGui::Checkbox("Show Quick Access Icon", &Data.ShowQuickAccessIcon))
  {
    WriteSettings();
  }
}

void Settings::SetAPIKey(std::string key)
{
  Data.APIKey = key;
  WriteSettings();
  RequestConnectAccount();
}


void Settings::WriteSettings()
{
  nlohmann::json SettingsJson = Data;

  std::string path = FAddon->GetAPI()->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\settings.json";

  std::filesystem::path dirPath = std::filesystem::path(path).parent_path();

  // Check if the directory exists, and create it if it doesn't
  if (!std::filesystem::exists(dirPath)) {
    if (!std::filesystem::create_directories(dirPath)) {
      FAddon->Log(WARNING, "Failed to write settings file at '%s'", path.c_str());
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
    FAddon->Log(WARNING, "Failed to write settings at file '%s'\n%s", path.c_str(), e.what());
  }
}

void Settings::ReadSettings()
{
  std::string path = FAddon->GetAPI()->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\settings.json";
  std::ifstream inFile(path);

  if (!inFile) {
    FAddon->Log(WARNING, "Failed to open settings file at '%s'", path.c_str());
    return;
  }

  nlohmann::json jsonObject;
  try
  {
    inFile >> jsonObject;
  }
  catch (const nlohmann::json::parse_error& e) {
    FAddon->Log(WARNING, "Failed to parse JSON in settings file '%s'\n %s", path.c_str(), e.what());
    return;
  }

  inFile.close();

  try
  {
    Data = jsonObject.get<SettingsData>();
  }
  catch (const nlohmann::json::type_error& e) {
    FAddon->Log(WARNING, "JSON structure mismatch in settings file '%s'\n %s", path.c_str(), e.what());
    return;
  }
}

void Settings::RequestConnectAccount()
{
  if (ConnectedAccountHandle != HTTPREQUEST_HANDLE_INVALID)
  {
    FAddon->Log(CRITICAL, "Trying to connect account, but already have an outgoing handle. Overriding");
  }
  ConnectedAccountHandle = GW2API::Request(FAddon, API_ACCOUNT);
}

void Settings::TryConnectAccount()
{
  std::string payload = "";
  if (GW2API::GetPayload(FAddon, ConnectedAccountHandle, payload))
  {
    if (!payload.empty())
    {
      nlohmann::json Json = nlohmann::json::parse(payload);
      AccountData data = Json.get<AccountData>();
      ConnectedAccount = data.Name;
    }
    else
      FAddon->Log(WARNING, "No payload when trying to connect account! Provided key: %s", Data.APIKey.c_str());

    ConnectedAccountHandle = HTTPREQUEST_HANDLE_INVALID;
  }
}
