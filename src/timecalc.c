#include <stdlib.h>
#include <time.h>
#include "backlightd.h"
#include "sunrise.h"
#include "timecalc.h"


static struct tm utc2localtime(struct tm utctm)
{
    time_t tmp = timegm(&utctm);
    struct tm *localtm = localtime(&tmp);
    return *localtm;
}


static time_t utc_hour_to_timestamp(double hour)
{
    time_t t = time(NULL);
    struct tm *now = gmtime(&t);
    struct tm time_utc = *now;
    time_utc.tm_hour = (int)hour;
    time_utc.tm_min = (int)(60 * (hour - (int)hour));
    
    return timegm(&time_utc);
}


static double get_today_sunrise_hour_utc(const config_handle_t config)
{
    time_t t = time(NULL);
    struct tm *now = gmtime(&t);
    
    double trise, tset;
    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    
    calc_sun_rise_set(
        year, month, day, config->longitude, config->latitude, SUNRISE_SUNSET, 1, 
        &trise, &tset);
    return trise;
}

time_t get_today_sunrise_timestamp(const config_handle_t config)
{
    double trise = get_today_sunrise_hour_utc(config);
    return utc_hour_to_timestamp(trise);
}


static double get_today_sunset_hour_utc(const config_handle_t config)
{
    time_t t = time(NULL);
    struct tm *now = gmtime(&t);
    
    double trise, tset;
    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    
    calc_sun_rise_set(
        year, month, day, config->longitude, config->latitude, SUNRISE_SUNSET, 1, 
        &trise, &tset);
    return tset;
}

time_t get_today_sunset_timestamp(const config_handle_t config)
{
    double tset = get_today_sunset_hour_utc(config);
    return utc_hour_to_timestamp(tset);
}

int seconds_before_sunrise(const config_handle_t config)
{
    time_t now = time(NULL);
    time_t sunrise = get_today_sunrise_timestamp(config);
    return (int)(sunrise - now);
}

int seconds_before_sunset(const config_handle_t config)
{
    time_t now = time(NULL);
    time_t sunset = get_today_sunset_timestamp(config);
    return (int)(sunset - now);
}

int is_before_sunrise(config_handle_t config)
{
    return seconds_before_sunrise(config) > 0;
}

int is_before_sunset(config_handle_t config)
{
    return seconds_before_sunset(config) > 0;
}