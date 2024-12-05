#include "pch.h"

CompletedTrackerUI::CompletedTrackerUI(EntryData* entry)
{
  Entry = entry;
}

void CompletedTrackerUI::Open()
{
  Visible = true;
}

void CompletedTrackerUI::Render()
{
  if (Visible)
  {

    if (ImGui::Begin("Completed Items", &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))

    {
      if (ImGui::BeginTable("Completed Items", 12, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
      {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 350);
        ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Buy price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Total buy price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 10);
        ImGui::TableSetupColumn("Sell price", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("After Tax", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Total After Tax", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("RoI%", ImGuiTableColumnFlags_WidthFixed, 50);
        ImGui::TableSetupColumn("Profit", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Edit", ImGuiTableColumnFlags_WidthFixed, 30);
        ImGui::TableSetupColumn("Del", ImGuiTableColumnFlags_WidthFixed, 45);
        ImGui::TableHeadersRow();

        int ItemToRemove = -1;
        for (int i = 0; i < Entry->Modules.CompletedTracker->GetCompletedItems()->size(); ++i)
        {
          const CompletedItem& item = Entry->Modules.CompletedTracker->GetCompletedItems()->at(i);

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
            CurrencyDisplay::Render(Entry, item.BuyPrice);
            ImGui::TableNextColumn();
            int TotalBuyPrice = item.BuyPrice * item.Quantity;
            CurrencyDisplay::Render(Entry, TotalBuyPrice);
            ImGui::TableNextColumn();
            //separator
            ImGui::TableNextColumn();
            CurrencyDisplay::Render(Entry, item.SellPrice);
            ImGui::TableNextColumn();
            int AfterTax = (int)((float)item.SellPrice * 0.85f);
            CurrencyDisplay::Render(Entry, AfterTax);
            ImGui::TableNextColumn();
            float TotalAfterTax = (item.SellPrice * item.Quantity) * 0.85f;
            CurrencyDisplay::Render(Entry, (int)TotalAfterTax);
            ImGui::TableNextColumn();
            float RoI = (float)AfterTax / item.BuyPrice;
            RoI -= 1.f; //1.15 to 0.15;
            RoI *= 100; //make it 15%
            if (RoI > 15.0f)
              ImGui::PushStyleColor(ImGuiCol_Text, COL_GREEN);
            else if (RoI > 5.0f)
              ImGui::PushStyleColor(ImGuiCol_Text, COL_YELLOW);
            else if (RoI > 0.0f)
              ImGui::PushStyleColor(ImGuiCol_Text, COL_ORANGE);
            else
              ImGui::PushStyleColor(ImGuiCol_Text, COL_RED);
            ImGui::Text("%.2f%%", RoI);
            ImGui::PopStyleColor();
            ImGui::TableNextColumn();
            int TotalProfit = (int)TotalAfterTax - TotalBuyPrice;
            CurrencyDisplay::Render(Entry, TotalProfit);
            ImGui::TableNextColumn();
            ImGui::PushID(i);
            if (ImGui::Button("Edit"))
              Entry->UI.CompleteTrackedItem->ShowEdit(item, i);
            ImGui::PopID();
            ImGui::TableNextColumn();
            ImGui::PushID(i);
            if (RemovedIndex != i && ImGui::Button("X"))
              RemovedIndex = i;
            if (RemovedIndex == i)
            {
              if (ImGui::Button("N"))
                RemovedIndex = -1;
              ImGui::SameLine();
              if (ImGui::Button("Y"))
              {
                ItemToRemove = i;
                RemovedIndex = -1;
              }
            }
            ImGui::PopID();
          }
        }
        ImGui::TableNextRow();
        ImGui::Separator();
        ImGui::TableNextColumn();
        ImGui::Text("Total:");
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();
        unsigned int TotalBuyPrice = 0;
        for (int i = 0; i < Entry->Modules.CompletedTracker->GetCompletedItems()->size(); ++i)
        {
          const CompletedItem& item = Entry->Modules.CompletedTracker->GetCompletedItems()->at(i);
          TotalBuyPrice += (item.BuyPrice * item.Quantity);
        }
        CurrencyDisplay::Render(Entry, TotalBuyPrice);
        ImGui::EndTable();


        if (ItemToRemove != -1)
        {
          Entry->Modules.CompletedTracker->RemoveCompletedItem(ItemToRemove);
        }

        ImGui::End();
      }
    }
  }
}