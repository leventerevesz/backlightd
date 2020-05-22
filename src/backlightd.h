#ifndef BACKLIGHTD_H
#define BACKLIGHTD_H

#define CONFIG_PATH "/etc/backlightd/conf"
#define PIDFILE "/run/backlightd.pid"

#define SLEEP_TIMEOUT 60
#define TRANSITION_TIME 60

typedef struct {
    double longitude;
    double latitude;
    int brightness_max;
    int brightness_min;
    char interface[80];
} config_t, *config_handle_t;


// Load configuration from file.
int load_config(const char *config_path, config_handle_t config);

// Get the currently set screen brightness in percent.
int get_current_brightness(const char *interface);

// Set the screen brightness in percent.
void set_brightness(int brightness, const char *interface);

void brightness_transition(const char *interface, int current, int goal);

#endif