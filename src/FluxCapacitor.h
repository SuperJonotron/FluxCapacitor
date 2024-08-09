#ifndef FLUXCAPACITOR_H
#define FLUXCAPACITOR_H

#include <Timezone.h>

/* Not defined for esp8266. Probably other boards too
 * so just going to add to to any board that might be
 * missing it.
 */
#ifndef LONG_MAX
#define LONG_MAX 2147483647L // Max 32 bit Long Value
#endif

long getLastDate(time_t curDate,Timezone tz, std::vector<time_t> dates, int subYears);
long getNextDate(time_t curDate,Timezone tz, std::vector<time_t> dates, int addYears);
bool isAfter(time_t date1, time_t date2);
bool isBefore(time_t date1, time_t date2);
bool isSameDay(time_t date1, time_t date2);
void printDigits(int val);
void printInt00(int val);
void printTime(time_t t, Timezone tz);
time_t setYear(time_t utc, Timezone tz, int year);
time_t toTimezone(time_t utc, Timezone tz);

#endif