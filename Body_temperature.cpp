#include "Body_temperature.h"
const int MAX_LIGHTNESS = 80;
void Body_temp::Show(const int body_pins[], int max_lightness)
{
    //    Serial.println(body_temp);
    float head_temp, chest_temp, limbs_temp;
    float mid_left = min_air_temp + (max_air_temp - min_air_temp) * 3.0 / 8,
          mid_right = min_air_temp + (max_air_temp - min_air_temp) * 5.0 / 8;

    if (body_temp < mid_left)
    {
        head_temp = -(mid_left - body_temp) / (mid_left - min_air_temp) * 0.6;
        chest_temp = head_temp / 0.6 * 0.3;
        limbs_temp = head_temp / 0.6;
    }
    else if (body_temp < mid_right)
    {
        head_temp = 0;
        chest_temp = (body_temp - mid_left) * 1.0 / (max_air_temp - mid_left);
        limbs_temp = -(mid_right - body_temp) / (mid_right - min_air_temp);
    }
    else
    {
        head_temp = (body_temp - mid_right) * 1.0 / (max_air_temp - mid_right);
        chest_temp = (body_temp - mid_left) * 1.0 / (max_air_temp - mid_left);
        limbs_temp = head_temp * 0.6;
    }

    analogWrite(body_pins[0], max(0, max_lightness * head_temp));
    analogWrite(body_pins[1], max(0, -max_lightness * head_temp));

    analogWrite(body_pins[2], max(0, max_lightness * chest_temp));
    analogWrite(body_pins[3], max(0, -max_lightness * chest_temp));

    analogWrite(body_pins[4], max(0, max_lightness * limbs_temp));
    analogWrite(body_pins[5], max(0, -max_lightness * limbs_temp));
}
void Body_temp::BreatheShow(const int body_pins[], int max_lightness)
{
    float head_temp, chest_temp, limbs_temp;
    float mid_left = min_air_temp + (max_air_temp - min_air_temp) * 3.0 / 8,
          mid_right = min_air_temp + (max_air_temp - min_air_temp) * 5.0 / 8;

    if (body_temp < mid_left)
    {
        head_temp = -(mid_left - body_temp) / (mid_left - min_air_temp) * 0.6;
        chest_temp = head_temp / 0.6 * 0.3;
        limbs_temp = head_temp / 0.6;
    }
    else if (body_temp < mid_right)
    {
        head_temp = 0;
        chest_temp = (body_temp - mid_left) * 1.0 / (max_air_temp - mid_left);
        limbs_temp = -(mid_right - body_temp) / (mid_right - min_air_temp);
    }
    else
    {
        head_temp = (body_temp - mid_right) * 1.0 / (max_air_temp - mid_right);
        chest_temp = (body_temp - mid_left) * 1.0 / (max_air_temp - mid_left);
        limbs_temp = head_temp * 0.6;
    }

    float w = 2 * PI * breathe_rate / 60; //角速度

    analogWrite(body_pins[0], Wave(max_lightness * head_temp, w));
    analogWrite(body_pins[1], Wave(0.5*-max_lightness * head_temp, w));

    analogWrite(body_pins[2], Wave(max_lightness * chest_temp, w));
    analogWrite(body_pins[3], Wave(0.5*-max_lightness * chest_temp, w));

    analogWrite(body_pins[4], Wave(max_lightness * limbs_temp, w));
    analogWrite(body_pins[5], Wave(0.5*-max_lightness * limbs_temp, w));
}
void Body_temp::Change(float air_temp, const int body_pins[], int max_lightness)
{
    int short_time = 2; //短时间内改变体温
    float mid_temp = (max_air_temp + min_air_temp) / 2.0;
    float d_temp = (air_temp - body_temp) / (short_time * FPS),
          d_heart_rate = (65 + abs(body_temp - mid_temp) * 1.0 / (max_air_temp - mid_temp) * 10 - heart_rate) / (short_time * FPS),
          d_breathe_rate = (map(air_temp,min_air_temp,max_air_temp,20,40) - breathe_rate) / (short_time * FPS);
    for (int i = 0; i < short_time * FPS; i++)
    {
        body_temp += d_temp;
        heart_rate += d_heart_rate;
//        breathe_rate += d_breathe_rate;
        breathe_rate=map(body_temp,min_air_temp,max_air_temp,20,40);
       
//        Serial.println(body_temp);        
        BreatheShow(body_pins, max_lightness);
        delay(1000 / FPS);
//        Serial.flush();
    }
}
Body_temp Body_temp::Heartbeat()
{
    Body_temp body(*this);                   //拷贝
    float w = 2 * PI * body.heart_rate / 60; //角速度
    body.body_temp += sin(w * millis() / 1000) * 5.0;
    //    Serial.println(body.body_temp);
    body.body_temp = min(max_air_temp, body.body_temp); //防止温度溢出
    body.body_temp = max(min_air_temp, body.body_temp);
    return body;
}
float Body_temp::min_air_temp = -20; //设置最低气温
float Body_temp::max_air_temp = 50;  //设置最高气温

float Wave(float x, float w)
{
    float y = x * 0.5 * (sin(w * millis() / 1000) + 1);
    y = min(MAX_LIGHTNESS, y);
    y = max(0, y);
    return y;
}
