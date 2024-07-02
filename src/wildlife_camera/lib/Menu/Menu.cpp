#include "Menu.h"
#include <Buttons.h>
#include <pins.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Camera.h>
#include <Arduino.h>
#include <DS3231.h>

DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int menu_state = MAIN_MENU;

int main_menu_items[] = {MM_DETECT_MOTION, MM_CALIBRATE_PIR, MM_SET_RTC, MM_MIN_DELAY, MM_SENSITIVITY, MM_BURST};
int main_menu_item_count = 6;
int main_menu_selected = 0;

int rtc_settings[] = {RTC_SETTING_YEAR, RTC_SETTING_MONTH, RTC_SETTING_DAY, RTC_SETTING_HOUR, RTC_SETTING_MINUTE, RTC_SETTING_SECOND};
int rtc_setting_selected = 0;

int rtc_setting_values[] = {0, 0, 0, 0, 0, 0};

bool century = false;
bool h12Flag;
bool pmFlag;

int min_delay = 1;
int sensitivity = 5;
int burst = 1;

void init_rtc() {
    rtc.setClockMode(false);
    if(rtc.getYear() < 24) {
        rtc.setYear(24);
        rtc.setMonth(1);
        rtc.setDate(1);
        rtc.setHour(0);
        rtc.setMinute(0);
        rtc.setSecond(0);
    }

}

bool init_display() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }

    init_rtc();

    display.display();

    return true;
}

String getTimestamp() {
    byte second, minute, hour, day, month, year;
    second = rtc.getSecond();
    minute = rtc.getMinute();
    hour = rtc.getHour(h12Flag, pmFlag);
    day = rtc.getDate();
    month = rtc.getMonth(century);
    year = rtc.getYear();

    String timestamp = String("20") + String(year) + "-" + String(month) + "-" + String(day) + "-" + String(hour) + ":" + String(minute) + ":" + String(second);
    return timestamp;
}

void draw_main_menu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(" [ Main Menu ]");

    for(int i = main_menu_selected; i < main_menu_selected + main_menu_item_count; i++) {
        int j = (i -1) % main_menu_item_count;
        if(j == main_menu_selected) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print(">");
        } else {
            display.setTextColor(SSD1306_WHITE);
        }

        switch(main_menu_items[j]) {
            case MM_DETECT_MOTION:
                display.println("Detect Motion");
                break;
            case MM_CALIBRATE_PIR:
                display.println("Calibrate PIR");
                break;
            case MM_SET_RTC:
                display.println("Set RTC");
                break;
            case MM_MIN_DELAY:
                display.print("Min Delay: ");
                display.println(min_delay);
                break;
            case MM_SENSITIVITY:
                display.print("Sensitivity: ");
                display.println(sensitivity);
                break;
            case MM_BURST:
                display.print("Burst: ");
                display.println(burst);
                break;
        }
    }

    display.display();

}

void load_rtc_to_values() {
    rtc_setting_values[RTC_SETTING_YEAR] = rtc.getYear();
    rtc_setting_values[RTC_SETTING_MONTH] = rtc.getMonth(century);
    rtc_setting_values[RTC_SETTING_DAY] = rtc.getDate();
    rtc_setting_values[RTC_SETTING_HOUR] = rtc.getHour(h12Flag, pmFlag);
    rtc_setting_values[RTC_SETTING_MINUTE] = rtc.getMinute();
    rtc_setting_values[RTC_SETTING_SECOND] = rtc.getSecond();
}

void handle_main_menu_input(int input) {
    switch(input) {
        case B_UP:
            main_menu_selected--;
            if(main_menu_selected < 0) {
                main_menu_selected = main_menu_item_count - 1;
            }
            break;
        case B_DOWN:
            main_menu_selected++;
            if(main_menu_selected >= main_menu_item_count) {
                main_menu_selected = 0;
            }
            break;
        case B_LEFT:
            switch(main_menu_items[main_menu_selected]) {
                case MM_DETECT_MOTION:
                    break;
                case MM_CALIBRATE_PIR:
                    break;
                case MM_SET_RTC:
                    break;
                case MM_MIN_DELAY:
                    min_delay--;
                    if(min_delay < 0) {
                        min_delay = 0;
                    }
                    break;
                case MM_SENSITIVITY:
                    sensitivity--;
                    if(sensitivity < 0) {
                        sensitivity = 0;
                    }
                    set_pir_sensitivity(sensitivity);
                    break;
                case MM_BURST:
                    burst--;
                    if(burst < 0) {
                        burst = 0;
                    }
                    break;
            }
            break;

        case B_RIGHT:
            switch(main_menu_items[main_menu_selected]) {
                case MM_DETECT_MOTION:
                    set_menu(DETECT_MOTION);
                    break;
                case MM_CALIBRATE_PIR:
                    set_menu(CALIBRATE_PIR);
                    break;
                case MM_SET_RTC:
                    load_rtc_to_values();
                    set_menu(SET_RTC);
                    break;
                case MM_MIN_DELAY:
                    min_delay++;
                    if(min_delay > 10) {
                        min_delay = 10;
                    }
                    break;
                case MM_SENSITIVITY:
                    sensitivity++;
                    if(sensitivity > 10) {
                        sensitivity = 10;
                    }
                    set_pir_sensitivity(sensitivity);
                    break;
                case MM_BURST:
                    burst++;
                    if(burst > 10) {
                        burst = 10;
                    }
                    break;
            }
            break;
    }
}

