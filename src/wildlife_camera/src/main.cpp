#include <Arduino.h>
#include <Camera.h>
#include <Menu.h>
#include <Wire.h>
#include <pins.h>
#include <Adafruit_GFX.h>
#include <Buttons.h>

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

    if(!setup_mcp())
    {
        Serial.println("MCP23017 setup failed");
        while(1)
        {
        }
    }

    if(!setup_buttons())
    {
        Serial.println("Button setup failed");
        while(1)
        {
        }
    }

    // write_text("/test.txt", "Hello World!\n");


}

int pir_count = 0;
void loop()
{
    if(detect_pir())
    {
        Serial.printf("PIR detected %d times\n", pir_count++);

        auto fb = take_picture();

        String pathString = "/image" + String(pir_count) + ".jpg";
        const char* path = pathString.c_str();
        save_picture(fb, path);

        // free up memory
        release_picture(fb);


        set_mcp_led(MCP_LED_PIN_1, HIGH);
        delay(10000);
        set_mcp_led(MCP_LED_PIN_1, LOW);
    }
}
