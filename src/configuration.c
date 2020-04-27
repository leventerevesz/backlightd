#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include "backlightd.h"

int is_valid_line(char *line)
{
    if (line[0] == '#' || line[0] == '\n')
        return 0;
    else if (strchr(line, '=') == NULL)
        return 0;
    else
        return 1;
}

int parse_config_line(char *line, config_handle_t config)
{
    if (is_valid_line(line) == 0)
        return -1;
    
    char *name = strtok(line, "=");
    char *value = strtok(NULL, "\n");
    if (name == NULL || value == NULL)
        return -2;
    
    
    if (strncmp(name, "longitude", 9) == 0) 
    {
        double dval;
        if (sscanf(value, "%lf", &dval) == 1)
            config->latitude = dval;
        else
            syslog(LOG_ERR, "Error parsing config: %s", name);
    }
    else if (strncmp(name, "latitude", 8) == 0)
    {
        double dval;
        if (sscanf(value, "%lf", &dval) == 1)
            config->longitude = dval;
        else
            syslog(LOG_ERR, "Error parsing config: %s", name);
    }
    else if (strncmp(name, "automatic", 9) == 0)
    {
        char bval[6];
        if (sscanf(value, "%s", bval) == 1)
        {
            if (strncmp(bval, "true", 4) == 0)
                config->automatic = 1;
            else if (strncmp(bval, "false", 5) == 0)
                config->automatic = 0;
            else
                syslog(LOG_ERR, "Error parsing config: %s", name);
        }
        else
            syslog(LOG_ERR, "Error parsing config: %s", name);
    }
    else if (strncmp(name, "interface", 9) == 0)
    {
        char sval[80];
        if (sscanf(value, "%s", sval) == 1)
            strncpy(config->interface, sval, 80);
        else
            syslog(LOG_ERR, "Error parsing config: %s", name);
    }
    else
    {
        return 1;
    }
    return 0;
}

int read_config(const char *config_path, config_handle_t config) 
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
