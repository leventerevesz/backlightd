#ifndef TIMECALC_H
#define TIMECALC_H

time_t get_today_sunrise_timestamp(const config_handle_t config);
time_t get_today_sunset_timestamp(const config_handle_t config);
int seconds_before_sunrise(const config_handle_t config);
int seconds_before_sunset(const config_handle_t config);
int is_before_sunrise(config_handle_t config);
int is_before_sunset(config_handle_t config);

#endif