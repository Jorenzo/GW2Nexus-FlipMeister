#include "pch.h"

CompletedTrackerModule::CompletedTrackerModule(Addon* addon)
{
  FAddon = addon;
  ReadFromFile();
}

void CompletedTrackerModule::AddCompletedItem(const CompletedItem& item)
{
  CompletedItems.push_back(item);
  WriteToFile();
}

void CompletedTrackerModule::EditItem(int index, const CompletedItem& item)
{
  if (index < CompletedItems.size() && index >= 0)
  {
    CompletedItems[index] = item;
    WriteToFile();
  }
}

void CompletedTrackerModule::RemoveCompletedItem(int index)
{
  if (index < CompletedItems.size() && index >= 0)
  {
    CompletedItems.erase(CompletedItems.begin() + index);
    WriteToFile();
  }
}

void CompletedTrackerModule::ReadFromFile()
{
  std::string path = FAddon->GetAPI()->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\completed.json";
  std::ifstream inFile(path);

  if (!inFile) 
  {
    FAddon->Log(WARNING, "Failed to open completed tracker file at '%s'", path.c_str());
    return;
  }

  nlohmann::json jsonObject;
  try
  {
    inFile >> jsonObject;
  }
  catch (const nlohmann::json::parse_error& e) 
  {
    FAddon->Log(WARNING, "Failed to parse JSON in completed tracker file '%s'\n %s", path.c_str(), e.what());
    return;
  }

  inFile.close();

  try
  {
    CompletedItems = jsonObject.get<std::vector<CompletedItem>>();
  }
  catch (const nlohmann::json::type_error& e) 
  {
    FAddon->Log(WARNING, "JSON structure mismatch in completed tracker file '%s'\n %s", path.c_str(), e.what());
    return;
  }
}

void CompletedTrackerModule::WriteToFile()
{
  nlohmann::json data = CompletedItems;

  std::string path = FAddon->GetAPI()->GetAddonDirectory(ADDON_DIRECTORY_NAME);
  path += "\\completed.json";

  std::filesystem::path dirPath = std::filesystem::path(path).parent_path();

  // Check if the directory exists, and create it if it doesn't
  if (!std::filesystem::exists(dirPath)) 
  {
    if (!std::filesystem::create_directories(dirPath)) 
    {
      FAddon->Log(WARNING, "Failed to write completed tracker file at '%s'", path.c_str());
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
    FAddon->Log(WARNING, "Failed to write completed tracker at file '%s'\n%s", path.c_str(), e.what());
  }
}