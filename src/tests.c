#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "backlightd.h"
#include "timecalc.h"

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

void test_sunset_sunrise_times(void)
{   
    config_t config;
    read_config(CONFIG_PATH, &config);
    time_t sunrise = get_today_sunrise_timestamp(&config);
    time_t sunset = get_today_sunset_timestamp(&config);
    printf("Sunrise time: (%ld) %s", sunrise, ctime(&sunrise));
    printf("Sunset  time: (%ld) %s", sunset, ctime(&sunset));
    printf("Time to sunrise: %lf hours\n", seconds_before_sunrise(&config)/3600.0);
    printf("Time to sunset : %lf hours\n", seconds_before_sunset(&config)/3600.0);
}

int main(void)
{
    print_config();
    //test_get_current_brightness();
    //test_set_brightness();
    //test_get_current_brightness();
    test_sunset_sunrise_times();
    return 0;
}