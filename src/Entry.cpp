#include <Windows.h>

#include "pch.h"

#define LOGO "FLIPMEISTER_LOGO"
#define QUICKACCESS "FLIPMEISTER_QUICKACCESS"
#define QUICKACCESS_HOVER "FLIPMEISTER_QUICKACCESS_HOVER"

#define ADDON_SHORTCUT "FLIPMEISTER_SHORTCUT"

void AddonLoad(AddonAPI* aApi);
void AddonUnload();
void AddonRender();
void AddonOptions();

EntryData Entry;
AddonDefinition AddonDef;
bool Visible = false;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH: Entry.hSelf = hModule; break;
  case DLL_PROCESS_DETACH: break;
  case DLL_THREAD_ATTACH: break;
  case DLL_THREAD_DETACH: break;
  }
  return TRUE;
}

extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef()
{
  AddonDef.Signature = -9475; // set to random unused negative integer
  AddonDef.APIVersion = NEXUS_API_VERSION;
  AddonDef.Name = ADDON_NAME;
  AddonDef.Version.Major = 1;
  AddonDef.Version.Minor = 0;
  AddonDef.Version.Build = 0;
  AddonDef.Version.Revision = 1;
  AddonDef.Author = "Joren.4310";
  AddonDef.Description = "Utility for tracking purchases from the trading post. Evon Gnashblade approved!";
  AddonDef.Load = AddonLoad;
  AddonDef.Unload = AddonUnload;
  AddonDef.Flags = EAddonFlags_None;

  /* not necessary if hosted on Raidcore, but shown anyway for the example also useful as a backup resource */
  AddonDef.Provider = EUpdateProvider_GitHub;
  AddonDef.UpdateLink = "https://github.com/Jorenzo/GW2Nexus-FlipMeister";

  return &AddonDef;
}

void ProcessKeybind(const char* aIdentifier)
{
  if (strcmp(aIdentifier, ADDON_VISIBILITY_KEYBIND) == 0)
  {
    Visible = !Visible;
    return;
  }
}

void AddonLoad(AddonAPI* aApi)
{
  Entry.APIDefs = aApi;

  ImGui::SetCurrentContext((ImGuiContext*)Entry.APIDefs->ImguiContext);
  ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))Entry.APIDefs->ImguiMalloc, (void(*)(void*, void*))Entry.APIDefs->ImguiFree);

  Entry.NexusLink = (NexusLinkData*)Entry.APIDefs->GetResource("DL_NEXUS_LINK");
  //Entry.MumbleLink = (Mumble::Data*)Entry.APIDefs->GetResource("DL_MUMBLE_LINK");

  Entry.HTTPClient = new HTTPClient(&Entry);

  Entry.Settings = new Settings(&Entry);
  Entry.Settings->Init();

  Entry.APIDefs->RegisterRender(ERenderType_Render, AddonRender);
  Entry.APIDefs->RegisterRender(ERenderType_OptionsRender, AddonOptions);

  Entry.APIDefs->RegisterKeybindWithString(ADDON_VISIBILITY_KEYBIND, ProcessKeybind, "ALT+F");

  Entry.APIDefs->LoadTextureFromResource(LOGO, FM_Logo, Entry.hSelf, nullptr);
  Entry.APIDefs->LoadTextureFromResource(QUICKACCESS, FM_QuickAccess, Entry.hSelf, nullptr);
  Entry.APIDefs->LoadTextureFromResource(QUICKACCESS_HOVER, FM_QuickAccessHover, Entry.hSelf, nullptr);
  CurrencyDisplay::SetupResources(&Entry);

  Entry.APIDefs->AddShortcut(ADDON_SHORTCUT, QUICKACCESS, QUICKACCESS_HOVER, ADDON_VISIBILITY_KEYBIND, "");

  ModuleData Modules;
  Modules.Tracker = new TrackerModule(&Entry);
  Modules.CompletedTracker = new CompletedTrackerModule(&Entry);
  Modules.ItemData = new ItemDataModule(&Entry);
  Modules.CommerceData = new CommerceDataModule(&Entry);

  Entry.Modules = Modules;



  UIData UI;
  UI.Tracker = new TrackerUI(&Entry);
  UI.CompletedTracker = new CompletedTrackerUI(&Entry);
  UI.TradingPost = new TradingPostUI(&Entry);
  UI.NewTrackerItem = new NewTrackerItemUI(&Entry);
  UI.CompleteTrackedItem = new CompleteTrackedItemUI(&Entry);

  Entry.UI = UI;

  Log(&Entry, INFO, ADDON_LOG_NAME " Addon Loaded");
}

