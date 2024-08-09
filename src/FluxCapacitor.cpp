/*
* The Flux Capacitor was invented by Dr. Emmit L. Brown, "Doc". 
* On November 5, 1955, the idea for this invention came when he 
* was standing on the edge of his toilet hanging a clock.  
* The porcelain was wet, which caused him to slip and hit his 
* head on the edge of the sink.  When he came to, he had a revelation, 
* a vision, a picture in his head.  It was the Flux Capacitor and 
* it's what makes time travel possible.
*
* Unfortunately we do not have a real Flux Capacitor for
* time trevel so for the purposes of this library, 
* the Flux Capacitor will be responsible for all time related 
* methods and manipulation.
*/
#include <FluxCapacitor.h>

 /**
 * Resolves the prevoius date compared to curent time from the array of dates provided. Each date
 * provided is adjusted by the current year to account for recurring events to determine the
 * correct next date.
 *
 * time_t curDate - Current date and time in UTC
 * Timezone tz - Timezone to adjust all times into for comparing
 * time_t dates[] - Dates expected to be static from one of the event arrays (birthday,
 * anniversaries, etc). These will always be in the past. 
 * int len - Length of the array to be used. 
 * int addYears - Number of additional years to remove for automatic adjustment. 
 * Allows for recursive action to always find the last date even if no more exist for 
 * the current year.
 */
long getLastDate(time_t curDate,Timezone tz, std::vector<time_t> dates, int subYears) {
  long minDiff = LONG_MAX;
  long nextTime = 0;
  int curYear = year(curDate);
  for(std::size_t i=0; i<dates.size(); ++i){
    time_t eventDate = setYear(dates[i],tz, curYear - subYears);
    long diff = isBefore(curDate, eventDate) ? curDate - eventDate : LONG_MAX;
    if (diff < minDiff) {
      minDiff = diff;
      nextTime = eventDate;
    }
  }
  if (nextTime == 0) {
    return getLastDate(curDate,tz, dates, ++subYears);
  }
  return nextTime;
 }

/**
 * Resolves the next date compared to curent time from the array of dates provided. Each date
 * provided is adjusted by the current year to account for recurring events to determine the
 * correct next date.
 *
 * time_t curDate - Current date and time in UTC
 * Timezone tz - Timezone to adjust all times into for comparing
 * time_t dates[] - Dates expected to be static from one of the event arrays (birthday,
 * anniversaries, etc). These will always be in the past. int len - Length of the array to be
 * used. 
 * int addYears - Number of additional years to add to automatic adjustment. Allows for
 * recursive action to always find the next date even if no more exist for the current year.
 */
long getNextDate(time_t curDate,Timezone tz, std::vector<time_t> dates,int addYears) {
  long minDiff = LONG_MAX;
  long nextTime = 0;
  int curYear = year(curDate);
  for(std::size_t i=0; i<dates.size(); ++i){
    time_t eventDate = setYear(dates[i],tz, curYear + addYears);
    long diff = isAfter(curDate, eventDate) ? eventDate - curDate : LONG_MAX;
    if (diff < minDiff) {
      minDiff = diff;
      nextTime = eventDate;
    }
  }
  if (nextTime == 0) {
    return getNextDate(curDate,tz, dates, ++addYears);
  }
  return nextTime;
 }

/**
* Compare two time_t instances to determine 
* if one of them is occurs after the other one.
*
* date1 - time_t object used as the base comparison
* date2 - time_t object checked to see if it occurs
* after date 1.
* return - boolean value of the result
  True - If date2 occurs after date1
  False - If date2 does not occur after date1
*/
bool isAfter(time_t date1, time_t date2) {
  return date1 < date2;
}
/**
* Compare two time_t instances to determine 
* if one of them is occurs after the other one.
*
* date1 - time_t object used as the base comparison
* date2 - time_t object checked to see if it occurs
* before date 1.
* return - boolean value of the result
  True - If date2 occurs before date1
  False - If date2 does not occur before date1
*/

bool isBefore(time_t date1, time_t date2) {
  return date1 > date2;
}

/**
* Check if two time_t instances occur on the
* same date.  Does not compare time of day
* only month, year and day
* 
* date1 - time_t object of the first date to compare
* date2 - time_t object of the second date to compare
* return - boolean value of result.
  True - dates are on the same day
  False - dates are not on the same day
*/
bool isSameDay(time_t date1, time_t date2) {
  return (year(date1) == year(date2))
  && (month(date1) == month(date2))
  && (day(date1) == day(date2));
}

/*
 * Print an integer in ":00" format (with leading zero).
 * Input value assumed to be between 0 and 99.s
 */
void printDigits(int val)
{
  Serial.print(':');
  if (val < 10) Serial.print('0');
  Serial.print(val, DEC);
}

/*
 * Print an integer in "00" format (with leading zero)
 * Input value assumed to be between 0 and 99.
 */
void printInt00(int val)
{
  if (val < 10) Serial.print('0');
  Serial.print(val, DEC);
  return;
}

/*
 * Print a time_t object to the serial console
 * and adjust the time to the timezone provided
 * in the format 
 */
void printTime(time_t t, Timezone tz)
{
  TimeChangeRule *tcr;
  t = tz.toLocal(t,&tcr);
  printInt00(hour(t)>12?hour(t)-12:hour(t));
  printDigits(minute(t));
  printDigits(second(t));
  Serial.print(' ');
  Serial.print(dayShortStr(weekday(t)));
  Serial.print(' ');
  printInt00(day(t));
  Serial.print(' ');
  Serial.print(monthShortStr(month(t)));
  Serial.print(' ');
  Serial.print(year(t));
  Serial.print(' ');
  Serial.print(tcr -> abbrev);
  Serial.print(' ');
  Serial.println();
}

/**
* Set a time_t instance in UTC/GMT to the year and
* timezone specified and return it in the timezone
* specified. Conversion to the timezone specified
* happens before adjusting the year to normalize
* the hour portion which can have issues over DST
* if the time is left in UTC.
*
* utc - time_t object to be adjusted
* tz - Timezone to use when converting the time
* year - The year to adjust the time provided to
* return - time_t object at the same time and day but
* for the specified year in UTC
*/
time_t setYear(time_t utc, Timezone tz, int year) {
  utc = toTimezone(utc, tz);
  tmElements_t myElements = {
    static_cast<uint8_t>(second(utc)), 
    static_cast<uint8_t>(minute(utc)), 
    static_cast<uint8_t>(hour(utc)), 
    static_cast<uint8_t>(weekday(utc)), 
    static_cast<uint8_t>(day(utc)), 
    static_cast<uint8_t>(month(utc)), 
    static_cast<uint8_t>(year - 1970)
  };
  //makeTime creates time in UTC but because the
  //base time was actually in the specified timezone,
  //calling toUTC read adjusts this difference back into UTC
  return tz.toUTC(makeTime(myElements));
}

/*
* Convert a time_t object from utc time to 
* the timezone defined.
*
* utc - time_t object in UTC/GMT time
* tz - Timezone object of the timezone to 
  convert the utc time to
* return - time_t object of the utc time converted
  to the local time defined by the Timezone 
  provided
*/  
time_t toTimezone(time_t utc, Timezone tz){
  TimeChangeRule *tcr;
  time_t dateLocal = tz.toLocal(utc, &tcr);
  return dateLocal;
}
