// use Board "ESP32 Dev Module" to build with Arduino IDE
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>

GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));

// Screen used
#include "bitmaps/Bitmaps3c176x264.h" // 2.7"  b/w/r

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "parameters.h"
#include "data.h"
#include "display.h"
#include "network.h"

const uint64_t SECOND = 1000;
const uint64_t MINUTE = 60 * SECOND;
const uint64_t HOUR = 60 * MINUTE;
const uint64_t DAY = 24 * HOUR;
const uint64_t MICRO_SEC_TO_MILLI_SEC_FACTOR = 1000;

void setup() {
  Serial.begin(115200);
  display.init(115200);
  // *** special handling for Waveshare ESP32 Driver board *** //
  SPI.end(); // release standard SPI pins, e.g. SCK(18), MISO(19), MOSI(23), SS(5)
  //SPI: void begin(int8_t sck=-1, int8_t miso=-1, int8_t mosi=-1, int8_t ss=-1);
  SPI.begin(13, 12, 14, 15); // map and init SPI pins SCK(13), MISO(12), MOSI(14), SS(15)
  // *** end of special handling for Waveshare ESP32 Driver board *** //
  print_wakeup_reason();  
  display.setRotation(1);
}

JSONVar jsonStatus;
JSONVar jsonPrices;
Amounts amounts;
Prices prices;

void loop() {
  uint64_t sleepTime = HOUR;
  
  if (!connectToWifi()) {
    displayCenteredText("Can’t connect to wifi");
  } else {
    String statusStr = getBasicAuthJSON(URL_CRYPTOCURRENCIES_STATUS, LOGIN, PASSWORD);
    jsonStatus = JSON.parse(statusStr);
    Serial.print("jsonStatus: "); Serial.println(jsonStatus);

    String pricesStr = getApiKeyJSON(URL_COINMARKETCAP_QUOTES, COINMARKETCAP_API_HEADER, COINMARKETCAP_API_KEY);
    jsonPrices = JSON.parse(pricesStr);
    Serial.print("jsonPrices: "); Serial.println(jsonPrices);
    
    if (JSON.typeof(jsonStatus) == "undefined") {
      displayCenteredText("Error fetching status JSON");
    } else if (JSON.typeof(jsonPrices) == "undefined") {
      displayCenteredText("Error fetching prices JSON");
    } else {
      if (!fillAmountsFromJson(jsonStatus, &amounts)) {
        displayCenteredText("Error parsing status JSON");
      } else if (!fillPricesFromJson(jsonPrices, &prices)) {
        displayCenteredText("Error parsing prices JSON");
      } else {
        displayData(&amounts, &prices);
        if (prices.eth.last_updated[11] == '0' && prices.eth.last_updated[12] == '0') sleepTime = HOUR * 6; // sleep for the night
      }
    }
    disconnectFromWifi();
  }

  sleep(sleepTime);

  Serial.println("After sleep, that line should never be printed");
  delay(HOUR);
}

void sleep(uint64_t sleepTime) {
  Serial.flush();
  display.powerOff();
  esp_sleep_enable_timer_wakeup((uint64_t) sleepTime * MICRO_SEC_TO_MILLI_SEC_FACTOR);
  esp_deep_sleep_start();
  delay(MINUTE);
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : {
      Serial.print("Wakeup caused by touchpad pin "); 
      Serial.println(esp_sleep_get_touchpad_wakeup_status());
      break;
    }
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}
