#pragma once

class CompleteTrackedItemUI
{
public:
  CompleteTrackedItemUI(Addon* addon);

  void Render();
  void Show();
  void Show(const CompletedItem& item);
  void ShowEdit(const CompletedItem& item, int index);
private:
  void Init();
  bool Visible = false;
  char ItemIDBuffer[128] = "";
  bool ItemIDError = false;
  int Quantity = 0;
  CurrencyInputField BuyPriceInputField;
  CurrencyInputField SellPriceInputField;
  CompletedItem Item;
  Addon* FAddon = nullptr;
  bool EditMode = false;
  int EditIndex = -1;
};