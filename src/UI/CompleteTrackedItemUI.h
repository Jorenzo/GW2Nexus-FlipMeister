#pragma once

class CompleteTrackedItemUI
{
public:
  CompleteTrackedItemUI(EntryData* entry);

  void Render();
  void Show();
  void Show(const CompletedItem& item);
  void ShowEdit(const CompletedItem& item, int index);
private:
  void Init();
  bool Visible = false;
  char ItemIDBuffer[128] = "";
  char QuantityBuffer[128] = "";
  char BuyPriceBuffer[128] = "";
  char SellPriceBuffer[128] = "";
  bool ItemIDError = false;
  bool QuantityError = false;
  bool BuyPriceError = false;
  bool SellPriceError = false;
  CompletedItem Item;
  EntryData* Entry = nullptr;
  bool EditMode = false;
  int EditIndex = -1;
};