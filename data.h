struct Amounts {
  double btc[2];
  double eth[2];
};

boolean fillAmountsFromJson(JSONVar json, Amounts* amounts) {
  if (!json.hasOwnProperty("btc")) {
    Serial.println("fillAmountsFromJson: btc not found");
    return false;
  }
  if (!json.hasOwnProperty("eth")) {
    Serial.println("fillAmountsFromJson: eth not found");
    return false;
  }

  amounts->btc[0] = (double) json["btc"]["user1"];
  amounts->btc[1] = (double) json["btc"]["user2"];

  amounts->eth[0] = (double) json["eth"]["user1"];
  amounts->eth[1] = (double) json["eth"]["user2"];

  return true;
}
/* status.json
{
    "btc": {
        "user1": 0.01,
        "user2": 0.02
    },
    "eth": {
        "user1": 0.01,
        "user2": 0.02
    }
} */

struct Price {
  double price;
  double percent_change_30d;
  double percent_change_7d;
  double percent_change_24h;
  char last_updated[25];
};

struct Prices {
  Price btc;
  Price eth;
};

boolean fillPricesFromJson(JSONVar json, Prices* prices) {
  if (!json.hasOwnProperty("data")) {
    Serial.println("fillPricesFromJson: data not found");
    return false;
  }
  if (!json["data"].hasOwnProperty("BTC")) {
    Serial.println("fillPricesFromJson: BTC not found");
    return false;
  }
  if (!json["data"].hasOwnProperty("ETH")) {
    Serial.println("fillPricesFromJson: ETH not found");
    return false;
  }

  prices->btc.price = (double) json["data"]["BTC"]["quote"]["EUR"]["price"];
  prices->btc.percent_change_30d = (double) json["data"]["BTC"]["quote"]["EUR"]["percent_change_30d"];
  prices->btc.percent_change_7d = (double) json["data"]["BTC"]["quote"]["EUR"]["percent_change_7d"];
  prices->btc.percent_change_24h = (double) json["data"]["BTC"]["quote"]["EUR"]["percent_change_24h"];
  sprintf(prices->btc.last_updated, "%s", (const char*) json["data"]["BTC"]["quote"]["EUR"]["last_updated"]);
  prices->btc.last_updated[16] = 0;

  prices->eth.price = (double) json["data"]["ETH"]["quote"]["EUR"]["price"];
  prices->eth.percent_change_30d = (double) json["data"]["ETH"]["quote"]["EUR"]["percent_change_30d"];
  prices->eth.percent_change_7d = (double) json["data"]["ETH"]["quote"]["EUR"]["percent_change_7d"];
  prices->eth.percent_change_24h = (double) json["data"]["ETH"]["quote"]["EUR"]["percent_change_24h"];
  sprintf(prices->eth.last_updated, "%s", (const char*) json["data"]["ETH"]["quote"]["EUR"]["last_updated"]);
  prices->eth.last_updated[16] = 0;

  return true;
}
/* URL : https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest?symbol=BTC,ETH&convert=EUR
 * Doc : https://coinmarketcap.com/api/documentation/v1/#operation/getV1CryptocurrencyQuotesLatest
{
  "data": {
    "ETH": {
      "quote": {
        "EUR": {
          "last_updated": "2021-02-27T20:17:03.000Z",
          "market_cap": 143353931030.55582,
          "percent_change_30d": 12.85010394,
          "percent_change_7d": -25.51044525,
          "percent_change_24h": -0.8234246,
          "percent_change_1h": 0.99798796,
          "volume_24h": 17712698891.264263,
          "price": 1248.2108997477699
        }
      },
      "last_updated": "2021-02-27T20:16:07.000Z",
      "total_supply": 114847523.8115,
      "circulating_supply": 114847523.8115,
      "id": 1027,
      "name": "Ethereum",
      "symbol": "ETH",
      "slug": "ethereum",
      ...
    },
  "BTC": { ... }
} */
