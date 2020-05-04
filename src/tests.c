#include <stdio.h>
#include <stdlib.h>
#include "backlightd.h"

void print_config(void)
{
    config_t config;

    read_config(CONFIG_PATH, &config);
    printf("Config \n    %s\n", CONFIG_PATH);
    printf("    longitude  %lf\n    latitude   %lf\n    brightness_min  %d\n    brightness_max  %d\n    automatic  %d\n    interface  %s\n", 
        config.longitude, config.latitude, config.brightness_min, config.brightness_max, config.automatic, config.interface);
}

void test_get_current_brightness(void)
{
    config_t config;
    read_config(CONFIG_PATH, &config);
    int current_brightness = get_current_brightness(config.interface);
    printf("Actual brightness  %d\n", current_brightness);
}

void test_set_brightness(void)
{
    config_t config;
    read_config(CONFIG_PATH, &config);
    set_brightness(77, config.interface);
}

int main(void)
{
    print_config();
    test_get_current_brightness();
    test_set_brightness();
    test_get_current_brightness();
    return 0;
}