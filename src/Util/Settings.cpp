#include "pch.h"

Settings::Settings(Addon* addon) :
  TrackerUndercutInputField(addon)
{
  FAddon = addon;
}

void Settings::Init()
{
  ReadSettings();
  
  strncpy_s(APIInputBuffer, sizeof(APIInputBuffer), Data.APIKey.c_str(), _TRUNCATE);
  APIInputBuffer[sizeof(APIInputBuffer) - 1] = '\0';
  
  TrackerUndercutInputField.SetValue(Data.TrackerUndercutValue);

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
    Data.APIKey = APIInputBuffer;
    WriteSettings();
    RequestConnectAccount();
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
    FAddon->ShowQuickAccessIconChanged();
  }
  if (ImGui::InputInt("Price Watch Refresh Time", &Data.AutoUpdatePriceWatchSeconds))
  {
    Data.AutoUpdatePriceWatchSeconds = std::max(Data.AutoUpdatePriceWatchSeconds, 15);
    WriteSettings();
  }
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Time in seconds between each refresh of all the item prices");
  ImGui::Separator();
  ImGui::Text("Tracker");
  ImGui::Separator();
  if (ImGui::Checkbox("Calculate Undercut", &Data.TrackerCalculateUndercut))
    SetTrackerCalculateUndercut(Data.TrackerCalculateUndercut);
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("When enabled, the tracker will have an undercut section which takes the amount of from the sell price");
  if (Data.TrackerCalculateUndercut)
  {
    if (TrackerUndercutInputField.Render(Data.TrackerUndercutValue, 1, "Undercut value"))
    {
      FAddon->GetSettings()->SetTrackerUndercutValue(Data.TrackerUndercutValue);
    }
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("The value of which the tracker will undercut the sell price");
  }
  ImGui::Separator();
  ImGui::Text("Trading Post");
  ImGui::Separator();
  if (ImGui::Checkbox("Auto Refresh Trading Post Transactions", &Data.AutoUpdateTradingPost))
    WriteSettings();
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("When enabled, the trading post UI will refresh the transactions of the selected tab automatically");
  if (ImGui::InputInt("Trading Post Refresh Time", &Data.AutoUpdateTradingPostSeconds))
  {
    Data.AutoUpdateTradingPostSeconds = std::max(Data.AutoUpdateTradingPostSeconds, 15);
    WriteSettings();
  }
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Time in seconds between each refresh of the selected transactions tab in the trading post UI");
}

void Settings::SetTrackerCalculateUndercut(bool value)
{
  Data.TrackerCalculateUndercut = value;
  WriteSettings();
}

void Settings::SetTrackerUndercutValue(int value)
{
  Data.TrackerUndercutValue = value;
  WriteSettings();
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
