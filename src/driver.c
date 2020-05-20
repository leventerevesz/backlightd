#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "backlightd.h"


static int read_raw_actual_brightness(const char *interface)
{
    FILE *file;
    char path[80];
    int actual_brightness;

    strcpy(path, interface);
    strcat(path, "/actual_brightness");
    file = fopen(path, "r");
    fscanf(file, "%d", &actual_brightness);
    fclose(file);
    return actual_brightness;
}


static int read_raw_max_brightness(const char *interface)
{
    FILE *file;
    char path[80];
    int max_brightness;

    strcpy(path, interface);
    strcat(path, "/max_brightness");
    file = fopen(path, "r");
    fscanf(file, "%d", &max_brightness);
    fclose(file);
    return max_brightness;
}


// Get the currently set screen brightness in percent.
int get_current_brightness(const char *interface)
{
    int actual_brightness = read_raw_actual_brightness(interface);
    int max_brightness = read_raw_max_brightness(interface);
    int brightness = (int)(100 * (double)actual_brightness / (double)max_brightness);
    return brightness;
}


static int validate_raw_brightness(int raw_brightness, const char* interface)
{
    int max_brightness = read_raw_max_brightness(interface);
    if (raw_brightness < 0 || raw_brightness > max_brightness)
    {
        syslog(LOG_ERR, "Illegal raw brightness value: %d\n", raw_brightness);
        return -1;
    }
    return 0;
}


static void write_raw_brightness(int raw_brightness, const char* interface)
{
    FILE *file;
    char path[80];

    if (validate_raw_brightness(raw_brightness, interface) != 0)
    {
        return;
    }

    strcpy(path, interface);
    strcat(path, "/brightness");
    if (access(path, W_OK) != 0)
    {
        syslog(LOG_ERR, "Write permission denied on %s\n", path);
        exit(EXIT_FAILURE);
    }
    file = fopen(path, "w");
    fprintf(file, "%d", raw_brightness);
    fclose(file);
}


// Set the screen brightness in percent.
void set_brightness(int brightness, const char *interface)
{
    int max_brightness = read_raw_max_brightness(interface);
    int raw_brightness = (int)(brightness / 100.0 * max_brightness);
    write_raw_brightness(raw_brightness, interface);
}