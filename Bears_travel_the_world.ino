#include "Body_temperature.h"
const int MAX_LIGHTNESS = 80;                 //最大发光值
const int LED_pins[6] = {3, 5, 6, 9, 10, 11}; //LED引脚设置
Body_temp bear(-20);                          //建立熊的身体
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while (Serial.available() > 0) //串口读入
  {
    delay(1000);
    int temp = Serial.read();
    int air_temp = (temp - 2 - '0') * 10;

    bear.Change(air_temp, LED_pins, MAX_LIGHTNESS); //体温变化

    while (Serial.available() > 0)
      Serial.read();
  }

  /*   for(int i=-20;i<50;i+=5){
    bear.Change(i,LED_pins, MAX_LIGHTNESS);
  }*/
  /*  for(;;){    
    bear.BreatheShow(LED_pins, MAX_LIGHTNESS); //让熊呼吸
    delay(1000 / FPS);
  }*/
}
