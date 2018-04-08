#include "../../apps.h"

#define M_SIZE 1.3333
#define GREY 0x5AEB
#define LOOP_PERIOD 35 // Display updates every 35 ms

float ltx = 0;              // Saved x coord of bottom of needle
int ndlx = 120, ndly = 120; // Saved x & y coords
long updateTime = 0;        // time for next update

float old_analog = -999;  // Value last displayed
float old_digital = -999; // Value last displayed

float value[6] = {0, 0, 0, 0, 0, 0};
float old_value[6] = {-1, -1, -1, -1, -1, -1};
int d = 0;
float val, needleval;
float textv;
float maxval = 3.3;

// A custom map function that work's with float
float remap(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void plotNeedle(float value, byte ms_delay)
{
    M5.Lcd.setTextColor(BLACK, WHITE);

    if (value < -10)
        value = -10; // Limit value to emulate needle end stops
    if (value > 110)
        value = 110;

    // Move the needle util new value reached
    while (!((int)value == (int)old_analog))
    {
        if (old_analog < value)
            old_analog++;
        else
            old_analog--;

        if (ms_delay == 0)
            old_analog = value; // Update immediately id delay is 0

        float sdeg = map(old_analog, -10, 110, -150, -30); // Map value to angle
        // Calcualte tip of needle coords
        float sx = cos(sdeg * 0.0174532925);
        float sy = sin(sdeg * 0.0174532925);

        // Calculate x delta of needle start (does not start at pivot point)
        float tx = tan((sdeg + 90) * 0.0174532925);

        // Erase old needle image
        M5.Lcd.drawLine(120 + 20 * ltx - 1, 140 - 20, ndlx - 1, ndly, WHITE);
        M5.Lcd.drawLine(120 + 20 * ltx, 140 - 20, ndlx, ndly, WHITE);
        M5.Lcd.drawLine(120 + 20 * ltx + 1, 140 - 20, ndlx + 1, ndly, WHITE);

        // Re-plot text under needle
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Lcd.drawCentreString(String(remap(val, 0, 4095, 0, maxval)) + " V", 120, 70, 4); // // Comment out to avoid font 4

        // Store new needle end coords for next erase
        ltx = tx;
        ndlx = sx * 98 + 120;
        ndly = sy * 98 + 140;

        // Draw the needle in the new postion, magenta makes needle a bit bolder
        // draws 3 lines to thicken needle
        M5.Lcd.drawLine(120 + 20 * ltx - 1, 140 - 20, ndlx - 1, ndly, RED);
        M5.Lcd.drawLine(120 + 20 * ltx, 140 - 20, ndlx, ndly, MAGENTA);
        M5.Lcd.drawLine(120 + 20 * ltx + 1, 140 - 20, ndlx + 1, ndly, RED);

        // Slow needle down slightly as it approaches new postion
        if (abs(old_analog - value) < 10)
            ms_delay += ms_delay / 5;

        // Wait before next update
        delay(ms_delay);
    }
}

// #########################################################################
//  Draw a linear meter on the screen
// #########################################################################
void plotLinear(char *label, int x, int y)
{
    int w = 36;
    M5.Lcd.drawRect(x, y, w, 155, GREY);
    M5.Lcd.fillRect(x + 2, y + 19, w - 3, 155 - 38, WHITE);
    M5.Lcd.setTextColor(CYAN, BLACK);
    M5.Lcd.drawCentreString(label, x + w / 2, y + 2, 2);

    for (int i = 0; i < 110; i += 10)
    {
        M5.Lcd.drawFastHLine(x + 20, y + 27 + i, 6, BLACK);
    }

    for (int i = 0; i < 110; i += 50)
    {
        M5.Lcd.drawFastHLine(x + 20, y + 27 + i, 9, BLACK);
    }

    M5.Lcd.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 - 5, RED);
    M5.Lcd.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 + 5, RED);

    M5.Lcd.drawCentreString("---", x + w / 2, y + 155 - 18, 2);
}

// #########################################################################
//  Adjust 6 linear meter pointer positions
// #########################################################################
void plotPointer(void)
{
    int dy = 187;
    byte pw = 16;

    M5.Lcd.setTextColor(GREEN, BLACK);

    // Move the 6 pointers one pixel towards new value
    for (int i = 0; i < 6; i++)
    {
        //char buf[8];
        //dtostrf(remap(value[i], 0, 100, 0, maxval), 4, 0, buf);
        M5.Lcd.drawRightString(String(remap(value[i], 0, 99, 0, maxval)), i * 40 + 36 - 5, 187 - 27 + 155 - 18, 2);

        int dx = 3 + 40 * i;
        if (value[i] < 0)
            value[i] = 0; // Limit value to emulate needle end stops
        if (value[i] > 100)
            value[i] = 100;

        while (!((int)value[i] == (int)old_value[i]))
        {
            dy = 187 + 100 - old_value[i];
            if (old_value[i] > value[i])
            {
                M5.Lcd.drawLine(dx, dy - 5, dx + pw, dy, WHITE);
                old_value[i]--;
                M5.Lcd.drawLine(dx, dy + 6, dx + pw, dy + 1, RED);
            }
            else
            {
                M5.Lcd.drawLine(dx, dy + 5, dx + pw, dy, WHITE);
                old_value[i]++;
                M5.Lcd.drawLine(dx, dy - 6, dx + pw, dy - 1, RED);
            }
        }
    }
}

// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
void analogMeter()
{
    // Meter outline
    M5.Lcd.fillRect(0, 0, 239, 126, GREY);
    M5.Lcd.fillRect(5, 3, 230, 119, WHITE);

    M5.Lcd.setTextColor(BLACK); // Text colour

    // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
    for (int i = -50; i < 51; i += 5)
    {
        // Long scale tick length
        int tl = 15;

        // Coodinates of tick to draw
        float sx = cos((i - 90) * 0.0174532925);
        float sy = sin((i - 90) * 0.0174532925);
        int x0 = sx * (100 + tl) + 120;
        int y0 = sy * (100 + tl) + 140;
        int x1 = sx * 100 + 120;
        int y1 = sy * 100 + 140;

        // Coordinates of next tick for zone fill
        float sx2 = cos((i + 5 - 90) * 0.0174532925);
        float sy2 = sin((i + 5 - 90) * 0.0174532925);
        int x2 = sx2 * (100 + tl) + 120;
        int y2 = sy2 * (100 + tl) + 140;
        int x3 = sx2 * 100 + 120;
        int y3 = sy2 * 100 + 140;

        //Yellow zone limits
        if (i >= -50 && i < 0)
        {
            M5.Lcd.fillTriangle(x0, y0, x1, y1, x2, y2, YELLOW);
            M5.Lcd.fillTriangle(x1, y1, x2, y2, x3, y3, YELLOW);
        }

        // Green zone limits
        if (i >= 0 && i < 25)
        {
            M5.Lcd.fillTriangle(x0, y0, x1, y1, x2, y2, GREEN);
            M5.Lcd.fillTriangle(x1, y1, x2, y2, x3, y3, GREEN);
        }

        // Orange zone limits
        if (i >= 25 && i < 50)
        {
            M5.Lcd.fillTriangle(x0, y0, x1, y1, x2, y2, ORANGE);
            M5.Lcd.fillTriangle(x1, y1, x2, y2, x3, y3, ORANGE);
        }

        // Short scale tick length
        if (i % 25 != 0)
            tl = 8;

        // Recalculate coords incase tick lenght changed
        x0 = sx * (100 + tl) + 120;
        y0 = sy * (100 + tl) + 140;
        x1 = sx * 100 + 120;
        y1 = sy * 100 + 140;

        // Draw tick
        M5.Lcd.drawLine(x0, y0, x1, y1, BLACK);

        // Check if labels should be drawn, with position tweaks
        if (i % 25 == 0)
        {
            // Calculate label positions
            x0 = sx * (100 + tl + 10) + 120;
            y0 = sy * (100 + tl + 10) + 140;
            switch (i / 25)
            {
            case -2:
                M5.Lcd.drawCentreString("0", x0, y0 - 12, 2);
                break;
            case -1:
                M5.Lcd.drawCentreString(String(maxval / 4), x0, y0 - 9, 2);
                break;
            case 0:
                M5.Lcd.drawCentreString(String(maxval / 2), x0, y0 - 6, 2);
                break;
            case 1:
                M5.Lcd.drawCentreString(String((maxval / 4) * 3), x0, y0 - 9, 2);
                break;
            case 2:
                M5.Lcd.drawCentreString(String(maxval), x0, y0 - 12, 2);
                break;
            }
        }

        // Now draw the arc of the scale
        sx = cos((i + 5 - 90) * 0.0174532925);
        sy = sin((i + 5 - 90) * 0.0174532925);
        x0 = sx * 100 + 120;
        y0 = sy * 100 + 140;
        // Draw scale arc, don't draw the last part
        if (i < 50)
        M5.Lcd.drawLine(x0, y0, x1, y1, BLACK);
    }

    M5.Lcd.drawString("V", 5 + 230 - 40, 119 - 20, 2); // Units at bottom right
                                                       // M5.Lcd.drawCentreString("V", 120, 70, 4);          // Comment out to avoid font 4
    M5.Lcd.drawRect(5, 3, 230, 119, BLACK);            // Draw bezel line

    plotNeedle(0, 0); // Put meter needle at 0
}

void meters_setup()
{
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE,BLACK);
    M5.Lcd.drawCentreString("Input pin: 35", 160, 70, 4);
    M5.Lcd.drawCentreString("Max Voltage: 3.3v", 160, 120, 4);
    M5.Lcd.drawCentreString("Lower Linear meters show voltage history", 160, 170, 2);
    M5.Lcd.drawCentreString("Press Key A to Exit", 160, 190, 2);
    delay(5000);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setRotation(3);
    analogMeter(); // Draw analogue meter
    pinMode(A7, INPUT);

    // Draw 6 linear meters
    byte d = 40;
    plotLinear("V 0", 0, 160);
    plotLinear("V-1", 1 * d, 160);
    plotLinear("V-2", 2 * d, 160);
    plotLinear("V-3", 3 * d, 160);
    plotLinear("V-4", 4 * d, 160);
    plotLinear("V-5", 5 * d, 160);
}

void meters_run()
{
    if (updateTime <= millis())
    {
        updateTime = millis() + LOOP_PERIOD;
        d += 4;
        if (d >= 360)
        {
            d = 0;
        }

        val = analogRead(A7);
        needleval = map(val, 0, 4096, 0, 100); // Test with value form Analogue 0

        if (needleval > 5)
        {
            // if the new value is different, record it to linear meters
            if ((needleval - value[0]) > 10 || (value[0] - needleval) > 10)
            {
                for (size_t i = 5; i > 0; i--)
                {
                    value[i] = value[i - 1];
                }
            }
        }
        value[0] = needleval;

        plotPointer();
        plotNeedle(value[0], 0);
        M5.update();
    }
}