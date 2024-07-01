#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <Wire.h>

/* different I2C pins, since default pins are used by camera */
#define SCL_PIN 13
#define SDA_PIN 12

#define MCP_BUTTON_PIN_1 0
#define MCP_BUTTON_PIN_2 1
#define MCP_BUTTON_PIN_3 2
#define MCP_BUTTON_PIN_4 3
#define MCP_BUTTON_PIN_5 4

Adafruit_MCP23X17 mcp;
auto debounce = new bool[4];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if(!mcp.begin_I2C()) {
    Serial.println("Could not find MCP23017");
    while(1);
  }
  Serial.println("MCP23017 connected!");

  debounce[0] = 0;
  debounce[1] = 0;
  debounce[2] = 0;
  debounce[3] = 0;


  mcp.pinMode(MCP_BUTTON_PIN_1, INPUT_PULLUP);
  mcp.pinMode(MCP_BUTTON_PIN_2, INPUT_PULLUP);
  mcp.pinMode(MCP_BUTTON_PIN_3, INPUT_PULLUP);
  mcp.pinMode(MCP_BUTTON_PIN_4, INPUT_PULLUP);

}


void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Hello World");

  if(mcp.digitalRead(MCP_BUTTON_PIN_1) == LOW) {
    if(debounce[0] == 0) {
      debounce[0] = 1;
      Serial.println("Button 1 pressed");
    }
  } else {
    debounce[0] = 0;
  }

  if(mcp.digitalRead(MCP_BUTTON_PIN_2) == LOW) {
    if(debounce[1] == 0) {
      debounce[1] = 1;
      Serial.println("Button 2 pressed");
    }
  } else {
    debounce[1] = 0;
  }

  if(mcp.digitalRead(MCP_BUTTON_PIN_3) == LOW) {
    if(debounce[2] == 0) {
      debounce[2] = 1;
      Serial.println("Button 3 pressed");
    }
  } else {
    debounce[2] = 0;
  }

  if(mcp.digitalRead(MCP_BUTTON_PIN_4) == LOW) {
    if(debounce[3] == 0) {
      debounce[3] = 1;
      Serial.println("Button 4 pressed");
    }
  } else {
    debounce[3] = 0;
  }
}

