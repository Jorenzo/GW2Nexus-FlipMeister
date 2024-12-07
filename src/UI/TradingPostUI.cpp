#include "pch.h"

TradingPostUI::TradingPostUI(EntryData* entry)
{
  Entry = entry;
}

void TradingPostUI::Open()
{
  Visible = true;
}

void TradingPostUI::Render()
{
  if (Visible)
  {
    if (ImGui::Begin("Trading Post", &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
    {
      if (ImGui::Button("Refresh Current"))
      {
        Entry->Modules.CommerceData->PullCurrentBuys();
      }
      ImGui::SameLine();
      if (ImGui::Button("Refresh History"))
      {
        Entry->Modules.CommerceData->PullHistoryBuys();
      }

      if (ImGui::BeginTable("Bought Items", 5, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
      {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 350);
        ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Buy price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Total buy price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Track", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultHide, 50);
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Current Open Buy Orders");
        ImGui::TableSetColumnIndex(4);
        unsigned int counter = 0;
        for (const TransactionData& item : *Entry->Modules.CommerceData->GetCurrentBuys())
        {
          counter++;
          ItemData Data;
          if (Entry->Modules.ItemData->RequestItemData(item.ItemID, Data))
          {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (Texture* tex = Entry->APIDefs->GetTexture(Data.TextureID.c_str()))
            {
              ImGui::Image((ImTextureID)tex->Resource, ImVec2(18, 18));
              ImGui::SameLine();
            }
            ImGui::Text(Data.Name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%i", item.Quantity);
            ImGui::TableNextColumn();
            CurrencyDisplay::Render(Entry, item.Price);
            ImGui::TableNextColumn();
            CurrencyDisplay::Render(Entry, item.Price * item.Quantity);
            ImGui::TableNextColumn();
            ImGui::PushID(counter);
            if (ImGui::Button("Track"))
            {
              TrackedItem trackedItem;
              trackedItem.ItemID = item.ItemID;
              trackedItem.BuyPrice = item.Price;
              trackedItem.Quantity = item.Quantity;
              Entry->UI.NewTrackerItem->Show(trackedItem);
            }
            ImGui::PopID();
          }
        }
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Buy History");
        ImGui::TableSetColumnIndex(4);
        for (const TransactionData& item : *Entry->Modules.CommerceData->GetHistoryBuys())
        {
          counter++;
          ItemData Data;
          if (Entry->Modules.ItemData->RequestItemData(item.ItemID, Data))
          {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (Texture* tex = Entry->APIDefs->GetTexture(Data.TextureID.c_str()))
            {
              ImGui::Image((ImTextureID)tex->Resource, ImVec2(18, 18));
              ImGui::SameLine();
            }
            ImGui::Text(Data.Name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%i", item.Quantity);
            ImGui::TableNextColumn();
            CurrencyDisplay::Render(Entry, item.Price);
            ImGui::TableNextColumn();
            CurrencyDisplay::Render(Entry, item.Price * item.Quantity);
            ImGui::TableNextColumn();
            ImGui::PushID(counter);
            if (ImGui::Button("Track"))
            {
              TrackedItem trackedItem;
              trackedItem.ItemID = item.ItemID;
              trackedItem.BuyPrice = item.Price;
              trackedItem.Quantity = item.Quantity;
              Entry->UI.NewTrackerItem->Show(trackedItem);
            }
            ImGui::PopID();
          }
        }
        ImGui::EndTable();
      }
    }
    ImGui::End();
  }
}