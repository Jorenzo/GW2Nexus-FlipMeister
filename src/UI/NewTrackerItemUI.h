#pragma once

class NewTrackerItemUI
{
public:
  NewTrackerItemUI(Addon* addon);

  void Render();
  void Show();
  void Show(const TrackedItem& item);
  void ShowEdit(const TrackedItem& item, int index);
private:
  void Init();
  bool Visible = false;
  CurrencyInputField BuyPriceInputField;
  char ItemIDBuffer[128] = "";
  bool ItemIDError = false;
  int Quantity = 0;
  TrackedItem Item;
  Addon* FAddon = nullptr;
  bool EditMode = false;
  int EditIndex = -1;
};