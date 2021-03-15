#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

#include "icons/btc.h"
#include "icons/eth.h"
#include "icons/arrow_up.h"
#include "icons/arrow_down.h"

void drawTinyText(int x, int y, char* text, int color) {
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawSmallText(int x, int y, char* text, int color) {
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void displayTrend(int x, int y, char* label, double trend) {
  drawTinyText(x, y, label, GxEPD_BLACK);
  boolean up = trend > 0;
  display.drawBitmap(x + 15, y - 16, up ? arrow_up_bits : arrow_down_bits, 16, 17, up ? GxEPD_RED : GxEPD_BLACK);
}

void displayTrends(int x, int y, Price price) {
  char euroAmountStr[20];
  sprintf(euroAmountStr, "%.0f eur", price.price);
  drawTinyText(x + 6, y, euroAmountStr, GxEPD_BLACK);
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
  sprintf(euroAmountStr, euroAmount > 1000 ? "%.0f eur" : "%.2f eur", euroAmount);

  drawTinyText(baseX, baseY, currentAmountStr, GxEPD_BLACK);
  drawSmallText(baseX, baseY + 20, euroAmountStr, GxEPD_BLACK);
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

    displayCurrenciesLogos(20, 100);

    displayPrice(50, 25, amounts->btc[0], prices->btc.price);
    displayPrice(160, 25, amounts->btc[1], prices->btc.price);
    displayTrends(10, 70, prices->btc);

    displayPrice(50, 105, amounts->eth[0], prices->eth.price);
    displayPrice(160, 105, amounts->eth[1], prices->eth.price);
    displayTrends(10, 150, prices->eth);

    displayLastUpdated(85, 172, prices->eth.last_updated);

  } while (display.nextPage());
}

void displayCenteredText(char* text) {
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  // here we use paged drawing, even if the processor has enough RAM for full buffer
  // so this can be used with any supported processor board.
  // the cost in code overhead and execution time penalty is marginal
  // tell the graphics class to use paged drawing mode
  display.firstPage();
  do
  {
    // this part of code is executed multiple times, as many as needed,
    // in case of full buffer it is executed once
    // IMPORTANT: each iteration needs to draw the same, to avoid strange effects
    // use a copy of values that might change, don't read e.g. from analog or pins in the loop!
    display.fillScreen(GxEPD_WHITE); // set the background to white (fill the buffer with value for white)
    display.setCursor(x, y); // set the postition to start printing text
    display.print(text); // print some text
    // end of part executed multiple times
  }
  // tell the graphics class to transfer the buffer content (page) to the controller buffer
  // the graphics class will command the controller to refresh to the screen when the last page has been transferred
  // returns true if more pages need be drawn and transferred
  // returns false if the last page has been transferred and the screen refreshed for panels without fast partial update
  // returns false for panels with fast partial update when the controller buffer has been written once more, to make the differential buffers equal
  // (for full buffered with fast partial update the (full) buffer is just transferred again, and false returned)
  while (display.nextPage());
}
