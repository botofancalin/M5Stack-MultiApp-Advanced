// DST Adjust for esp8266 arduino IDE
//
// Simplified routines to get DST adjusted time() result when within DST rule timerange
//
// Portions of code adapted from:
//    - Arduino Timezone Library by Jack Christensen
//    - Arduino TimeLib
//


#include "simpleDSTadjust.h"

// Constructor
simpleDSTadjust::simpleDSTadjust(struct dstRule startRule, struct dstRule endRule ) {
  dstStartRule = startRule;
  dstEndRule = endRule;
}

// Public
time_t simpleDSTadjust::time(char **abbrev)
{
 time_t now = ::time(NULL);  // Call the original time() function
 uint8_t year = calcYear(now);
 static time_t dstStart;  // Start of DST in specific Year (seconds since 1970)
 static time_t dstEnd;    // End of DST in listed Year (seconds since 1970)
 
  // Init DST variables if necessary
  if(dstYear!=year)
   {
    dstYear=year;
    dstStart = calcTime(&dstStartRule);
    dstEnd = calcTime(&dstEndRule);
	
    Serial.println("\nDST Rules Updated:");
    Serial.print("DST Start: ");
    Serial.print(ctime(&dstStart));
    Serial.print("DST End:   ");
    Serial.println(ctime(&dstEnd));
   }

 bool northTZ = (dstEnd>dstStart)?1:0; // Northern or Southern hemisphere TZ?
 
  if(northTZ && (now >= dstStart && now < dstEnd) || !northTZ && (now < dstEnd || now >= dstStart))
   {
    now += dstStartRule.offset;
	if(abbrev!=NULL)
     *abbrev = dstStartRule.abbrev;
   }
  else
   {
    if(abbrev!=NULL)
	 *abbrev = dstEndRule.abbrev;
   }
   
  return(now);
}

// Private
uint8_t simpleDSTadjust::calcYear(time_t time)
{
 uint8_t year=0;
 unsigned long days=0;
 
  time /= SECS_PER_DAY; // now it is days

  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  
  return(year);
}

/*----------------------------------------------------------------------*
 * Convert the given DST change rule to a time_t value                  *
 * for the given year.                                                  *
 * Based on code from Arduino Timezone Library by Jack Christensen      *
 *----------------------------------------------------------------------*/
time_t simpleDSTadjust::calcTime(struct dstRule * tr)
{
 struct tm tm2;
 time_t t;
 uint8_t m, w;            //temp copies
 
    m = tr->month;
    w = tr->week;
    if (w == 0) {            //Last week = 0
        if (++m > 11) {      //for "Last", go to the next month
            m = 0;
            // yr++;
        }
        w = 1;               //and treat as first week of next month, subtract 7 days later
    }

    tm2.tm_hour = tr->hour;
    tm2.tm_min = 0;
    tm2.tm_sec = 0;
    tm2.tm_mday = 1;
    tm2.tm_mon = m;
    tm2.tm_year = dstYear;

    // t = ::mktime(&tm2);        // mktime() seems to be broken, below is replacement
    t = my_mktime(&tm2);        //first day of the month, or first day of next month for "Last" rules

    t += (7 * (w - 1) + (tr->dow - weekday(t) + 7) % 7) * SECS_PER_DAY;
    if (tr->week == 0) t -= 7 * SECS_PER_DAY;    //back up a week if this is a "Last" rule
    
    return t;
}

/*----------------------------------------------------------------------*
 * Replacement for broken mktime()                                      *
 * Based on code from Paul Stoffregen's Arduino Time Library for        *
 *----------------------------------------------------------------------*/
time_t simpleDSTadjust::my_mktime(struct tm *tmptr)
{   
  int i;
  time_t seconds;
  static int8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};

  // seconds from 1970 till the given year
  seconds= tmptr->tm_year*(SECS_PER_DAY * 365);
  for (i = 0; i < tmptr->tm_year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year
  for (i = 0; i < tmptr->tm_mon; i++) {
    if ( (i == 1) && LEAP_YEAR(tmptr->tm_year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i];
    }
  }
  seconds+= (tmptr->tm_mday-1) * SECS_PER_DAY;
  seconds+= tmptr->tm_hour * SECS_PER_HOUR;
  seconds+= tmptr->tm_min * SECS_PER_MIN;
  seconds+= tmptr->tm_sec;

  return (time_t)seconds; 
}
