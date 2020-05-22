#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include "backlightd.h"
#include "timefunctions.h"
#include "apicalls.h"

static int pid_file;
static int rc;

void brightness_transition(const char *interface, int current, int goal)
{
    syslog(LOG_INFO, "Transition %d -> %d in %d seconds...\n", current, goal, TRANSITION_TIME);
    double stepsize = (goal - current) / 5.0;
    for (int step = 1; step <= 5; step++)
    {
        int brightness = current + (int)(step * stepsize);
        set_brightness(brightness, interface);
        syslog(LOG_DEBUG, "  - step %d: %d\n", step, brightness);
        sleep(TRANSITION_TIME / 5);
    }
}

void quit(int signum)
{
    syslog(LOG_INFO, "Backlightd shutting down...\n");
    rc = flock(pid_file, LOCK_UN);
    close(pid_file);
    unlink(PIDFILE);
    exit(EXIT_SUCCESS);
}

int main()
{
    openlog("backlightd", 0, LOG_DAEMON);

    if (geteuid() != 0)
    {
        printf("Please run this program with sudo.\n");
        exit(EXIT_FAILURE);
    }

    // Create PIDfile
    pid_file = open(PIDFILE, O_CREAT | O_RDWR, 0666);
    rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc) {
        if(EWOULDBLOCK == errno)
            exit(EXIT_FAILURE);
    }
    
    if(daemon(0, 0) < 0)
    {
        perror("daemon");
        exit(EXIT_FAILURE);
    }
    char pid[10];
    sprintf(pid, "%d", getpid());
    write(pid_file, pid, strlen(pid));

    // Handle SIGTERM signal
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = quit;
    sigaction(SIGTERM, &action, NULL);

    // Load configuration from CONFIG_PATH
    config_t config_s;
    config_handle_t config = &config_s;
    load_config(CONFIG_PATH, config);

    syslog(LOG_INFO, "Backlightd starting up...\n");

    sun_times_t suntimes; // the NEXT sunset and sunrise times

    // Load today's sunset-sunrise times
    apicall(config->latitude, config->longitude, "today", &suntimes);
    while (1)
    {
        if (seconds_to_timestamp(suntimes.sunrise) > 0)
        {
            syslog(LOG_DEBUG, "Waiting for sunrise...\n");
            wait_until(suntimes.sunrise);
            brightness_transition(
                config->interface,
                get_current_brightness(config->interface),
                config->brightness_max);
        }
        else if (seconds_to_timestamp(suntimes.sunset) > 0)
        {
            syslog(LOG_DEBUG, "Waiting for sunset...\n");
            wait_until(suntimes.sunset);
                brightness_transition(
                    config->interface,
                    get_current_brightness(config->interface),
                    config->brightness_min);
        }
        else
        {
            // Load tomorrow's sunset-sunrise times
            apicall(config->latitude, config->longitude, "tomorrow", &suntimes);
        }
    }

    closelog();
    exit(EXIT_SUCCESS);
}