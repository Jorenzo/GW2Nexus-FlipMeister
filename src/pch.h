#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include <urlmon.h>
#include <sstream>
#include <chrono>

#include "../assets/resource.h"

#include "nexus/Nexus.h"
//#include "mumble/Mumble.h"
#include "imgui/imgui.h"
#include "util/Log.h"

#include "util/json.hpp"
#include "util/HTTPClient.h"
#include "util/Settings.h"
#include "util/GW2API.h"
#include "util/Colors.h"
#include "Util/Timer.h"

#include "EntryData.h"

#include "Modules/ItemDataModule.h"
#include "Modules/CommerceDataModule.h"
#include "Modules/TrackerModule.h"
#include "Modules/CompletedTrackerModule.h"

#include "UI/CurrencyDisplay.h"
#include "UI/TrackerUI.h"
#include "UI/CompletedTrackerUI.h"
#include "UI/TradingPostUI.h"
#include "UI/NewTrackerItemUI.h"
#include "UI/CompleteTrackedItemUI.h"