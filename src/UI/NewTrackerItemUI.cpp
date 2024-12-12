#include "pch.h"

NewTrackerItemUI::NewTrackerItemUI(Addon* addon)
{
  FAddon = addon;
}

void NewTrackerItemUI::Render()
{
  if (Visible)
  {
    std::string WindowName = "Add Tracker Item";
    if (EditMode)
      WindowName = "Edit Tracker Item";
    if (ImGui::Begin(WindowName.c_str(), &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
    {
      if (ImGui::InputText("Item ID", ItemIDBuffer, IM_ARRAYSIZE(ItemIDBuffer), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank))
      {
        char *endptr = nullptr;
        unsigned long parsed_value = std::strtoul(ItemIDBuffer, &endptr, 10);
        if (endptr != ItemIDBuffer && *endptr == '\0' && parsed_value <= UINT32_MAX)
        {
          Item.ItemID = static_cast<unsigned int>(parsed_value);
          ItemIDError = false;
        }
        else
        {
          ItemIDError = true;
        }
      }

      if (ItemIDError)
      {
        ImGui::PushStyleColor(ImGuiCol_Text, COL_RED);
        ImGui::Text("Invalid Value");
        ImGui::PopStyleColor();
      }

      if (ImGui::InputText("Quantity", QuantityBuffer, IM_ARRAYSIZE(QuantityBuffer), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank))
      {
        char *endptr = nullptr;
        unsigned long parsed_value = std::strtoul(QuantityBuffer, &endptr, 10);
        if (endptr != QuantityBuffer && *endptr == '\0' && parsed_value <= UINT32_MAX)
        {
          unsigned int value = static_cast<unsigned int>(parsed_value);
          if (value > 0)
          {
            Item.Quantity = value;
            QuantityError = false;
          }
          else
            QuantityError = true;
        }
        else
        {
          QuantityError = true;
        }
      }

      if (QuantityError)
      {
        ImGui::PushStyleColor(ImGuiCol_Text, COL_RED);
        ImGui::Text("Invalid Value");
        ImGui::PopStyleColor();
      }

      if (ImGui::InputText("Buy Price", BuyPriceBuffer, IM_ARRAYSIZE(BuyPriceBuffer), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank))
      {
        char *endptr = nullptr;
        long parsed_value = std::strtol(BuyPriceBuffer, &endptr, 10);
        if (endptr != BuyPriceBuffer && *endptr == '\0' && parsed_value <= UINT32_MAX)
        {
          int value = static_cast<int>(parsed_value);
          if (value > 0)
          {
            Item.BuyPrice = value;
            BuyPriceError = false;
          }
          else
            BuyPriceError = true;
        }
        else
        {
          BuyPriceError = true;
        }
      }

      if (BuyPriceError)
      {
        ImGui::PushStyleColor(ImGuiCol_Text, COL_RED);
        ImGui::Text("Invalid Value");
        ImGui::PopStyleColor();
      }

      ImGui::Separator();

      if (ImGui::BeginTable("Tracked Items", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
      {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 350);
        ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Buy price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Total buy price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ItemData Data;
        if (Item.ItemID != 0 && FAddon->GetModules()->ItemData->RequestItemData(Item.ItemID, Data))
        {
          if (Texture* tex = FAddon->GetAPI()->GetTexture(Data.TextureID.c_str()))
          {
            ImGui::Image((ImTextureID)tex->Resource, ImVec2(18, 18));
            ImGui::SameLine();
          }
          ImGui::Text(Data.Name.c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%i", Item.Quantity);
          ImGui::TableNextColumn();
          CurrencyDisplay::Render(FAddon, Item.BuyPrice);
          ImGui::TableNextColumn();
          CurrencyDisplay::Render(FAddon, Item.BuyPrice * Item.Quantity);
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Cancel"))
      {
        Visible = false;
      }
      ImGui::SameLine();
      if (ImGui::Button("Confirm"))
      {
        if (!EditMode)
          FAddon->GetModules()->Tracker->TrackItem(Item);
        else
          FAddon->GetModules()->Tracker->EditItem(EditIndex, Item);
        Visible = false;
      }
    }
    ImGui::End();
  }
}

void NewTrackerItemUI::Show()
{
  Item = {};
  EditMode = false;
  Init();
}

void NewTrackerItemUI::Show(const TrackedItem& item)
{
  Item = item;
  EditMode = false;
  Init();
}

void NewTrackerItemUI::ShowEdit(const TrackedItem& item, int index)
{
  Item = item;
  EditMode = true;
  EditIndex = index;
  Init();
}

void NewTrackerItemUI::Init()
{
  std::snprintf(ItemIDBuffer, sizeof(ItemIDBuffer), "%u", Item.ItemID);
  std::snprintf(QuantityBuffer, sizeof(QuantityBuffer), "%u", Item.Quantity);
  std::snprintf(BuyPriceBuffer, sizeof(BuyPriceBuffer), "%u", Item.BuyPrice);
  ItemIDError = false;
  QuantityError = false;
  BuyPriceError = false;
  Visible = true;
}