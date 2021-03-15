#include "Fonts/Cantarell_Regular_euro8pt8b.h"
#include "Fonts/Cantarell_Regular_euro10pt8b.h"
#include "Fonts/Cantarell_Regular_euro12pt8b.h"
#include "Fonts/Cantarell_Bold_euro12pt8b.h"

#include "icons/btc.h"
#include "icons/eth.h"
#include "icons/arrow_up.h"
#include "icons/arrow_down.h"

void drawTinyText(int x, int y, char* text, int color) {
  display.setFont(&Cantarell_Regular_euro8pt8b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawSmallText(int x, int y, char* text, int color) {
  display.setFont(&Cantarell_Regular_euro10pt8b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawText(int x, int y, char* text, int color) {
  display.setFont(&Cantarell_Regular_euro12pt8b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawBoldText(int x, int y, char* text, int color) {
  display.setFont(&Cantarell_Bold_euro12pt8b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void displayTrend(int x, int y, char* label, double trend) {
  drawSmallText(x, y, label, GxEPD_BLACK);
  boolean up = trend > 0;
  display.drawBitmap(x + 15, y - 16, up ? arrow_up_bits : arrow_down_bits, 16, 17, up ? GxEPD_RED : GxEPD_BLACK);
}

void displayTrends(int x, int y, Price price) {
  char euroAmountStr[20];
  sprintf(euroAmountStr, "%.0f \x80", price.price);
  drawText(x + 6, y, euroAmountStr, GxEPD_BLACK);
  displayTrend(x + 125, y, "d", price.percent_change_24h);
  displayTrend(x + 165, y, "w", price.percent_change_7d);
  displayTrend(x + 205, y, "m", price.percent_change_30d);
}

void displayPrice(int x, int y, double currencyAmount, double euroPrice) {
  int baseX = x;
  int baseY = y;
  char currentAmountStr[10];
  char euroAmountStr[20];
  sprintf(currentAmountStr, "%.6f", currencyAmount);
  double euroAmount = currencyAmount * euroPrice;
  sprintf(euroAmountStr, euroAmount > 100 ? "%.0f \x80" : "%.2f \x80", euroAmount);

  drawSmallText(baseX, baseY, currentAmountStr, GxEPD_BLACK);
  drawBoldText(baseX, baseY + 25, euroAmountStr, GxEPD_BLACK);
}

void displayCurrenciesLogos(int yBtc, int yEth) {
  display.drawBitmap(6, yBtc, btc_bits, 32, 32, GxEPD_BLACK);
  display.drawBitmap(6, yEth, eth_bits, 32, 32, GxEPD_BLACK);
}

void displayLastUpdated(int x, int y, char* lastUpdated) {
  drawTinyText(x, y, lastUpdated, GxEPD_BLACK);
}
  
void displayData(Amounts* amounts, Prices* prices) {
  display.fillScreen(GxEPD_WHITE);
  display.firstPage();
  do {
    displayCurrenciesLogos(10, 90);

    displayPrice(50, 20, amounts->btc[0], prices->btc.price);
    displayPrice(160, 20, amounts->btc[1], prices->btc.price);
    displayTrends(10, 70, prices->btc);

    displayPrice(50, 100, amounts->eth[0], prices->eth.price);
    displayPrice(160, 100, amounts->eth[1], prices->eth.price);
    displayTrends(10, 150, prices->eth);

    displayLastUpdated(120, 172, prices->eth.last_updated);

  } while (display.nextPage());
}

void displayCenteredText(char* text) {
  display.setFont(&Cantarell_Bold_euro12pt8b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(text);
  }
  while (display.nextPage());
}
