#include <stdio.h>
#include <stdlib.h>
#include "backlightd.h"

void print_config(void)
{
    config_t config;

    read_config(CONFIG_PATH, &config);
    printf("Config \n    %s\n", CONFIG_PATH);
    printf("    longitude  %lf\n    latitude   %lf\n    automatic  %d\n    interface  %s\n", 
        config.longitude, config.latitude, config.automatic, config.interface);
}

int main()
{

    print_config();
    return 0;
}