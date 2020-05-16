#ifndef BACKLIGHTD_H
#define BACKLIGHTD_H

#define CONFIG_PATH "/etc/backlightd/conf"

typedef struct {
    double longitude;
    double latitude;
    int brightness_max;
    int brightness_min;
    int automatic;
    char interface[80];
} config_t, *config_handle_t;

int read_config(const char *config_path, config_handle_t config);

int get_current_brightness(const char *interface);
void set_brightness(int brightness, const char *interface);

#endif