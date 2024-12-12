#include "pch.h"

CompleteTrackedItemUI::CompleteTrackedItemUI(Addon* addon)
{
  FAddon = addon;
}

void CompleteTrackedItemUI::Render()
{
  if (Visible)
  {
    std::string WindowName = "Complete Tracked Item";
    if (EditMode)
      WindowName = "Edit Completed Item";
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

      if (ImGui::InputText("Sell Price", SellPriceBuffer, IM_ARRAYSIZE(SellPriceBuffer), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank))
      {
        char *endptr = nullptr;
        long parsed_value = std::strtol(SellPriceBuffer, &endptr, 10);
        if (endptr != SellPriceBuffer && *endptr == '\0' && parsed_value <= UINT32_MAX)
        {
          int value = static_cast<int>(parsed_value);
          if (value > 0)
          {
            Item.SellPrice = value;
            SellPriceError = false;
          }
          else
            SellPriceError = true;
        }
        else
        {
          SellPriceError = true;
        }
      }

      if (SellPriceError)
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
        ImGui::TableSetupColumn("Total Buy Price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Sell Price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Total Sell Price", ImGuiTableColumnFlags_WidthFixed, 160);
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
          ImGui::TableNextColumn();
          CurrencyDisplay::Render(FAddon, Item.SellPrice);
          ImGui::TableNextColumn();
          CurrencyDisplay::Render(FAddon, Item.SellPrice * Item.Quantity);
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
          FAddon->GetModules()->CompletedTracker->AddCompletedItem(Item);
        else
          FAddon->GetModules()->CompletedTracker->EditItem(EditIndex, Item);
        Visible = false;
      }
    }
    ImGui::End();
  }
}

void CompleteTrackedItemUI::Show()
{
  Item = {};
  EditMode = false;
  Init();
}

void CompleteTrackedItemUI::Show(const CompletedItem& item)
{
  Item = item;
  EditMode = false;
  Init();
}

void CompleteTrackedItemUI::ShowEdit(const CompletedItem& item, int index)
{
  Item = item;
  EditMode = true;
  EditIndex = index;
  Init();
}

void CompleteTrackedItemUI::Init()
{
  std::snprintf(ItemIDBuffer, sizeof(ItemIDBuffer), "%u", Item.ItemID);
  std::snprintf(QuantityBuffer, sizeof(QuantityBuffer), "%u", Item.Quantity);
  std::snprintf(BuyPriceBuffer, sizeof(BuyPriceBuffer), "%u", Item.BuyPrice);
  std::snprintf(SellPriceBuffer, sizeof(SellPriceBuffer), "%u", Item.SellPrice);
  ItemIDError = false;
  QuantityError = false;
  BuyPriceError = false;
  SellPriceError = false;
  Visible = true;
}