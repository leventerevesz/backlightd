/* Sunrise and sunset computation utilities.
   Copyright (c) 2006, Mark S. Teel (mark@teel.ws)

   This source code is released for free distribution under the terms
   of the GNU General Public License.  */

#ifndef SUNRISE_H
#define SUNRISE_H

#define SUNRISE_SUNSET -35.0/60.0
#define CIVIL_TWILIGHT -6.0
#define NAUTICAL_TWILIGHT -12.0
#define ASTRONOMICAL_TWILIGHT -18.0


/*  Calculate sunrise and sunset times.
 *
 *  year: between 2000 and 2099
 *  month
 *  day
 *  lon: longitude, positive for east, negative for west of the Prime Meridian.
 *  lat: latitude, positive for north, negative for south of the equator.
 *  twilight_type: SUNRISE_SUNSET or CIVIL_TWILIGHT or NAUTICAL_TWILIGHT or
 *          ASTRONOMICAL_TWILIGHT 
 *  upper_limb: use the upper limb of the Sun (1) or its center (0) to 
 *          calculate sunset and sunrise times.
 *  trise: sunrise time in hours UTC
 *  tset: sunset time in hours UTC
 *
 *  returns: 0 if Sun sets on given day, 
 *          +1 if the Sun stays above the "horizon" specified in twilight_type
 *          -1 if the Sun stays under the "horizon" specified in twilight_type
 */
int calc_sun_rise_set(
    int year, int month, int day, double lon, double lat,
    double twilight_type, int upper_limb, double *trise, double *tset);


/*  Calculate day length.
 *
 *  year: between 2000 and 2099
 *  month
 *  day
 *  lon: longitude, positive for east, negative for west of the Prime Meridian.
 *  lat: latitude, positive for north, negative for south of the equator.
 *  twilight_type: SUNRISE_SUNSET or CIVIL_TWILIGHT or NAUTICAL_TWILIGHT or
 *          ASTRONOMICAL_TWILIGHT 
 *  upper_limb: use the upper limb of the Sun (1) or its center (0) to 
 *          calculate sunset and sunrise times.
 *
 *  returns: 0 if Sun sets on given day, 
 *          +1 if the Sun stays above the "horizon" specified in twilight_type
 *          -1 if the Sun stays under the "horizon" specified in twilight_type
 */
double calc_day_length(
    int year, int month, int day, double lon, double lat,
    double twilight_type, int upper_limb);

#endif