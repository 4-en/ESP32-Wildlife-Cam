#ifndef WLC_MENU_H

#define WLC_MENU_H

#define MAIN_MENU 0
#define CALIBRATE_PIR 1
#define SET_RTC 2
#define DETECT_MOTION 3

#define MM_DETECT_MOTION 0
#define MM_CALIBRATE_PIR 1
#define MM_SET_RTC 2
#define MM_MIN_DELAY 3
#define MM_SENSITIVITY 4
#define MM_BURST 5
#define MM_CAPTURED_COUNT 6

#define RTC_SETTING_YEAR 0
#define RTC_SETTING_MONTH 1
#define RTC_SETTING_DAY 2
#define RTC_SETTING_HOUR 3
#define RTC_SETTING_MINUTE 4
#define RTC_SETTING_SECOND 5

#endif

bool init_display();
void draw_menu();
void set_menu(int menu);
void handle_menu_button_press(int button);
void tick_menu();