void draw_rtc_menu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Set RTC");
    
    switch(rtc_setting_selected) {
        case RTC_SETTING_YEAR:
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print(">");
            display.setTextColor(SSD1306_WHITE);
            display.print("Year: ");
            display.println(rtc_setting_values[RTC_SETTING_YEAR]);
            break;
        case RTC_SETTING_MONTH:
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print(">");
            display.setTextColor(SSD1306_WHITE);
            display.print("Month: ");
            display.println(rtc_setting_values[RTC_SETTING_MONTH]);
            break;
        case RTC_SETTING_DAY:
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print(">");
            display.setTextColor(SSD1306_WHITE);
            display.print("Day: ");
            display.println(rtc_setting_values[RTC_SETTING_DAY]);
            break;
        case RTC_SETTING_HOUR:
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print(">");
            display.setTextColor(SSD1306_WHITE);
            display.print("Hour: ");
            display.println(rtc_setting_values[RTC_SETTING_HOUR]);
            break;
        case RTC_SETTING_MINUTE:
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print(">");
            display.setTextColor(SSD1306_WHITE);
            display.print("Minute: ");
            display.println(rtc_setting_values[RTC_SETTING_MINUTE]);
            break;
        case RTC_SETTING_SECOND:
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print(">");
            display.setTextColor(SSD1306_WHITE);
            display.print("Second: ");
            display.println(rtc_setting_values[RTC_SETTING_SECOND]);
            break;
    }

    display.display();
}

// redraw the menu
void draw_menu() {

    switch(menu_state) {
        case MAIN_MENU:
            draw_main_menu();
            break;
        case CALIBRATE_PIR:
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.println("IR Calibration Mode");
            display.println("Press any button to");
            display.println("exit");
            display.display();
            break;
        case SET_RTC:
            draw_rtc_menu();
            break;
        case DETECT_MOTION:
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.println("Detecting Motion");
            display.println("Press any button to");
            display.println("exit");
            display.display();
            break;
    }

}

void set_menu(int menu) {
    menu_state = menu;
    draw_menu();

}

void set_rtc_from_values() {
    rtc.setYear(rtc_setting_values[RTC_SETTING_YEAR]);
    rtc.setMonth(rtc_setting_values[RTC_SETTING_MONTH]);
    rtc.setDate(rtc_setting_values[RTC_SETTING_DAY]);
    rtc.setHour(rtc_setting_values[RTC_SETTING_HOUR]);
    rtc.setMinute(rtc_setting_values[RTC_SETTING_MINUTE]);
    rtc.setSecond(rtc_setting_values[RTC_SETTING_SECOND]);

    rtc_setting_selected = 0;
}

