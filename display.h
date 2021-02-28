void drawText(int x, int y, char* text, int color) {
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

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

void displayNickNames (int y, char* nickname1, int x1, char* nickname2, int x2) {
  drawText(x1, y, nickname1, GxEPD_RED);
  drawText(x2, y, nickname2, GxEPD_RED);
}

void displayCryptocurrencies(int x, char* cryptocurrency1, int y1, char* cryptocurrency2, int y2) {
  drawSmallText(x, y1, cryptocurrency1, GxEPD_RED);
  drawSmallText(x, y2, cryptocurrency2, GxEPD_RED);
}

void displayPrice(int x, int y, double currencyAmount, double euroPrice) {
  int baseX = x;
  int baseY = y;
  char currentAmountStr[10];
  sprintf(currentAmountStr, "%.6f", currencyAmount);
  Serial.printf("currentAmountStr: %s\n", currentAmountStr);
  char euroAmountStr[20];
  sprintf(euroAmountStr, "%.0f eur", currencyAmount * euroPrice);
  Serial.printf("euroAmountStr: %s\n", euroAmountStr);

  drawSmallText(baseX, baseY, currentAmountStr, GxEPD_BLACK);
  drawSmallText(baseX, baseY + 20, euroAmountStr, GxEPD_BLACK);
}

void displayData(Amounts* amounts, Prices* prices) {
  display.fillScreen(GxEPD_WHITE);
  display.firstPage();
  do {
    displayNickNames(20, USER_1_NICKNAME, 60, USER_2_NICKNAME, 180);
    displayCryptocurrencies(4, "BTC", 60, "ETH", 120);
    displayPrice(50, 50, amounts->btc[0], prices->btc.price);
    displayPrice(160, 50, amounts->btc[1], prices->btc.price);
    displayPrice(50, 120, amounts->eth[0], prices->eth.price);
    displayPrice(160, 120, amounts->eth[1], prices->eth.price);
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
