#pragma once
#define NOMINMAX
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <algorithm>
#include "curl/curl.h"

#include "../assets/resource.h"

#include "nexus/Nexus.h"
//#include "mumble/Mumble.h"
#include "imgui/imgui.h"

#include "util/json.hpp"
#include "util/HTTPClient.h"
#include "util/Settings.h"
#include "util/GW2API.h"
#include "util/Colors.h"
#include "Util/Timer.h"

#include "Addon.h"

#include "Modules/ItemDataModule.h"
#include "Modules/CommerceDataModule.h"
#include "Modules/TrackerModule.h"
#include "Modules/CompletedTrackerModule.h"

#include "UI/CurrencyDisplay.h"
#include "UI/CurrencyInputField.h"
#include "UI/TrackerUI.h"
#include "UI/CompletedTrackerUI.h"
#include "UI/TradingPostUI.h"
#include "UI/NewTrackerItemUI.h"
#include "UI/CompleteTrackedItemUI.h"