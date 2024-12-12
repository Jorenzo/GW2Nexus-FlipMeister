#include <Windows.h>

#include "pch.h"

void AddonLoad(AddonAPI* aApi);
void AddonUnload();
void AddonPreRender();
void AddonRender();
void AddonOptions();

Addon addon = Addon();
AddonDefinition AddonDef;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH: addon.SetHModule(hModule); break;
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
  addon.ProcessKeybind(aIdentifier);
}

void AddonLoad(AddonAPI* aApi)
{
  aApi->RegisterRender(ERenderType_Render, AddonRender);
  aApi->RegisterRender(ERenderType_PreRender, AddonPreRender);
  aApi->RegisterRender(ERenderType_OptionsRender, AddonOptions);

  aApi->RegisterKeybindWithString(ADDON_VISIBILITY_KEYBIND, &ProcessKeybind, "ALT+F");

  addon.AddonLoad(aApi);

  addon.Log(INFO, ADDON_LOG_NAME " Addon Loaded");
}

void AddonUnload()
{
  addon.AddonUnload();

  addon.GetAPI()->DeregisterRender(AddonRender);
  addon.GetAPI()->DeregisterRender(AddonPreRender);
  addon.GetAPI()->DeregisterRender(AddonOptions);

  addon.Log(INFO, ADDON_LOG_NAME " Addon Unloaded");
}

void AddonPreRender()
{
  addon.AddonPreRender();
}

void AddonRender()
{
  addon.AddonRender();
}

void AddonOptions()
{
  addon.AddonOptions();
}