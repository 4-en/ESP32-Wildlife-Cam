#ifndef BUTTONS_H

#define BUTTONS_H
#define B_LEFT 0
#define B_RIGHT 1
#define B_UP 2
#define B_DOWN 3

#endif

bool setup_mcp();
bool setup_buttons();

bool is_button_pressed(int button);

bool detect_pir();

void set_mcp_led(int led, bool state);