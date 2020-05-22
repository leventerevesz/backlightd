#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <curl/curl.h>
#include "jsmn.h"
#include "backlightd.h"
#include "timefunctions.h"
#include "apicalls.h"


static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}


// Parse JSON response, return sunset and sunrise times.
int load_response(char *json_string, sun_times_handle_t times)
{
    int i;
    int r;
    jsmn_parser p;
    jsmntok_t t[128]; /* We expect no more than 128 tokens */
    char sunrisestr[80];
    char sunsetstr[80];

    jsmn_init(&p);
    r = jsmn_parse(&p, json_string, strlen(json_string), t,
                   sizeof(t) / sizeof(t[0]));
    if (r < 0)
    {
        syslog(LOG_ERR, "Failed to parse JSON: %d\n", r);
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
        syslog(LOG_ERR, "Object expected\n");
        return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++)
    {
        if (jsoneq(json_string, &t[i], "sunrise") == 0)
        {
            /* We may use strndup() to fetch string value */
            memset(sunrisestr, 0, 80);
            strncpy(sunrisestr, json_string + t[i + 1].start, t[i + 1].end - t[i + 1].start);
            i++;
        }
        else if (jsoneq(json_string, &t[i], "sunset") == 0)
        {
            /* We may additionally check if the value is either "true" or "false" */
            memset(sunsetstr, 0, 80);
            strncpy(sunsetstr, json_string + t[i + 1].start, t[i + 1].end - t[i + 1].start);
            i++;
        }
    }

    // Convert strings to timestamps
    times->sunrise = isostring_to_timestamp(sunrisestr);
    times->sunset = isostring_to_timestamp(sunsetstr);
    return 0;
}


// curl callback function
size_t my_write_callback(char *ptr, size_t size, size_t nmemb, char *buff)
{
    memset(buff, 0, 1000);
    strncpy(buff, ptr, size * nmemb);
    return size * nmemb;
}


// Get sunrise-sunset times from api.sunrise-sunset.org
int apicall(double latitude, double longitude, const char *date, sun_times_handle_t times)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        char buff[1000];
        char endpoint[120];
        sprintf(endpoint, "http://api.sunrise-sunset.org/json?lat=%.4lf&lng=%.4lf&date=%s&formatted=0", latitude, longitude, date);
        curl_easy_setopt(curl, CURLOPT_URL, endpoint);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buff);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            syslog(LOG_ERR, "curl_easy_perform() failed: %s\n",
                   curl_easy_strerror(res));

        load_response(buff, times);
        syslog(LOG_DEBUG, "Sunrise timestamp: %ld", times->sunrise);
        syslog(LOG_DEBUG, "Sunset  timestamp: %ld", times->sunset);

        curl_easy_cleanup(curl);
    }
    return 0;
}