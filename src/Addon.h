#pragma once

//Names
#define ADDON_NAME "FlipMeister"
#define ADDON_LOG_NAME "FlipMeister"
#define ADDON_DIRECTORY_NAME "FlipMeister"
#define ADDON_VISIBILITY_KEYBIND "Toggle FlipMeister"

//Logging
#define OFF ELogLevel_OFF
#define CRITICAL ELogLevel_CRITICAL
#define WARNING ELogLevel_WARNING
#define INFO ELogLevel_INFO
#define DEBUG ELogLevel_DEBUG
#define TRACE ELogLevel_TRACE

struct ModuleData
{
  class TrackerModule* Tracker = nullptr;
  class CompletedTrackerModule* CompletedTracker = nullptr;
  class ItemDataModule* ItemData = nullptr;
  class CommerceDataModule* CommerceData = nullptr;
};

struct UIData
{
  class TradingPostUI* TradingPost = nullptr;
  class TrackerUI* Tracker = nullptr;
  class CompletedTrackerUI* CompletedTracker = nullptr;
  class NewTrackerItemUI* NewTrackerItem = nullptr;
  class CompleteTrackedItemUI* CompleteTrackedItem = nullptr;
};

class Addon
{
public:
  Addon();
  void SetHModule(HMODULE module);
  void ProcessKeybind(const char* aIdentifier);
  void AddonLoad(AddonAPI* aApi);
  void AddonUnload();
  void AddonPreRender();
  void AddonRender();
  void AddonOptions();
  //
  AddonAPI* GetAPI() const { return APIDefs; }
  Settings* GetSettings() const { return FSettings; }
  HTTPClient* GetHTTPClient() const { return FHTTPClient; }
  const ModuleData* GetModules() const { return &Modules; }
  const UIData* GetUI() const { return &UI; }
  //
  void Log(ELogLevel LogType, const std::string Log, ...);
private:
  HMODULE hSelf = nullptr;
  AddonAPI* APIDefs = nullptr;
  NexusLinkData* NexusLink = nullptr;
  //Mumble::Data* MumbleLink = nullptr;
  Settings* FSettings = nullptr;
  HTTPClient* FHTTPClient = nullptr;
  ModuleData Modules = {};
  UIData UI = {};

  bool Visible = false;
};