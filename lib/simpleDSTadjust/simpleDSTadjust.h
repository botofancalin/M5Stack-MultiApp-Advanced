// DST Adjust Library for ESP8266/Arduino platform
//
//  Provides DST adjusted current time using user defined DST rules
//
// Portions of code adapted from:
//    - Arduino Timezone Library by Jack Christensen
//    - Arduino TimeLib

#pragma once

#include "Arduino.h"
#include <time.h>


#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
#define SECS_PER_MIN  (60)
#define SECS_PER_HOUR (3600)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24)
#define weekday(t) (((t / SECS_PER_DAY) + 4) % 7)

//convenient constants for dstRules
enum week_t {Last, First, Second, Third, Fourth}; 
enum dow_t {Sun=0, Mon, Tue, Wed, Thu, Fri, Sat};
enum month_t {Jan=0, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

//structure to describe rules for when daylight/summer time begins, or when standard time begins.
struct dstRule
 {
  char abbrev[6];    //five chars max
  uint8_t week;      //First, Second, Third, Fourth, or Last week of the month
  uint8_t dow;       //day of week, 0=Sun, 1=Mon, ... 6=Sat
  uint8_t month;     //0=Jan, 1=Feb, ... 11=Dec
  uint8_t hour;      //0-23
  int offset;        //offset from Standard Time in seconds
 };
	
class simpleDSTadjust {

  public:
	simpleDSTadjust(struct dstRule startRule, struct dstRule endRule );
	time_t time(char **abbrev);

  private:
	uint8_t calcYear(time_t time);
	time_t calcTime(struct dstRule * tr);
	time_t my_mktime(struct tm *tmptr);
    uint8_t dstYear;  // Years since 1970
    struct dstRule dstStartRule;
    struct dstRule dstEndRule;
	
};
