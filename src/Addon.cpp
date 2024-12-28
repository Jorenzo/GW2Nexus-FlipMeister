#include "pch.h"

#define LOGO "FLIPMEISTER_LOGO"
#define QUICKACCESS "FLIPMEISTER_QUICKACCESS"
#define QUICKACCESS_HOVER "FLIPMEISTER_QUICKACCESS_HOVER"

#define ADDON_SHORTCUT "FLIPMEISTER_SHORTCUT"

Addon::Addon()
{

}

void Addon::SetHModule(HMODULE module)
{
  hSelf = module;
}

void Addon::ProcessKeybind(const char* aIdentifier)
{
  if (strcmp(aIdentifier, ADDON_VISIBILITY_KEYBIND) == 0)
  {
    Visible = !Visible;
    return;
  }
}

void Addon::AddonLoad(AddonAPI* aApi)
{
  APIDefs = aApi;

  ImGui::SetCurrentContext((ImGuiContext*)APIDefs->ImguiContext);
  ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree);

  NexusLink = (NexusLinkData*)APIDefs->GetResource("DL_NEXUS_LINK");
  //Entry.MumbleLink = (Mumble::Data*)Entry.APIDefs->GetResource("DL_MUMBLE_LINK");

  FHTTPClient = new HTTPClient(this);

  FSettings = new Settings(this);
  FSettings->Init();

  APIDefs->LoadTextureFromResource(LOGO, FM_Logo, hSelf, nullptr);
  APIDefs->LoadTextureFromResource(QUICKACCESS, FM_QuickAccess, hSelf, nullptr);
  APIDefs->LoadTextureFromResource(QUICKACCESS_HOVER, FM_QuickAccessHover, hSelf, nullptr);
  CurrencyDisplay::SetupResources(this);

  if (FSettings->ShowQuickAccessIcon())
    AddQuickAccessIcon();

  ModuleData M;
  M.Tracker = new TrackerModule(this);
  M.CompletedTracker = new CompletedTrackerModule(this);
  M.ItemData = new ItemDataModule(this);
  M.CommerceData = new CommerceDataModule(this);

  Modules = M;

  UIData U;
  U.Tracker = new TrackerUI(this);
  U.CompletedTracker = new CompletedTrackerUI(this);
  U.TradingPost = new TradingPostUI(this);
  U.NewTrackerItem = new NewTrackerItemUI(this);
  U.CompleteTrackedItem = new CompleteTrackedItemUI(this);

  UI = U;
}

void Addon::AddonUnload()
{
  if (FSettings->ShowQuickAccessIcon())
    RemoveQuickAccessIcon();

  APIDefs->DeregisterKeybind(ADDON_VISIBILITY_KEYBIND);

  delete FHTTPClient;
  delete FSettings;
  delete Modules.CommerceData;
  delete Modules.CompletedTracker;
  delete Modules.ItemData;
  delete Modules.Tracker;
  delete UI.CompletedTracker;
  delete UI.CompleteTrackedItem;
  delete UI.TradingPost;
  delete UI.NewTrackerItem;
  delete UI.Tracker;
}

void Addon::AddonPreRender()
{
  FHTTPClient->UpdateRequests();
  FSettings->Update();
  Modules.CommerceData->Update();
  Modules.ItemData->Update();
}

void Addon::AddonRender()
{
  //ImGuiIO& io = ImGui::GetIO();

  if (Visible)
  {
    if (ImGui::Begin(ADDON_NAME, &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
    {
      Texture* LogoTexture = APIDefs->GetTexture(LOGO);
      if (LogoTexture)
      {
        float scale = 0.8f * GetScaleRatio();
        ImVec2 imageSize = ImVec2(((float)LogoTexture->Width) * scale, ((float)LogoTexture->Height) * scale);
        float contentRegionWidth = ImGui::GetContentRegionAvail().x;
        float xPos = (contentRegionWidth - imageSize.x) * 0.5f;
        ImGui::SetCursorPosX(xPos);
        ImGui::Image((ImTextureID)LogoTexture->Resource, imageSize);
      }

      ImGui::Separator();
      ImGui::Separator();

      if (ImGui::Button("Open Trading Post Transactions", ImVec2(250 * GetScaleRatio(), 30 * GetScaleRatio())))
      {
        UI.TradingPost->Open();
      }
      ImGui::SameLine();
      if (ImGui::Button("Open Completed Tracked Items", ImVec2(250 * GetScaleRatio(), 30 * GetScaleRatio())))
      {
        UI.CompletedTracker->Open();
      }

      //ImGui::Text("UI Tick: %u", nullptr != Entry.MumbleLink ? Entry.MumbleLink->UITick : 0 );

      //ImGui::Text("%s", nullptr != Entry.NexusLink ? Entry.NexusLink->IsMoving ? "Currently moving!" : "Currently standing still." : "We don't know whether we are standing or moving? NexusLink seems to be empty." );

      UI.Tracker->Render();


      //bool show = true;
      //ImGui::ShowDemoWindow(&show);
      ImGui::End();
    }
  }

  UI.TradingPost->Render();
  UI.CompletedTracker->Render();
  UI.NewTrackerItem->Render();
  UI.CompleteTrackedItem->Render();
}

void Addon::AddonOptions()
{
  FSettings->Render();
}

void Addon::Log(ELogLevel LogType, const std::string Log, ...)
{
  constexpr size_t buffer_size = 1024;  // Adjust this as needed
  char buffer[buffer_size];

  // Start processing the variadic arguments
  va_list args;
  va_start(args, Log);

  // Format the message into the buffer
  std::vsnprintf(buffer, buffer_size, Log.c_str(), args);

  // End processing the variadic arguments
  va_end(args);

  // Convert the buffer to a std::string
  std::string logMessage(buffer);

  // Pass the formatted string to the logging system
  APIDefs->Log(LogType, ADDON_LOG_NAME, logMessage.c_str());
}

void Addon::ShowQuickAccessIconChanged()
{
  bool show = FSettings->ShowQuickAccessIcon();
  if (show)
    AddQuickAccessIcon();
  else
    RemoveQuickAccessIcon();
}

void Addon::AddQuickAccessIcon()
{
  APIDefs->AddShortcut(ADDON_SHORTCUT, QUICKACCESS, QUICKACCESS_HOVER, ADDON_VISIBILITY_KEYBIND, "");
}

void Addon::RemoveQuickAccessIcon()
{
  APIDefs->RemoveShortcut(ADDON_SHORTCUT);
}