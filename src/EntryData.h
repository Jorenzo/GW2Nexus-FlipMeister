#pragma once

#define ADDON_NAME "FlipMeister"
#define ADDON_LOG_NAME "FlipMeister"
#define ADDON_DIRECTORY_NAME "FlipMeister"
#define ADDON_VISIBILITY_KEYBIND "Toggle FlipMeister"

struct ModuleData
{
  class TrackerModule* Tracker = nullptr;
  class ItemDataModule* ItemData = nullptr;
  class CommerceDataModule* CommerceData = nullptr;
};

struct UIData
{
  class TradingPostUI* TradingPost = nullptr;
  class TrackerUI* Tracker = nullptr;
  class NewTrackerItemUI* NewTrackerItem = nullptr;
};

struct EntryData
{
  HMODULE hSelf = nullptr;
  AddonAPI* APIDefs = nullptr;
  NexusLinkData* NexusLink = nullptr;
  Mumble::Data* MumbleLink = nullptr;
  Settings* Settings = nullptr;

  ModuleData Modules = {};
  UIData UI = {};
};