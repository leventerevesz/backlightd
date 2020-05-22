#ifndef TIMECALC_H
#define TIMECALC_H


typedef struct
{
    time_t sunrise;
    time_t sunset;
} sun_times_t, *sun_times_handle_t;


// Returns the number of seconds left until timestamp.
int seconds_to_timestamp(time_t timestamp);

// Wait in SLEEP_TIMEOUT second intervals until the given timestamp
void wait_until(time_t timestamp);

// Convert ISO 8601 formatted time representation to unixtime
time_t isostring_to_timestamp(const char* timestr);

#endif