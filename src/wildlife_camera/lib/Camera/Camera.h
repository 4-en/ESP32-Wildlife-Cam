#include "esp_camera.h"

camera_fb_t * take_picture();
bool save_picture(camera_fb_t * picture, const char * path);
void release_picture(camera_fb_t * picture);
bool setup_camera();
bool setup_mmc();
void write_text(const char * path, const char * message);