void handle_rtc_button_press(int button) {
    switch(button) {
        case B_LEFT:
            if(rtc_setting_selected > 0) {
                rtc_setting_selected--;
            } else {
                set_menu(MAIN_MENU);
            }
            draw_menu();
            return;
        case B_RIGHT:
            if(rtc_setting_selected < 5) {
                rtc_setting_selected++;
            } else {
                set_rtc_from_values();
                set_menu(MAIN_MENU);
            }
            draw_menu();
            return;
        case B_UP:
            switch(rtc_setting_selected) {
                case RTC_SETTING_YEAR:
                    rtc_setting_values[RTC_SETTING_YEAR]++;
                    if(rtc_setting_values[RTC_SETTING_YEAR] > 99) {
                        rtc_setting_values[RTC_SETTING_YEAR] = 0;
                    }
                    break;
                case RTC_SETTING_MONTH:
                    rtc_setting_values[RTC_SETTING_MONTH]++;
                    if(rtc_setting_values[RTC_SETTING_MONTH] > 12) {
                        rtc_setting_values[RTC_SETTING_MONTH] = 1;
                    }
                    break;
                case RTC_SETTING_DAY:
                    rtc_setting_values[RTC_SETTING_DAY]++;
                    if(rtc_setting_values[RTC_SETTING_DAY] > 31) {
                        rtc_setting_values[RTC_SETTING_DAY] = 1;
                    }
                    break;
                case RTC_SETTING_HOUR:
                    rtc_setting_values[RTC_SETTING_HOUR]++;
                    if(rtc_setting_values[RTC_SETTING_HOUR] > 23) {
                        rtc_setting_values[RTC_SETTING_HOUR] = 0;
                    }
                    break;
                case RTC_SETTING_MINUTE:
                    rtc_setting_values[RTC_SETTING_MINUTE]++;
                    if(rtc_setting_values[RTC_SETTING_MINUTE] > 59) {
                        rtc_setting_values[RTC_SETTING_MINUTE] = 0;
                    }
                    break;
                case RTC_SETTING_SECOND:
                    rtc_setting_values[RTC_SETTING_SECOND]++;
                    if(rtc_setting_values[RTC_SETTING_SECOND] > 59) {
                        rtc_setting_values[RTC_SETTING_SECOND] = 0;
                    }
                    break;
            }
            draw_menu();
            return;
        case B_DOWN:
            switch(rtc_setting_selected) {
                case RTC_SETTING_YEAR:
                    rtc_setting_values[RTC_SETTING_YEAR]--;
                    if(rtc_setting_values[RTC_SETTING_YEAR] < 0) {
                        rtc_setting_values[RTC_SETTING_YEAR] = 99;
                    }
                    break;
                case RTC_SETTING_MONTH:
                    rtc_setting_values[RTC_SETTING_MONTH]--;
                    if(rtc_setting_values[RTC_SETTING_MONTH] < 1) {
                        rtc_setting_values[RTC_SETTING_MONTH] = 12;
                    }
                    break;
                case RTC_SETTING_DAY:
                    rtc_setting_values[RTC_SETTING_DAY]--;
                    if(rtc_setting_values[RTC_SETTING_DAY] < 1) {
                        rtc_setting_values[RTC_SETTING_DAY] = 31;
                    }
                    break;
                case RTC_SETTING_HOUR:
                    rtc_setting_values[RTC_SETTING_HOUR]--;
                    if(rtc_setting_values[RTC_SETTING_HOUR] < 0) {
                        rtc_setting_values[RTC_SETTING_HOUR] = 23;
                    }
                    break;
                case RTC_SETTING_MINUTE:
                    rtc_setting_values[RTC_SETTING_MINUTE]--;
                    if(rtc_setting_values[RTC_SETTING_MINUTE] < 0) {
                        rtc_setting_values[RTC_SETTING_MINUTE] = 59;
                    }
                    break;
                case RTC_SETTING_SECOND:
                    rtc_setting_values[RTC_SETTING_SECOND]--;
                    if(rtc_setting_values[RTC_SETTING_SECOND] < 0) {
                        rtc_setting_values[RTC_SETTING_SECOND] = 59;
                    }
                    break;
            }
            draw_menu();
            return;

    }
}

void handle_menu_button_press(int button) {
    switch(menu_state) {
        case MAIN_MENU:
            handle_main_menu_input(button);
            draw_menu();
            break;
        case CALIBRATE_PIR:
            set_menu(MAIN_MENU);
            draw_menu();
            break;
        case SET_RTC:
            handle_rtc_button_press(button);
            break;
        case DETECT_MOTION:
            set_menu(MAIN_MENU);
            draw_menu();
            break;
    }
}

void tick_main_menu() {
    return;
}

void tick_calibrate_pir() {
    if(detect_pir()) {
        set_mcp_led(MCP_LED_PIN_1, true);
        // get timestamp from rtc
        auto timestamp = getTimestamp();
        auto now = timestamp.c_str();
        Serial.println(now);
        delay(min_delay * 1000);
        set_mcp_led(MCP_LED_PIN_1, false);
    }
}

void tick_set_rtc() {
    return;
}

void tick_detect_motion() {
    if(detect_pir()) {
        set_mcp_led(MCP_LED_PIN_1, true);
        for(int i = 0; i < burst; i++) {
            // take a picture and save it (with timestamp
            auto fb = take_picture();
            auto timestamp = millis();
            String pathString = "/image_" + String(timestamp) + ".jpg";
            const char* path = pathString.c_str();
            save_picture(fb, path);
            release_picture(fb);
            delay(100);
        }
        delay(min_delay * 1000);
        set_mcp_led(MCP_LED_PIN_1, false);
    }
}

void tick_menu() {

    // check buttons
    if(is_button_pressed(MCP_BUTTON_PIN_1)) {
        handle_menu_button_press(B_LEFT);
    } else if(is_button_pressed(MCP_BUTTON_PIN_2)) {
        handle_menu_button_press(B_UP);
    } else if(is_button_pressed(MCP_BUTTON_PIN_3)) {
        handle_menu_button_press(B_DOWN);
    } else if(is_button_pressed(MCP_BUTTON_PIN_4)) {
        handle_menu_button_press(B_RIGHT);
    } else {
        switch(menu_state) {
            case MAIN_MENU:
                tick_main_menu();
                break;
            case CALIBRATE_PIR:
                tick_calibrate_pir();
                break;
            case SET_RTC:
                tick_set_rtc();
                break;
            case DETECT_MOTION:
                tick_detect_motion();
                break;
        }
    }

}