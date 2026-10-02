#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED_PIN 2
#define BUTTON_PIN 4

#define DHT_PIN 15
#define DHT_TYPE DHT22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

DHT dht(DHT_PIN, DHT_TYPE);

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(115200);

    dht.begin();

    Wire.begin(21, 22);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("OLED initialization failed!");

        while (true)
        {
            delay(1000);
        }
    }

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("ENVIRONMENT MONITOR");

    display.setCursor(0, 20);
    display.println("System Starting...");

    display.display();

    delay(2000);
}

void loop()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    int buttonState = digitalRead(BUTTON_PIN);

    // LED control
    if (buttonState == LOW)
    {
        digitalWrite(LED_PIN, HIGH);
    }
    else
    {
        digitalWrite(LED_PIN, LOW);
    }

    // OLED
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("ENVIRONMENT MONITOR");

    // Check sensor
    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("DHT22 reading failed!");

        display.setCursor(0, 22);
        display.println("Sensor Error");

        display.setCursor(0, 38);
        display.println("Check DHT22");

        display.setCursor(0, 52);
        display.println("Retrying...");

        display.display();

        delay(2000);
        return;
    }

    // Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // OLED temperature
    display.setCursor(0, 20);
    display.print("Temperature: ");
    display.print(temperature);
    display.println(" C");

    // OLED humidity
    display.setCursor(0, 35);
    display.print("Humidity: ");
    display.print(humidity);
    display.println(" %");

    // OLED button status
    display.setCursor(0, 52);

    if (buttonState == LOW)
    {
        display.println("Button: PRESSED");
    }
    else
    {
        display.println("Button: READY");
    }

    display.display();

    delay(2000);
}