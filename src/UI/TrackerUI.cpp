#include "pch.h"

TrackerUI::TrackerUI(Addon* addon)
{
  FAddon = addon;
}

void TrackerUI::Render()
{
  ImGui::Separator();
  ImGui::Separator();
  ImGui::Text("Tracker");
  ImGui::Separator();
  if (ImGui::Button("Add Item Manually", ImVec2(150, 20)))
  {
    FAddon->GetUI()->NewTrackerItem->Show();
  }

  if (ImGui::BeginTable("Tracked Items", 13, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
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
    ImGui::TableSetupColumn("Resolve", ImGuiTableColumnFlags_WidthFixed, 60);
    ImGui::TableSetupColumn("Del", ImGuiTableColumnFlags_WidthFixed, 45);
    ImGui::TableHeadersRow();

    int ItemToRemove = -1;

    int AllTotalBuyPrice = 0;
    int AllAfterTax = 0;
    int AllProfit = 0;
    for (int i = 0; i < FAddon->GetModules()->Tracker->GetTrackedItems()->size(); ++i)
    {
      const TrackedItem& item = FAddon->GetModules()->Tracker->GetTrackedItems()->at(i);

      ItemData Data;
      if (FAddon->GetModules()->ItemData->RequestItemData(item.ItemID, Data))
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (Texture* tex = FAddon->GetAPI()->GetTexture(Data.TextureID.c_str()))
        {
          ImGui::Image((ImTextureID)tex->Resource, ImVec2(18, 18));
          ImGui::SameLine();
        }
        ImGui::Text(Data.Name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%i", item.Quantity);
        ImGui::TableNextColumn();
        CurrencyDisplay::Render(FAddon, item.BuyPrice);
        ImGui::TableNextColumn();
        int TotalBuyPrice = item.BuyPrice * item.Quantity;
        AllTotalBuyPrice += TotalBuyPrice;
        CurrencyDisplay::Render(FAddon, TotalBuyPrice);
        ImGui::TableNextColumn();
        //separator
        ImGui::TableNextColumn();
        int SellPrice = FAddon->GetModules()->CommerceData->GetSellPrice(item.ItemID);
        CurrencyDisplay::Render(FAddon, SellPrice);
        ImGui::TableNextColumn();
        int AfterTax = (int)((float)SellPrice * 0.85f);
        CurrencyDisplay::Render(FAddon, SellPrice != 0 ? AfterTax : 0);
        ImGui::TableNextColumn();
        float TotalAfterTax = (SellPrice * item.Quantity) * 0.85f;
        AllAfterTax += SellPrice != 0 ? (int)TotalAfterTax : 0;
        CurrencyDisplay::Render(FAddon, SellPrice != 0 ? (int)TotalAfterTax : 0);
        ImGui::TableNextColumn();
        if (SellPrice != 0)
        {
          float RoI = (float)TotalAfterTax / TotalBuyPrice;
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
        }
        else
          ImGui::Text("0%%");
        ImGui::TableNextColumn();
        int TotalProfit = (int)TotalAfterTax - TotalBuyPrice;
        if (SellPrice != 0)
          AllProfit += TotalProfit;
        CurrencyDisplay::Render(FAddon, SellPrice != 0 ? TotalProfit : 0);
        ImGui::TableNextColumn();
        ImGui::PushID(i);
        if (ImGui::Button("Edit"))
          FAddon->GetUI()->NewTrackerItem->ShowEdit(item, i);
        ImGui::PopID();
        ImGui::TableNextColumn();
        ImGui::PushID(i);
        if (ImGui::Button("Resolve"))
        {
          CompletedItem comp;
          comp.ItemID = item.ItemID;
          comp.Quantity = item.Quantity;
          comp.BuyPrice = item.BuyPrice;
          comp.SellPrice = FAddon->GetModules()->CommerceData->GetSellPrice(item.ItemID);
          FAddon->GetUI()->CompleteTrackedItem->Show(comp);
        }
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
    CurrencyDisplay::Render(FAddon, AllTotalBuyPrice);
    ImGui::TableNextColumn();
    ImGui::TableNextColumn();
    ImGui::TableNextColumn();
    ImGui::TableNextColumn();
    CurrencyDisplay::Render(FAddon, AllAfterTax);
    ImGui::TableNextColumn();
    if (AllAfterTax != 0)
    {
      float RoI = (float)AllAfterTax / AllTotalBuyPrice;
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
    }
    else
      ImGui::Text("0%%");
    ImGui::TableNextColumn();
    CurrencyDisplay::Render(FAddon, AllProfit);
    ImGui::EndTable();


    if (ItemToRemove != -1)
    {
      FAddon->GetModules()->Tracker->RemoveTrackedItem(ItemToRemove);
    }

  }
}