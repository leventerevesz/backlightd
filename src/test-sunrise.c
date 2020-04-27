#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sunrise.h"

struct tm utc2localtime(struct tm utctm)
{
    struct tm *localtm;
    time_t tmp = timegm(&utctm);
    localtm = localtime(&tmp);
    return *localtm;
}

int main()
{
    double longitude = 18.35337;
    double latitude = 46.16161;
    time_t rawtime = time(NULL);
    struct tm *now = gmtime(&rawtime);
    
    double trise, tset;
    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    
    calc_sun_rise_set(
        year, month, day, longitude, latitude, CIVIL_TWILIGHT, 1, 
        &trise, &tset);
    
    struct tm sunrise, sunrise_utc;
    sunrise_utc = *now;
    sunrise_utc.tm_hour = (int)trise;
    sunrise_utc.tm_min = (int)(60 * (trise - (int)trise));
    sunrise = utc2localtime(sunrise_utc);
    
    struct tm sunset, sunset_utc;
    sunset_utc = *now;
    sunset_utc.tm_hour = (int)tset;
    sunset_utc.tm_min = (int)(60 * (tset - (int)tset));
    sunset = utc2localtime(sunset_utc);
    
    printf("Date: %04d-%02d-%02d\n", year, month, day);
    printf("The Sun rises at %d:%02d, sets at %d:%02d.\n", 
        sunrise.tm_hour, sunrise.tm_min, sunset.tm_hour, sunset.tm_min);
    
    return 0;
}
