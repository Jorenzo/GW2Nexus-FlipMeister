#include "pch.h"

CompleteTrackedItemUI::CompleteTrackedItemUI(Addon* addon) :
  BuyPriceInputField(addon),
  SellPriceInputField(addon)
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

      if (FAddon->GetModules()->ItemData->IsUpdatingItems())
      {
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - (150.0f * GetScaleRatio()));
        ImGui::SetNextItemWidth(150.0f * GetScaleRatio());
        ImGui::Text("Fetching Items...");
      }

      if (ItemIDError)
      {
        ImGui::PushStyleColor(ImGuiCol_Text, COL_RED);
        ImGui::Text("Invalid Value");
        ImGui::PopStyleColor();
      }

      if (ImGui::InputInt("Quantity", &Quantity))
      {
        Quantity = std::max(Quantity, 1);
        Item.Quantity = Quantity;
      }

      int outBuyPrice = 0;
      if (BuyPriceInputField.Render(outBuyPrice, 1, "Buy Price"))
      {
        Item.BuyPrice = outBuyPrice;
      }

      int outSellPrice = 0;
      if (SellPriceInputField.Render(outSellPrice, 1, "Sell Price"))
      {
        Item.SellPrice = outSellPrice;
      }

      ImGui::Separator();

      bool HasItem = false;

      if (ImGui::BeginTable("Tracked Items", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
      {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 350 * GetScaleRatio());
        ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 70 * GetScaleRatio());
        ImGui::TableSetupColumn("Buy price", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
        ImGui::TableSetupColumn("Total Buy Price", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
        ImGui::TableSetupColumn("Sell Price", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
        ImGui::TableSetupColumn("Total Sell Price", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ItemData Data;
        if (Item.ItemID != 0 && FAddon->GetModules()->ItemData->RequestItemData(Item.ItemID, Data))
        {
          HasItem = true;
          if (Texture* tex = FAddon->GetAPI()->GetTexture(Data.TextureID.c_str()))
          {
            ImGui::Image((ImTextureID)tex->Resource, ImVec2(18.0f * GetScaleRatio(), 18.0f * GetScaleRatio()));
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

      if (HasItem)
      {
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
  Item.Quantity = std::max(Item.Quantity, 1u);
  Item.BuyPrice = std::max(Item.BuyPrice, 1);
  Item.SellPrice = std::max(Item.SellPrice, 1);

  std::snprintf(ItemIDBuffer, sizeof(ItemIDBuffer), "%u", Item.ItemID);
  Quantity = Item.Quantity;
  BuyPriceInputField.SetValue(Item.BuyPrice);
  SellPriceInputField.SetValue(Item.SellPrice);
  ItemIDError = false;
  Visible = true;
}