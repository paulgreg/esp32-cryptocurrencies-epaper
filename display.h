#include "Fonts/Cantarell_Regular_euro8pt8b.h"
#include "Fonts/FreeMonoBold_euro12pt8b.h"
#include "Fonts/FreeMonoBold_euro14pt8b.h"

#define FONT_SMALL Cantarell_Regular_euro8pt8b
#define FONT_NORMAL FreeMonoBold_euro12pt8b
#define FONT_BIG FreeMonoBold_euro14pt8b

#include "icons/btc.h"
#include "icons/eth.h"
#include "icons/arrow_up.h"
#include "icons/arrow_down.h"

// Screen resolution is 264 × 176

#define EURO "\x80"
#define FORMAT_INT "%.0f"
#define FORMAT_FLOAT "%.7f"

void drawSmallText(int x, int y, char* text, int color) {
  display.setFont(&FONT_SMALL);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawSmallTextRightAlign(int x, int y, char* text, int color) {
  display.setFont(&FONT_SMALL);
  display.setTextColor(color);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setCursor(x - tbw, y);
  display.println(text);
}

void drawText(int x, int y, char* text, int color) {
  display.setFont(&FONT_NORMAL);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawTextRightAlign(int x, int y, char* text, int color) {
  display.setFont(&FONT_NORMAL);
  display.setTextColor(color);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setCursor(x - tbw, y);
  display.println(text);
}

void drawBoldText(int x, int y, char* text, int color) {
  display.setFont(&FONT_BIG);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawBoldTextRightAlign(int x, int y, char* text, int color) {
  display.setFont(&FONT_BIG);
  display.setTextColor(color);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  display.setCursor(x - tbw, y);
  display.println(text);
}

void displayTrend(int x, int y, char* label, double trend) {
  drawSmallTextRightAlign(x, y, label, GxEPD_BLACK);
  boolean up = trend > 0;
  display.drawBitmap(x + 5, y - 14, up ? arrow_up_bits : arrow_down_bits, 16, 17, up ? GxEPD_RED : GxEPD_BLACK);
}

void displayTrends(int x, int y, Price price) {
  displayTrend(x, y, "d", price.percent_change_24h);
  displayTrend(x + 40, y, "w", price.percent_change_7d);
  displayTrend(x + 80, y, "m", price.percent_change_30d);
}

void displayCurrencyPrice(int x, int y, double price) {
  char euroAmountStr[20];
  sprintf(euroAmountStr, FORMAT_INT, price);
  drawTextRightAlign(x, y, EURO, GxEPD_BLACK);
  drawTextRightAlign(x - 22, y, euroAmountStr, GxEPD_BLACK);
}

void displayPrice(int x, int y, double currencyAmount, double euroPrice) {
  char currentAmountStr[10];
  char euroAmountStr[20];
  sprintf(currentAmountStr, FORMAT_FLOAT, currencyAmount);
  double euroAmount = currencyAmount * euroPrice;
  sprintf(euroAmountStr, FORMAT_INT, euroAmount);

  drawSmallTextRightAlign(x, y, currentAmountStr, GxEPD_BLACK);
  drawBoldTextRightAlign(x, y + 28, EURO, GxEPD_RED);
  drawBoldTextRightAlign(x - 25, y + 28, euroAmountStr, GxEPD_RED);
}

void displayCurrenciesLogos() {
  display.drawBitmap(8, 12, btc_bits, 32, 32, GxEPD_BLACK);
  display.drawBitmap(8, 95, eth_bits, 32, 32, GxEPD_BLACK);
}

void displayLastUpdated(int x, int y, char* lastUpdated) {
  drawSmallTextRightAlign(x, y, lastUpdated, GxEPD_BLACK);
}
  
void displayData(Amounts* amounts, Prices* prices) {
  display.fillScreen(GxEPD_WHITE);
  display.firstPage();
  do {
    displayCurrenciesLogos();

    displayPrice(140, 20, amounts->btc[0], prices->btc.price);
    displayPrice(250, 20, amounts->btc[1], prices->btc.price);
    displayCurrencyPrice(126, 76, prices->btc.price);
    displayTrends(150, 72, prices->btc);

    displayPrice(140, 100, amounts->eth[0], prices->eth.price);
    displayPrice(250, 100, amounts->eth[1], prices->eth.price);
    displayCurrencyPrice(126, 156, prices->eth.price);
    displayTrends(150, 152, prices->eth);

    displayLastUpdated(260, 172, prices->eth.last_updated);

  } while (display.nextPage());
}

void displayCenteredText(char* text) {
  display.setFont(&FONT_NORMAL);
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
