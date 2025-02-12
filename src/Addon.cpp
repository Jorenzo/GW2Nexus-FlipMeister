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

void Addon::ProcessKeybind(const char* aIdentifier, bool aIsRelease)
{
  if (aIsRelease && strcmp(aIdentifier, ADDON_VISIBILITY_KEYBIND) == 0)
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

  NexusLink = (NexusLinkData*)APIDefs->DataLink.Get("DL_NEXUS_LINK");
  //Entry.MumbleLink = (Mumble::Data*)Entry.APIDefs->GetResource("DL_MUMBLE_LINK");

  FHTTPClient = new HTTPClient(this);

  FSettings = new Settings(this);
  FSettings->Init();

  APIDefs->UI.RegisterCloseOnEscape(ADDON_NAME, &Visible);

  APIDefs->Textures.LoadFromResource(LOGO, FM_Logo, hSelf, nullptr);
  APIDefs->Textures.LoadFromResource(QUICKACCESS, FM_QuickAccess, hSelf, nullptr);
  APIDefs->Textures.LoadFromResource(QUICKACCESS_HOVER, FM_QuickAccessHover, hSelf, nullptr);
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

  APIDefs->InputBinds.Deregister(ADDON_VISIBILITY_KEYBIND);

  delete UI.CompletedTracker;
  delete UI.CompleteTrackedItem;
  delete UI.TradingPost;
  delete UI.NewTrackerItem;
  delete UI.Tracker;
  UI = {};
  delete Modules.CommerceData;
  delete Modules.CompletedTracker;
  delete Modules.ItemData;
  delete Modules.Tracker;
  Modules = {};
  delete FHTTPClient;
  FHTTPClient = nullptr;
  delete FSettings;
  FSettings = nullptr;
}

void Addon::AddonPreRender()
{
  if(FHTTPClient)
    FHTTPClient->UpdateRequests();
  if(FSettings)
    FSettings->Update();
  if(Modules.CommerceData)
    Modules.CommerceData->Update();
  if(Modules.ItemData)
    Modules.ItemData->Update();
}

void Addon::AddonRender()
{
  //ImGuiIO& io = ImGui::GetIO();

  if (Visible)
  {
    if (ImGui::Begin(ADDON_NAME, &Visible, ImGuiWindowFlags_NoCollapse))
    {
      Texture* LogoTexture = APIDefs->Textures.Get(LOGO);
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

      if (UI.Tracker)
      {
        if (ImGui::Button("Tracker", ImVec2(200 * GetScaleRatio(), 30 * GetScaleRatio())))
          CurrentPage = AddonPage_Tracker;
      }
      if (UI.TradingPost)
      {
        ImGui::SameLine();
        if(ImGui::Button("Trading Post", ImVec2(200 * GetScaleRatio(), 30 * GetScaleRatio())))
          CurrentPage = AddonPage_TradingPost;
      }
      if (UI.CompletedTracker)
      {
        ImGui::SameLine();
        if (ImGui::Button("Completed Tracked Items", ImVec2(200 * GetScaleRatio(), 30 * GetScaleRatio())))
          CurrentPage = AddongPage_CompletedTrackedItems;
      }

      ImGui::Separator();
      //ImGui::Text("UI Tick: %u", nullptr != Entry.MumbleLink ? Entry.MumbleLink->UITick : 0 );

      //ImGui::Text("%s", nullptr != Entry.NexusLink ? Entry.NexusLink->IsMoving ? "Currently moving!" : "Currently standing still." : "We don't know whether we are standing or moving? NexusLink seems to be empty." );
      ImVec2 availableSpace = ImGui::GetContentRegionAvail();
      ImGui::BeginChild("ScrollableSection", ImVec2(0, availableSpace.y), true);

      switch (CurrentPage)
      {
      case AddonPage_Tracker:
        if(UI.Tracker)
          UI.Tracker->Render();
        break;
      case AddonPage_TradingPost:
        if (UI.TradingPost)
          UI.TradingPost->Render();
        break;
      case AddongPage_CompletedTrackedItems:
        if (UI.CompletedTracker)
          UI.CompletedTracker->Render();
        break;
      default:
        break;
      }

      ImGui::EndChild();

      //bool show = true;
      //ImGui::ShowDemoWindow(&show);
      ImGui::End();
    }
  }

  if(UI.NewTrackerItem)
    UI.NewTrackerItem->Render();
  if(UI.CompleteTrackedItem)
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
  APIDefs->QuickAccess.Add(ADDON_SHORTCUT, QUICKACCESS, QUICKACCESS_HOVER, ADDON_VISIBILITY_KEYBIND, "");
}

void Addon::RemoveQuickAccessIcon()
{
  APIDefs->QuickAccess.Remove(ADDON_SHORTCUT);
}