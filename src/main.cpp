#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <PWM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <U8glib.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BUTTON_ONE 3
#define BUTTON_TWO 4

#define PWM_PIN 9
// #define TACHO_PIN 2

int outputValue;
int fanSpeed = 0; // 0-100%

// 'Frame1', 16x16px
const uint8_t FRAME1[] PROGMEM = {0x00, 0x00, 0x03, 0x80, 0x0f, 0xc0, 0x0f, 0xc0, 0x0f, 0xc0, 0x1f, 0xc0, 0x0f, 0xf8, 0x0f, 0xfc,
                                  0x0e, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xbe, 0x3f, 0x9c, 0x1e, 0x00, 0x00, 0x00};
// 'Frame2', 16x16px
const uint8_t FRAME2[] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x03, 0xf8, 0x03, 0xfc, 0x77, 0xfc, 0xff, 0xf8, 0xff, 0xe0, 
                                  0xfe, 0xf0, 0xff, 0xf0, 0x7f, 0xf8, 0x3f, 0xf8, 0x01, 0xf8, 0x01, 0xf8, 0x01, 0xf0, 0x01, 0xe0};

bool tiktok = false;
bool fanOn = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize display.
// U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);

DHT dht(DHTPIN, DHTTYPE);

long getSpeedFromTmp(float temp)
{
    if (temp < 20)
    {
        return 0;
    }
    else if (temp > 50)
    {
        return 100;
    }
    else
    {
        return map(temp, 20, 50, 20, 100);
    }
}

void drawFan(int16_t x, int16_t y, uint16_t color)
{
    if (!fanOn)
    {
        display.drawBitmap(x, y, FRAME1, 16, 16, color);
        return;
    }
    if (tiktok)
        display.drawBitmap(x, y, FRAME1, 16, 16, color);
    else
        display.drawBitmap(x, y, FRAME2, 16, 16, color);
    tiktok = !tiktok;
}

void setup()
{
    Serial.begin(115200);

    InitTimersSafe();                                   // not sure what this is for, but I think i need it for PWM control?
    bool success = SetPinFrequencySafe(PWM_PIN, 25000); // set frequency to 25kHz
    if (!success)
    {
        Serial.println("ERROR: Failed to set frequency");
    }
    pwmWrite(PWM_PIN, map(fanSpeed, 0, 100, 0, 255)); // 51=20% duty cycle, 255=100% duty cycle

    pinMode(BUTTON_ONE, INPUT);
    pinMode(BUTTON_TWO, INPUT);

    dht.begin();

    // Set font.
    // u8g.setFont(u8g_font_unifont);
    // u8g.setColorIndex(1);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setRotation(2);
}

void loop()
{
    int buttonOnePressed = digitalRead(BUTTON_ONE);
    int buttonTwoPressed = digitalRead(BUTTON_TWO);

    Serial.println(buttonOnePressed);
    Serial.println(buttonTwoPressed);

    float temp = dht.readTemperature();
    fanSpeed = getSpeedFromTmp(temp);
    if (fanSpeed > 0)
    {
        fanOn = true;
    }
    else
    {
        fanOn = false;
    }
    pwmWrite(PWM_PIN, map(fanSpeed, 0, 100, 0, 255));

    // u8g.firstPage();
    // do
    // {
    //     draw();
    // } while (u8g.nextPage());

    display.clearDisplay();
    display.setCursor(0, 0);

    display.print("FAN SPEED: ");
    display.print(fanSpeed);
    display.println(" %");

    display.print("TEMP: ");
    display.print(String(temp));
    display.println(" C");

    display.setCursor(0, 50);
    display.print("DEACTIVATE");

    display.setCursor(80, 50);
    display.print("ACTIVATE");

    drawFan(112, 0, WHITE);

    display.display();

    delay(100);
}
