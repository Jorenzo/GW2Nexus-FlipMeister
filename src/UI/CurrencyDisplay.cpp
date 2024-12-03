#include "pch.h"

void CurrencyDisplay::SetupResources(EntryData* Entry)
{
  Entry->APIDefs->LoadTextureFromURL(TEX_GOLD, "https://wiki.guildwars2.com", "/images/thumb/d/d1/Gold_coin.png/18px-Gold_coin.png", nullptr);
  Entry->APIDefs->LoadTextureFromURL(TEX_SILVER, "https://wiki.guildwars2.com", "/images/thumb/3/3c/Silver_coin.png/18px-Silver_coin.png", nullptr);
  Entry->APIDefs->LoadTextureFromURL(TEX_COPPER, "https://wiki.guildwars2.com", "/images/thumb/e/eb/Copper_coin.png/18px-Copper_coin.png", nullptr);
}

void CurrencyDisplay::Render(EntryData* Entry, int value)
{
  int absValue = abs(value);
  int copper = absValue % 100;
  absValue /= 100;
  int silver = absValue % 100;
  int gold = absValue / 100;

  bool sign = (value < 0);

  if (gold > 0)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, COL_GOLD);
    if (sign)
      ImGui::Text("-%i", gold);
    else
      ImGui::Text("%i", gold);
    ImGui::PopStyleColor();
    if (Texture* tex = Entry->APIDefs->GetTexture(TEX_GOLD))
    {
      ImGui::SameLine();
      ImGui::Image((ImTextureID)tex->Resource, ImVec2(18, 18));
    }
    ImGui::SameLine();
  }
  if (gold > 0 || silver > 0)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, COL_SILVER);
    if (sign && gold == 0)
      ImGui::Text("-%i", silver);
    else
      ImGui::Text("%i", silver);
    ImGui::PopStyleColor();
    if (Texture* tex = Entry->APIDefs->GetTexture(TEX_SILVER))
    {
      ImGui::SameLine();
      ImGui::Image((ImTextureID)tex->Resource, ImVec2(18, 18));
    }
    ImGui::SameLine();
  }
  ImGui::PushStyleColor(ImGuiCol_Text, COL_COPPER);
  if (sign && gold == 0 && silver == 0)
    ImGui::Text("-%i", copper);
  else
    ImGui::Text("%i", copper);
  ImGui::PopStyleColor();
  if (Texture* tex = Entry->APIDefs->GetTexture(TEX_COPPER))
  {
    ImGui::SameLine();
    ImGui::Image((ImTextureID)tex->Resource, ImVec2(18, 18));
  }
}