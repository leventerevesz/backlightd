#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "backlightd.h"
#include "timecalc.h"

// Sleeps are interrupted every SLEEP_TIMEOUT seconds, so that no actions are skipped
// due to device suspend. 
#define SLEEP_TIMEOUT 600

static int min(int a, int b)
{
    if (a <= b) return a;
    else return b;
}

static void wait_for_sunrise(config_handle_t config)
{
    int seconds = seconds_before_sunrise(config);
    while (seconds > 0)
    {
        sleep(min(seconds, SLEEP_TIMEOUT));
        seconds = seconds_before_sunrise(config);
    }
}

static void wait_for_sunset(config_handle_t config)
{
    int seconds = seconds_before_sunset(config);
    while (seconds > 0);
    {
        sleep(min(seconds, SLEEP_TIMEOUT));
        seconds = seconds_before_sunset(config);
    }
}

static void wait()
{
    sleep(SLEEP_TIMEOUT);
}

void quit(int signum)
{
    syslog(LOG_INFO, "Backlightd shutting down...\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    openlog("backlightd", 0, LOG_DAEMON);

    if(daemon(0, 0) < 0)
    {
        perror("daemon");
        return EXIT_FAILURE;
    }

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = quit;
    sigaction(SIGTERM, &action, NULL);

    config_t config_s;
    config_handle_t config = &config_s;
    load_config(CONFIG_PATH, config);

    syslog(LOG_INFO, "Backlightd starting up...\n");

    while (1)
    {
        if (is_before_sunrise(config))
        {
            wait_for_sunrise(config);
            set_brightness(config->brightness_max, config->interface);
            syslog(LOG_INFO, "Screen brightness set to daytime level.\n");
        }
        else if (is_before_sunset(config))
        {
            wait_for_sunset(config);
            set_brightness(config->brightness_min, config->interface);
            syslog(LOG_INFO, "Screen brightness set to nighttime level.\n");
        }
        else
        {
            wait();
        }
    }

    return EXIT_SUCCESS;
    closelog();
}