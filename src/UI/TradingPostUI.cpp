#include "pch.h"

TradingPostUI::TradingPostUI(Addon* addon)
{
  FAddon = addon;
  BuyingRefreshTimer.SetNow();
  BoughtRefreshTimer.SetNow();
}

void TradingPostUI::Open()
{
  Visible = true;
}

void TradingPostUI::Render()
{
  int AutoUpdateTime = FAddon->GetSettings()->AutoUpdateTradingPostSeconds();
  bool AutoUpdate = FAddon->GetSettings()->AutoUpdateTradingPost();

  if (Visible)
  {
    if (ImGui::Begin("Trading Post", &Visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
    {
      if (ImGui::BeginTabBar("Trading Post Tab bar"))
      {
        if (ImGui::BeginTabItem("Buying"))
        {
          if (AutoUpdate && BuyingRefreshTimer.GetSecondsPassed() > AutoUpdateTime)
          {
            FAddon->GetModules()->CommerceData->PullCurrentBuys();
            BuyingRefreshTimer.SetNow();
          }

          if (ImGui::Button("Refresh"))
          {
            FAddon->GetModules()->CommerceData->PullCurrentBuys();
            BuyingRefreshTimer.SetNow();
          }
          if (FAddon->GetModules()->CommerceData->IsUpdatingCurrenBuys())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Transactions...");
          }
          else if (FAddon->GetModules()->ItemData->IsUpdatingItems())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Items...");
          }

          RenderTransactionsTable(FAddon->GetModules()->CommerceData->GetCurrentBuys(), TradingPostTableType_Buying);

          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Selling"))
        {
          if (AutoUpdate && SellingRefreshTimer.GetSecondsPassed() > AutoUpdateTime)
          {
            FAddon->GetModules()->CommerceData->PullCurrentSells();
            SellingRefreshTimer.SetNow();
          }

          if (ImGui::Button("Refresh"))
          {
            FAddon->GetModules()->CommerceData->PullCurrentSells();
            SellingRefreshTimer.SetNow();
          }
          if (FAddon->GetModules()->CommerceData->IsUpdatingCurrenSells())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Transactions...");
          }
          else if (FAddon->GetModules()->ItemData->IsUpdatingItems())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Items...");
          }

          RenderTransactionsTable(FAddon->GetModules()->CommerceData->GetCurrentSells(), TradingPostTableType_Selling);
          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Bought"))
        {
          if (AutoUpdate && BoughtRefreshTimer.GetSecondsPassed() > AutoUpdateTime)
          {
            FAddon->GetModules()->CommerceData->PullHistoryBuys();
            BoughtRefreshTimer.SetNow();
          }

          if (ImGui::Button("Refresh"))
          {
            FAddon->GetModules()->CommerceData->PullHistoryBuys();
            BoughtRefreshTimer.SetNow();
          }
          if (FAddon->GetModules()->CommerceData->IsUpdatingHistoryBuys())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Transactions...");
          }
          else if (FAddon->GetModules()->ItemData->IsUpdatingItems())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Items...");
          }

          RenderTransactionsTable(FAddon->GetModules()->CommerceData->GetHistoryBuys(), TradingPostTableType_Bought);

          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sold"))
        {
          if (AutoUpdate && SoldRefreshTimer.GetSecondsPassed() > AutoUpdateTime)
          {
            FAddon->GetModules()->CommerceData->PullHistorySells();
            SoldRefreshTimer.SetNow();
          }

          if (ImGui::Button("Refresh"))
          {
            FAddon->GetModules()->CommerceData->PullHistorySells();
            SoldRefreshTimer.SetNow();
          }
          if (FAddon->GetModules()->CommerceData->IsUpdatingHistorySells())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Transactions...");
          }
          else if (FAddon->GetModules()->ItemData->IsUpdatingItems())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Items...");
          }

          RenderTransactionsTable(FAddon->GetModules()->CommerceData->GetHistorySells(), TradingPostTableType_Sold);

          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Pickup"))
        {
          if (AutoUpdate && DeliveryRefreshTimer.GetSecondsPassed() > AutoUpdateTime)
          {
            FAddon->GetModules()->CommerceData->PullDelivery();
            DeliveryRefreshTimer.SetNow();
          }

          if (ImGui::Button("Refresh"))
          {
            FAddon->GetModules()->CommerceData->PullDelivery();
            DeliveryRefreshTimer.SetNow();
          }
          if (FAddon->GetModules()->CommerceData->IsUpdatingDelivery())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Delivery...");
          }
          else if (FAddon->GetModules()->ItemData->IsUpdatingItems())
          {
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - (180.0f * GetScaleRatio()));
            ImGui::SetNextItemWidth(180.0f * GetScaleRatio());
            ImGui::Text("Fetching Items...");
          }

          ImGui::Text("Pickup: ");
          ImGui::SameLine();
          CurrencyDisplay::Render(FAddon, FAddon->GetModules()->CommerceData->GetDeliveryData()->Coins);

          if (ImGui::BeginTable("Items", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
          {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 350 * GetScaleRatio());
            ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 70 * GetScaleRatio());
            ImGui::TableSetupColumn("Track", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultHide, 50 * GetScaleRatio());
            ImGui::TableHeadersRow();

            unsigned int counter = 0;
            for (const DeliveryItemData& item : FAddon->GetModules()->CommerceData->GetDeliveryData()->Items)
            {
              counter++;
              ItemData Data;
              if (FAddon->GetModules()->ItemData->RequestItemData(item.ItemID, Data))
              {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (Texture* tex = FAddon->GetAPI()->GetTexture(Data.TextureID.c_str()))
                {
                  ImGui::Image((ImTextureID)tex->Resource, ImVec2(18.0f * GetScaleRatio(), 18.0f * GetScaleRatio()));
                  ImGui::SameLine();
                }
                ImGui::Text(Data.Name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%i", item.Quantity);
                ImGui::TableNextColumn();
                ImGui::PushID(counter);
                if (ImGui::Button("Track"))
                {
                  TrackedItem trackedItem;
                  trackedItem.ItemID = item.ItemID;
                  trackedItem.BuyPrice = 1;
                  trackedItem.Quantity = item.Quantity;
                  FAddon->GetUI()->NewTrackerItem->Show(trackedItem);
                }
                ImGui::PopID();
              }
            }
            ImGui::EndTable();
          }
          ImGui::EndTabItem();
        }
        // End the tab bar
        ImGui::EndTabBar();
      }
    }
    ImGui::End();
  }
}

