#define _GNU_SOURCE
#define __USE_XOPEN
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "backlightd.h"
#include "timefunctions.h"


static int min(int a, int b)
{
    if (a <= b) return a;
    else return b;
}

// Wait in SLEEP_TIMEOUT second intervals until the given timestamp
void wait_until(time_t timestamp)
{
    int seconds = seconds_to_timestamp(timestamp);
    while (seconds > 0)
    {
        sleep(min(seconds, SLEEP_TIMEOUT));
        seconds = seconds_to_timestamp(timestamp);
    }
}


int seconds_to_timestamp(time_t timestamp)
{
    time_t now = time(NULL);
    return (int)(timestamp - now);
}


// Convert ISO 8601 formatted time representation to unixtime
time_t isostring_to_timestamp(const char* timestr)
{
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(timestr, "%Y-%m-%dT%H:%M:%S+00:00", &tm);
    return timegm(&tm);
}