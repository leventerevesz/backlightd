

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