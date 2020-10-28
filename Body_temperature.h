#pragma once
#include "Arduino.h"
#include "Show.h"

class Body_temp
{

    float body_temp,                         //体温
        heart_rate,                          //心率
        breathe_rate;                        //呼吸速率
    static float max_air_temp, min_air_temp; //最高气温，最低气温
public:
    Body_temp(float air_temp = 0) : body_temp(air_temp), heart_rate(65), breathe_rate(30)
    {
        body_temp = max(min_air_temp, body_temp);
        body_temp = min(max_air_temp, body_temp);
    }
    void Show(const int pins[], int max_lightness);                   //显示体温
    void BreatheShow(const int pins[], int max_lightness);            //显示呼吸
    void Change(float air_temp, const int pins[], int max_lightness); //改变体温
    Body_temp Heartbeat();                                            //波动体温
};

float Wave(float x, float w);
