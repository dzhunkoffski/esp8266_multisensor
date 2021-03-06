# esp8266_multisensor
Esp8266 multisensor based on NodeMCU V3 which reads temperature throught DHT11 and movements throught pir sensor HC-SR501 and sends data via Telegram Bot for user (I'm using [UniversalTelegramBot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot) library). Download cover box for 3D printing [here](https://github.com/dzhunkoffski/esp8266_multisensor/tree/main/3d_models).
![3d box](https://github.com/dzhunkoffski/esp8266_multisensor/blob/main/board_code/Multisensor.png)


## Commands and messages:
Command/message | Definition
------------ | -------------
/start | start conevrsation with MultisensorBot 
/temperature | multisensor reads temperature from the DHT11 and sends it
/humidity | multisensor reads humidity from the DHT11 and sends it
ALERT!!! Motion detected!!! | HC-SR501's digital output sends a signal to the board, equal infrared radiation in the sensor's field of view
Failed to read from DHT sensor!!! | readTemperature() or readHumidity() returns NaN value

## Example:
![mockup](https://github.com/dzhunkoffski/esp8266_multisensor/blob/main/board_code/mckp.png)
