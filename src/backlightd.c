#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include "backlightd.h"
#include "timecalc.h"

#include <stdio.h>

int main()
{
    openlog("backlightd", 0, LOG_DAEMON);

    config_t __config;
    config_handle_t config = &__config;
    read_config(CONFIG_PATH, config);

    syslog(LOG_INFO, "Backlightd starting up...\n");

    return EXIT_SUCCESS;
    closelog();
}