#pragma once

class CurrencyInputField
{
public:
  CurrencyInputField(Addon* addon);
  bool Render(int& outValue, int minValue, const std::string& text);
  void SetValue(int value);
private:
  Addon* FAddon;
  bool GoldError = false;
  bool SilverError = false;
  bool CopperError = false;
  int Gold = 0;
  int Silver = 0;
  int Copper = 0;
};