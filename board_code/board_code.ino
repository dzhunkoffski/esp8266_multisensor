#include<ESP8266WiFi.h>
#include<WiFiClientSecure.h>
#include<UniversalTelegramBot.h>

#include "DHT.h"

#define DHTTYPE DHT11
const int pir_pin = 13; // D7
const int dht_pin = 14; // D5
DHT dht(dht_pin, DHTTYPE);

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#define BOT_TOKEN "here past your bot token"
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
const unsigned long BOT_MTBS = 1000;
unsigned long bot_lasttime;

String get_temp() {
  delay(2000);
  
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  if (isnan(humid) || isnan(temp)) {
    return "Failed to read from DHT sensor!!!";
  }
  
  return "Temperature: " + String(temp) + " ºC\n";
}

String get_humid() {
  delay(2000);

  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  if (isnan(humid) || isnan(temp)) {
    return "Failed to read from DHT sensor!!!";
  }

  return "Humidity: " + String(humid) + " %\n";
}

void process_new_messages(int num_of_new) {
  Serial.println("Get new messages:");
  Serial.println(num_of_new);

  for (int i = 0; i < num_of_new; ++i) {
    String chat_id = String(bot.messages[i].chat_id);
    
    //////// If you want bot only speaks with you ////////
    if ("474335162" != chat_id) {
      bot.sendMessage(chat_id, "Unauthorized user!", "");
      continue;
    }
    //////////////////////////////////////////////////////

    String command = bot.messages[i].text;
    Serial.println(command);
    String from_name = bot.messages[i].from_name;

    if (command == "/start") {
      String answer = "Welcome " + from_name + "!\n";
      answer += "This is multisensor which can read temperature, humidity and movement.\n\n";
      answer += "/temperature : to get the temperature in celsium\n";
      answer += "/humidity : to get the humidity\n";

      bot.sendMessage(chat_id, answer, "Markdown");
    }

    if (command == "/temperature") {
      bot.sendMessage(chat_id, get_temp(), "");
    }

    if (command == "/humidity") {
      bot.sendMessage(chat_id, get_humid(), "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  pinMode(pir_pin, INPUT);
  delay(2000);

  configTime(0, 0, "pool.ntp.org");
  secured_client.setTrustAnchors(&cert);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.println(" successfully!");
  delay(10000);
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int num_of_new = bot.getUpdates(bot.last_message_received + 1);
    while (num_of_new) {
      Serial.println("got response");
      process_new_messages(num_of_new);
      num_of_new = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }

  // process pir
  long state = digitalRead(pir_pin);
  if (state == HIGH) {
    Serial.println("Motion detected!");
    bot.sendMessage("474335162", "ALERT!!! Motion detected!!!", "");
    delay(1000);
  } else {
    Serial.println("Motion absent!!!");
    delay(1000);
  }
}
