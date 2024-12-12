#include "pch.h"

TrackerModule::TrackerModule(Addon* addon)
{
  FAddon = addon;
  ReadFromFile();
}

void TrackerModule::TrackItem(const TrackedItem& item)
{
  TrackedItems.push_back(item);
  WriteToFile();
}

void TrackerModule::EditItem(int index, const TrackedItem& item)
{
  if (index < TrackedItems.size() && index >= 0)
  {
    TrackedItems[index] = item;
    WriteToFile();
  }
}

void TrackerModule::RemoveTrackedItem(int index)
{
  if (index < TrackedItems.size() && index >= 0)
  {
    TrackedItems.erase(TrackedItems.begin() + index);
    WriteToFile();
  }
}

void TrackerModule::ReadFromFile()
{
  std::string path = FAddon->GetAPI()->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\tracker.json";
  std::ifstream inFile(path);

  if (!inFile) 
  {
    FAddon->Log(WARNING, "Failed to open tracker file at '%s'", path.c_str());
    return;
  }

  nlohmann::json jsonObject;
  try
  {
    inFile >> jsonObject;
  }
  catch (const nlohmann::json::parse_error& e) 
  {
    FAddon->Log(WARNING, "Failed to parse JSON in tracker file '%s'\n %s", path.c_str(), e.what());
    return;
  }

  inFile.close();

  try 
  {
    TrackedItems = jsonObject.get<std::vector<TrackedItem>>();
  }
  catch (const nlohmann::json::type_error& e) 
  {
    FAddon->Log(WARNING, "JSON structure mismatch in tracker file '%s'\n %s", path.c_str(), e.what());
    return;
  }
}

void TrackerModule::WriteToFile()
{
  nlohmann::json data = TrackedItems;

  std::string path = FAddon->GetAPI()->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\tracker.json";

  std::filesystem::path dirPath = std::filesystem::path(path).parent_path();

  // Check if the directory exists, and create it if it doesn't
  if (!std::filesystem::exists(dirPath)) 
  {
    if (!std::filesystem::create_directories(dirPath)) 
    {
      FAddon->Log( WARNING, "Failed to write tracker file at '%s'", path.c_str());
    }
  }


  try 
  {
    std::ofstream outFile(path);
    if (!outFile.is_open()) 
    {
      throw std::ios_base::failure("Failed to open file");
    }
    outFile << data.dump(4);
    outFile.close();
  }
  catch (const std::exception& e) 
  {
    FAddon->Log(WARNING, "Failed to write tracker at file '%s'\n%s", path.c_str(), e.what());
  }
}