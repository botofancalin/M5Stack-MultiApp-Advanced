#pragma once
#include "M5StackMod.h"
#include "WeatherIcons.h"
#include "JsonListener.h"
#include "WundergroundConditions.h"
#include "WundergroundForecast.h"
#include "WundergroundAstronomy.h"
#include "weathericons.h"
#include "simpleDSTadjust.h"

#define UTC_OFFSET +2
#define NTP_SERVERS "0.ch.pool.ntp.org", "1.ch.pool.ntp.org", "2.ch.pool.ntp.org"

class WeatherStationClass
{
  private:
    WGConditions conditions;
    WGForecast forecasts[12];
    WGAstronomy astronomy;

    bool IS_METRIC = true;
    bool IS_STYLE_12HR = false;
    bool drawn = false;
    bool dateDrawn = false;
    const uint8_t labelX = 110;
    const uint8_t valueX = 245;
    int fy, screen = 0;
    long lastDownloadUpdate;
    String WUNDERGROUND_CITY;
    String WUNDERGRROUND_API_KEY;
    String WUNDERGRROUND_LANGUAGE; // as per https://www.wunderground.com/weather/api/d/docs?d=resources/country-to-iso-matching
    String WUNDERGROUND_COUNTRY;
    const String degreeSign = "°C";
    String date = "", oldDate = "", day, temp, WeatherParams, name, usabledata;
    char time_str[11];
    char *dstAbbrev;

    dstRule StartRule = {"CEST", Last, Sun, Mar, 2, 3600}; // Central European Summer Time = UTC/GMT +2 hours
    dstRule EndRule = {"CET", Last, Sun, Oct, 2, 0};       // Central European Time = UTC/GMT +1 hour

    time_t now;
    struct tm *timeinfo;

    void updateData(bool msg);
    void drawTime();
    void drawCurrentWeather();
    void drawForecast();
    void drawForecastDetail(uint16_t x, uint16_t y, uint8_t dayIndex);
    void drawCurrentWeatherDetail();
    void drawLabelValue(uint8_t line, String label, String value);
    void drawForecastTable(uint8_t start);
    const char *getMeteoconIconFromProgmem(String iconText);
    const char *getMiniMeteoconIconFromProgmem(String iconText);
    const String getShortText(String iconText);
    bool GetParams(fs::FS &fs, const char *path);
    String GetTextData(String *source, String strname);

  public:
    void Run();
    WeatherStationClass();
    ~WeatherStationClass();
};