void TradingPostUI::RenderTransactionsTable(const std::vector<TransactionData>* transactions, TradingPostTableType type)
{
  const float TabWidth = 850.0f;
  std::string TableName = "Items";
  std::string ChildName = "ItemsChild";
  TableName += std::to_string(type);
  ChildName += std::to_string(type);
  if (ImGui::BeginTable(TableName.c_str(), 5, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY, ImVec2(TabWidth * GetScaleRatio(), 600.0f * GetScaleRatio())))
  {
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 350 * GetScaleRatio());
    ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 70 * GetScaleRatio());
    switch (type)
    {
    case TradingPostTableType_Buying:
    case TradingPostTableType_Selling:
      ImGui::TableSetupColumn("Listing price", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
      ImGui::TableSetupColumn("Total listing price", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
      break;
    case TradingPostTableType_Bought:
      ImGui::TableSetupColumn("Bought for", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
      ImGui::TableSetupColumn("Total Bought for", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
      break;
    case TradingPostTableType_Sold:
      ImGui::TableSetupColumn("Sold for", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
      ImGui::TableSetupColumn("Total sold for", ImGuiTableColumnFlags_WidthFixed, 160 * GetScaleRatio());
      break;
    default:
      break;
    }
    ImGui::TableSetupColumn("Track", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultHide, 50 * GetScaleRatio());
    ImGui::TableHeadersRow();
    unsigned int counter = 0;
    for (const TransactionData& item : *transactions)
    {
      counter++;
      ItemData Data;
      if (FAddon->GetModules() && FAddon->GetModules()->ItemData && FAddon->GetModules()->ItemData->RequestItemData(item.ItemID, Data))
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (Texture* tex = FAddon->GetAPI()->GetTexture(Data.TextureID.c_str()))
        {
          ImGui::Image((ImTextureID)tex->Resource, ImVec2(18 * GetScaleRatio(), 18 * GetScaleRatio()));
          ImGui::SameLine();
        }
        ImGui::Text(Data.Name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%i", item.Quantity);
        ImGui::TableNextColumn();
        CurrencyDisplay::Render(FAddon, item.Price);
        ImGui::TableNextColumn();
        CurrencyDisplay::Render(FAddon, item.Price * item.Quantity);
        ImGui::TableNextColumn();
        ImGui::PushID(counter);
        if (ImGui::Button("Track"))
        {
          TrackedItem trackedItem;
          trackedItem.ItemID = item.ItemID;
          trackedItem.BuyPrice = item.Price;
          trackedItem.Quantity = item.Quantity;
          FAddon->GetUI()->NewTrackerItem->Show(trackedItem);
        }
        ImGui::PopID();
      }
    }

    ImGui::EndTable();
  }
}

