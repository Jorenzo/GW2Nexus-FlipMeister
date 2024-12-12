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
  char ItemIDBuffer[128] = "";
  char QuantityBuffer[128] = "";
  char BuyPriceBuffer[128] = "";
  bool ItemIDError = false;
  bool QuantityError = false;
  bool BuyPriceError = false;
  TrackedItem Item;
  Addon* FAddon = nullptr;
  bool EditMode = false;
  int EditIndex = -1;
};