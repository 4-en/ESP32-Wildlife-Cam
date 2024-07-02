#include <Arduino.h>
#include <Camera.h>
#include <Menu.h>
#include <Wire.h>
#include <pins.h>
#include <Adafruit_GFX.h>

void scan_i2c()
{
    byte count = 0;
    for (byte i = 1; i < 120; i++)
    {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0)
        {
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.println(")");
            count++;
            delay(1);
        }
    }
    Serial.print("Found ");
    Serial.print(count, DEC);
    Serial.println(" device(s).");
}

void setup()
{
    Serial.begin(115200);
    Wire.setPins(SDA_PIN, SCL_PIN);
    Wire.begin();

    // print all I2C devices and their addresses
    scan_i2c();


    // setup modules
    // stop if any of the setup functions fail
    if (!setup_camera())
    {
        Serial.println("Camera setup failed");
        while (1)
        {
        }
    }

    if (!setup_mmc())
    {
        Serial.println("SD_MMC setup failed");
        while (1)
        {
        }
    }


}

void loop()
{
}
