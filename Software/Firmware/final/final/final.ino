#include "Cool500.h"
#include <OneWire.h>
#include <DallasTemperature.h>

volatile int CPU_LOAD;
volatile int CPU_TEMP;
volatile int RPM;
unsigned long TURN = 0;


CoolGuyModule_OLED OLED;
OneWire oneWire_8(8);
DallasTemperature sensors_8(&oneWire_8);
DeviceAddress insideThermometer;
float ds18b20_8_getTemp(int w) {
  sensors_8.requestTemperatures();
  if(w==0) {
return sensors_8.getTempC(insideThermometer);
}
  else {
return sensors_8.getTempF(insideThermometer);
}
}

void setup(){
  Serial.begin(9600);
  CPU_LOAD = 0;
  CPU_TEMP = 0;
  RPM = 0;
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
  sensors_8.getAddress(insideThermometer, 0);
  sensors_8.setResolution(insideThermometer, 9);
  OLED.OLED_Print(1,1, String("POWER ON"));
  OLED.OLED_Print(2,1, String("PI FAN"));
  OLED.OLED_Print(3,1, String("WATERCOOLER"));
  OLED.OLED_Print(4,1, String("PRO EDITION"));
  delay(5000);
  OLED.OLED_Print(1,1, String("                "));
  OLED.OLED_Print(2,1, String("                "));
  OLED.OLED_Print(3,1, String("                "));
  OLED.OLED_Print(4,1, String("                "));
}

void loop(){
  if (Serial.available() > 0) {
    CoolGuyModule_StringCmp::SetString(Serial.readString());
    CPU_LOAD = CoolGuyModule_StringCmp::Content_StringInt_ToInt("T", 1);
    CPU_TEMP = CoolGuyModule_StringCmp::Content_StringInt_ToInt("T", 0);
    RPM = 254 - (CPU_LOAD + CPU_TEMP) * 1.27;
    analogWrite(5, RPM );
    delay(6000);
   }
   TURN = 0;
  if (RPM < 230) {                    // 当风扇开始转动时，计算转速
    for (int i = 0; i < 5; i++) {           //高低电平变化5个周期（如果要精确的话，可以多取几次），取总计时间（微秒）
      TURN += pulseIn(2, HIGH);
      TURN += pulseIn(2, LOW);
    }
  } else {
    TURN = 4294967295;          //如果风扇未转动，设置为unsigned long类型的最大值，让计算后风扇转速接近为0
  }
  TURN = TURN / 5;          //高低电平变化1次用时
  TURN = 1000000 / TURN;    // 1秒内可变化几次（即转化为风扇转速频率Hz）
  TURN = TURN * 60;         //1分钟内可变化几次（即转换为风扇转速 转/分钟）
   

  OLED.OLED_Print(1,1, String("CPU LOAD:") + String(CPU_LOAD) + String("%"));
  OLED.OLED_Print(2,1, String("CPU TEMP:") + String(CPU_TEMP) + String("C"));
  OLED.OLED_Print(3,1, String("WATER TEMP") + String(ds18b20_8_getTemp(0)) + String("C"));
  OLED.OLED_Print(4,1, String("FAN SP:") + String(TURN) + String("RPM"));

}
