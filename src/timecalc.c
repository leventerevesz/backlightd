#include <stdlib.h>
#include <time.h>
#include "backlightd.h"
#include "sunrise.h"

double get_current_time_of_day()
{
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    return now->tm_hour + (now->tm_min)/60.0 + (now->tm_sec)/3600.0;
}


static struct tm utc2localtime(struct tm utctm)
{
    time_t tmp = timegm(&utctm);
    struct tm *localtm = localtime(&tmp);
    return *localtm;
}


double get_today_sunrise_time(const config_handle_t config)
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
    
    struct tm sunrise, sunrise_utc;
    sunrise_utc = *now;
    sunrise_utc.tm_hour = (int)trise;
    sunrise_utc.tm_min = (int)(60 * (trise - (int)trise));
    sunrise = utc2localtime(sunrise_utc);

    return sunrise.tm_hour + (sunrise.tm_min)/60.0 + (sunrise.tm_sec)/3600;
}


double get_today_sunset_time(const config_handle_t config)
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
    
    struct tm sunset, sunset_utc;
    sunset_utc = *now;
    sunset_utc.tm_hour = (int)tset;
    sunset_utc.tm_min = (int)(60 * (tset - (int)tset));
    sunset = utc2localtime(sunset_utc);

    return sunset.tm_hour + (sunset.tm_min)/60.0 + (sunset.tm_sec)/3600;
}