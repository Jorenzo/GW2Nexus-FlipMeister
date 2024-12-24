#include "pch.h"

CurrencyInputField::CurrencyInputField(Addon* addon) :
  FAddon(addon)
{

}

void CurrencyInputField::SetValue(int value)
{
  int absValue = abs(value);
  Copper = absValue % 100;
  absValue /= 100;
  Silver = absValue % 100;
  Gold = absValue / 100;
}


bool CurrencyInputField::Render(int& outValue, int minValue, const std::string& text)
{
  bool InputThisFrame = false;
  ImGui::PushStyleColor(ImGuiCol_Text, COL_GOLD);
  std::string id0 = text + "0";
  ImGui::PushID(id0.c_str());
  ImGui::SetNextItemWidth(80.0f * GetScaleRatio());
  if (ImGui::InputInt("", &Gold))
  {
    InputThisFrame = true;
    Gold = std::max(Gold, 0);
  }
  ImGui::PopID();
  ImGui::PopStyleColor();
  if (Texture* tex = FAddon->GetAPI()->GetTexture(TEX_GOLD))
  {
    ImGui::SameLine();
    ImGui::Image((ImTextureID)tex->Resource, ImVec2(18.0f * GetScaleRatio(), 18.0f * GetScaleRatio()));
  }
  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Text, COL_SILVER);
  std::string id1 = text + "1";
  ImGui::PushID(id1.c_str());
  ImGui::SetNextItemWidth(80.0f * GetScaleRatio());
  if (ImGui::InputInt("", &Silver))
  {
    Silver = std::clamp(Silver, 0, 99);
    InputThisFrame = true;
  }
  ImGui::PopID();
  ImGui::PopStyleColor();
  if (Texture* tex = FAddon->GetAPI()->GetTexture(TEX_SILVER))
  {
    ImGui::SameLine();
    ImGui::Image((ImTextureID)tex->Resource, ImVec2(18.0f * GetScaleRatio(), 18.0f * GetScaleRatio()));
  }
  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Text, COL_COPPER);
  std::string id2 = text + "2";
  ImGui::PushID(id2.c_str());
  ImGui::SetNextItemWidth(80.0f * GetScaleRatio());
  if (ImGui::InputInt("", &Copper))
  {
    Copper = std::clamp(Copper, 0, 99);
    InputThisFrame = true;
  }
  ImGui::PopID();
  ImGui::PopStyleColor();
  if (Texture* tex = FAddon->GetAPI()->GetTexture(TEX_COPPER))
  {
    ImGui::SameLine();
    ImGui::Image((ImTextureID)tex->Resource, ImVec2(18.0f * GetScaleRatio(), 18.0f * GetScaleRatio()));
  }

  ImGui::SameLine();
  ImGui::Text(text.c_str());

  bool Error = false;

  outValue = Copper;
  outValue += Silver * 100;
  outValue += Gold * 10000;

  if (outValue < minValue)
    SetValue(minValue);

  if (InputThisFrame)
    return true;

  return false;
}