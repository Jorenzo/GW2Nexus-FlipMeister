#pragma once

#define TEX_COPPER "TEX_COPPER"
#define TEX_SILVER "TEX_SILVER"
#define TEX_GOLD "TEX_GOLD"

class CurrencyDisplay
{
public:
  static void SetupResources(Addon* addon);
  static void Render(Addon* addon, int value);
};