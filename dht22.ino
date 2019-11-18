#include <DHT.h>
#include <Arduino.h> 
#include <U8g2lib.h> 
#include <Wire.h>
#include <math.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

#define DHTPINNUM 7
#define DHTTYPE DHT22
DHT dht(DHTPINNUM, DHTTYPE);

const int led_max_brightness = 40; // maximum led brightness 0-255
int   brightness = 0;    // how bright the LED is
bool  isLedOn = false;
float humidity;
float temperature;
char  buffer[5];
 
void setup()
{
   Serial.begin(9600);
   
   // DHT 22 - sensor init
   dht.begin();
   
   // OFF the LED Pin 13
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, LOW);
   
   // OLED monitor
   u8g2.begin();
}

void loop()
{

    temperature = dht.readTemperature();
    humidity    = dht.readHumidity();

    // format temperature string
    char tempString[7];
    dtostrf(temperature, -4, 1, buffer);
    snprintf(tempString, 7, "%s C", buffer);
    Serial.println(tempString);

    // format humidity string
    char humidString[7];
    dtostrf(humidity, -4, 1, buffer);
    snprintf(humidString, 7, "%s %%", buffer);
    Serial.println(humidString);

    // draw on OLED display
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_tenfatguys_tu);
    u8g2.setCursor(0, 24);
    u8g2.print(tempString);
    u8g2.setCursor(0,52);
    u8g2.print(humidString);
    u8g2.sendBuffer();

    // LED
    if(temperature > 30){
      brightness = 0;
      while(isLedOn == false){
        analogWrite(LED_BUILTIN, brightness);
        brightness = brightness + 2;
        if(brightness >= led_max_brightness){
          isLedOn = true;
        }
        delay(30);
      }
    }
    else if(isLedOn == true){
      brightness = led_max_brightness;
      while(true){
        analogWrite(LED_BUILTIN, brightness);
        brightness = brightness - 2;
        if(brightness < 0){
          isLedOn = false;
          break;
        }
        delay(30);
      }
    }

    delay(2000); // Due to DHT22 is 0.5Hz, run this every 2 seconds.
}