void AddonUnload()
{
  Entry.APIDefs->RemoveShortcut(ADDON_SHORTCUT);

  Entry.APIDefs->DeregisterKeybind(ADDON_VISIBILITY_KEYBIND);

  Entry.APIDefs->DeregisterRender(AddonRender);
  Entry.APIDefs->DeregisterRender(AddonOptions);

  delete Entry.HTTPClient;
  delete Entry.Settings;
  delete Entry.Modules.CommerceData;
  delete Entry.Modules.CompletedTracker;
  delete Entry.Modules.ItemData;
  delete Entry.Modules.Tracker;
  delete Entry.UI.CompletedTracker;
  delete Entry.UI.CompleteTrackedItem;
  delete Entry.UI.TradingPost;
  delete Entry.UI.NewTrackerItem;
  delete Entry.UI.Tracker;

  Log(&Entry, INFO, ADDON_LOG_NAME " Addon Unloaded");
}

void AddonRender()
{
  Entry.HTTPClient->UpdateRequests();
  Entry.Settings->Update();
  Entry.Modules.CommerceData->Update();
  Entry.Modules.ItemData->Update();

  ImGuiIO& io = ImGui::GetIO();

  if (Visible)
  {
    if (ImGui::Begin(ADDON_NAME, &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
    {
      Texture* LogoTexture = Entry.APIDefs->GetTexture(LOGO);
      if (LogoTexture)
      {
        float scale = 0.3f;
        ImVec2 imageSize = ImVec2(((float)LogoTexture->Width) * scale, ((float)LogoTexture->Height) * scale);
        float contentRegionWidth = ImGui::GetContentRegionAvail().x;
        float xPos = (contentRegionWidth - imageSize.x) * 0.5f;
        ImGui::SetCursorPosX(xPos);
        ImGui::Image((ImTextureID)LogoTexture->Resource, imageSize);
      }

      ImGui::Separator();
      ImGui::Separator();

      //ImGui::Text("UI Tick: %u", nullptr != Entry.MumbleLink ? Entry.MumbleLink->UITick : 0 );

      //ImGui::Text("%s", nullptr != Entry.NexusLink ? Entry.NexusLink->IsMoving ? "Currently moving!" : "Currently standing still." : "We don't know whether we are standing or moving? NexusLink seems to be empty." );

      Entry.UI.Tracker->Render();
      
      ImGui::End();
    }
  }

  Entry.UI.CompletedTracker->Render();
  Entry.UI.TradingPost->Render();
  Entry.UI.NewTrackerItem->Render();
  Entry.UI.CompleteTrackedItem->Render();
}

///----------------------------------------------------------------------------------------------------
/// AddonOptions:
/// 	Basically an ImGui callback that doesn't need its own Begin/End calls.
///----------------------------------------------------------------------------------------------------
void AddonOptions()
{
  ImGui::Separator();

  static char buffer[128] = "";
  if (ImGui::InputText("Enter API Key", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_Password))
  {
    Entry.Settings->SetAPIKey(buffer);
  }

  if (Entry.Settings->HasValidAPIKey())
  {
    ImGui::PushStyleColor(ImGuiCol_Text, COL_GREEN);
    ImGui::Text("Connected Account: %s", Entry.Settings->GetConnectedAccount().c_str());
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Text, COL_RED);
    ImGui::Text("No or Invalid API Key!");
  }
  ImGui::PopStyleColor();
}