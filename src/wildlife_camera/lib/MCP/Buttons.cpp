#include "Buttons.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <pins.h>

Adafruit_MCP23X17 mcp;

bool setup_mcp()
{
    if (!mcp.begin_I2C())
    {
        Serial.println("Could not find MCP23017");
        return false;
    }
    Serial.println("MCP23017 connected!");
    return true;
}
bool* debounce = new bool[4];
bool setup_button()
{
    debounce[0] = 0;
    debounce[1] = 0;
    debounce[2] = 0;
    debounce[3] = 0;

    mcp.pinMode(MCP_BUTTON_PIN_1, INPUT_PULLUP);
    mcp.pinMode(MCP_BUTTON_PIN_2, INPUT_PULLUP);
    mcp.pinMode(MCP_BUTTON_PIN_3, INPUT_PULLUP);
    mcp.pinMode(MCP_BUTTON_PIN_4, INPUT_PULLUP);
}

bool check_pressed(int pin, int button)
{
    if (mcp.digitalRead(pin) == LOW)
    {
        if (debounce[button] == 0)
        {
            debounce[button] = 1;
            return true;
        }
    }
    else
    {
        debounce[button] = 0;
    }
    return false;
}

bool is_button_pressed(int button) {
    switch (button)
    {
    case B_LEFT:
        return check_pressed(MCP_BUTTON_PIN_1, B_LEFT);
        break;
    case B_RIGHT:
        return check_pressed(MCP_BUTTON_PIN_2, B_RIGHT);
        break;
    case B_UP:
        return check_pressed(MCP_BUTTON_PIN_3, B_UP);
        break;
    case B_DOWN:
        return check_pressed(MCP_BUTTON_PIN_4, B_DOWN);
        break;
    }
    return false;
}