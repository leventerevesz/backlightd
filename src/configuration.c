#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include "backlightd.h"


static int is_valid_line(char *line)
{
    if (line[0] == '#' || line[0] == '\n')
        return 0;
    else if (strchr(line, '=') == NULL)
        return 0;
    else
        return 1;
}


// Split a line into a key-value pair.
static int split_config_pair(char *line, char **name, char **value)
{
    *name = strtok(line, "=");
    *value = strtok(NULL, "#");
    if (name == NULL || value == NULL)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


// Load one key-value pair to the config struct.
static int load_config_option(const char *name, const char *value, config_handle_t config)
{
    if (strncmp(name, "longitude", 9) == 0) 
    {
        double dval;
        if (sscanf(value, "%lf", &dval) == 1)
            config->longitude = dval;
    }
    else if (strncmp(name, "latitude", 8) == 0)
    {
        double dval;
        if (sscanf(value, "%lf", &dval) == 1)
            config->latitude = dval;
    }
    else if (strncmp(name, "interface", 9) == 0)
    {
        char sval[80];
        if (sscanf(value, "%s", sval) == 1)
            strncpy(config->interface, sval, 80);
    }
    else if (strncmp(name, "brightness_min", 14) == 0)
    {
        int ival;
            if (sscanf(value, "%d", &ival) == 1)
                config->brightness_min = ival;
    }
    else if (strncmp(name, "brightness_max", 14) == 0)
    {
        int ival;
            if (sscanf(value, "%d", &ival) == 1)
                config->brightness_max = ival;
    }
    else
    {
        return -1;
    }
    return 0;
}


// Parse one line of the config file, and save it to the config struct.
static int parse_config_line(char *line, config_handle_t config)
{
    if (is_valid_line(line) == 0) return 1;
    
    char *name = NULL;
    char *value = NULL;
    if (split_config_pair(line, &name, &value) != 0)
    {
        return 1;
    }

    int e = load_config_option(name, value, config);
    if (e == -1)
    {
        syslog(LOG_ERR, "Error reading config line: %s", name);
    }
    return 0;
}


// Main function to load the config file.
int load_config(const char *config_path, config_handle_t config) 
{
    FILE *conffile = fopen(config_path, "r");
    if (conffile == NULL)
    {
        syslog(LOG_ERR, "Could not open config file %s", CONFIG_PATH);
    }

    char buf[80];
    while (!feof(conffile))
    {
        memset(buf, 0, 80);
        fgets(buf, 79, conffile);
        parse_config_line(buf, config);
    }
    
    fclose(conffile);
    return 0;
}
