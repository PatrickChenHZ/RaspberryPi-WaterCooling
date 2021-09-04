#include "Cool500.h"
#include <dht.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            10
 
#define NUMPIXELS      20

float  RDE1=0;
float  GRE1=0;
float  BLE1=0;
volatile int CPU_LOAD;
volatile int CPU_TEMP;
volatile int RPM;
unsigned long TURN = 0;
dht myDHT_8;
int dht_8_gettemperature() {
  int chk = myDHT_8.read11(8);
  int value = myDHT_8.temperature;
  return value;
}

CoolGuyModule_OLED OLED;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  Serial.begin(9600);
  CPU_LOAD = 0;
  CPU_TEMP = 0;
  RPM = 0;
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
  OLED.OLED_Print(1,1, String("BOOTING"));
  OLED.OLED_Print(2,1, String("PI FAN"));
  OLED.OLED_Print(3,1, String("WATERCOOLER"));
  OLED.OLED_Print(4,1, String("PRO EDITION"));
  delay(5000);
  OLED.OLED_Print(1,1, String("                "));
  OLED.OLED_Print(2,1, String("                "));
  OLED.OLED_Print(3,1, String("                "));
  OLED.OLED_Print(4,1, String("                "));
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
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
  OLED.OLED_Print(3,1, String("WATER TEMP") + String(dht_8_gettemperature()) + String("C"));
  OLED.OLED_Print(4,1, String("FAN SP:") + String(TURN) + String("RPM"));

  GRE1=0;//这里再给初始化一下，以免出错，如果计算没有错的话，没有必要
for(int o=0;o<255;o++){
    RDE1=255;
    GRE1=GRE1+1;
    BLE1=0;
    for(int l=0;l<NUMPIXELS;l++){//这个for循环是一个个去点亮，不懂怎么去一起点亮
    pixels.setPixelColor(l, pixels.Color(RDE1,GRE1,BLE1)); 
    pixels.show(); 
    }
    delay(20);//让渐变慢些
}
  RDE1=255;

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
  OLED.OLED_Print(3,1, String("WATER TEMP") + String(dht_8_gettemperature()) + String("C"));
  OLED.OLED_Print(4,1, String("FAN SP:") + String(TURN) + String("RPM"));
    
for(int p=0;p<255;p++){
    RDE1=RDE1-1;
    GRE1=255;
    BLE1=0;
    for(int l=0;l<NUMPIXELS;l++){
    pixels.setPixelColor(l, pixels.Color(RDE1,GRE1,BLE1)); 
    pixels.show(); 
    }
    delay(20);
}
  BLE1=0;

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
  OLED.OLED_Print(3,1, String("WATER TEMP") + String(dht_8_gettemperature()) + String("C"));
  OLED.OLED_Print(4,1, String("FAN SP:") + String(TURN) + String("RPM"));

for(int k=0;k<255;k++){
    RDE1=0;
    GRE1=255;
    BLE1=BLE1+1;
    for(int l=0;l<NUMPIXELS;l++){
    pixels.setPixelColor(l, pixels.Color(RDE1,GRE1,BLE1)); 
    pixels.show(); 
    }
    delay(20);
}
  GRE1=255;

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
  OLED.OLED_Print(3,1, String("WATER TEMP") + String(dht_8_gettemperature()) + String("C"));
  OLED.OLED_Print(4,1, String("FAN SP:") + String(TURN) + String("RPM"));
  
for(int k=0;k<255;k++){
    RDE1=0;
    GRE1=GRE1-1;
    BLE1=255;
    for(int l=0;l<NUMPIXELS;l++){
    pixels.setPixelColor(l, pixels.Color(RDE1,GRE1,BLE1)); 
    pixels.show(); 
    }
    delay(20);
}
  RDE1=0;

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
  OLED.OLED_Print(3,1, String("WATER TEMP") + String(dht_8_gettemperature()) + String("C"));
  OLED.OLED_Print(4,1, String("FAN SP:") + String(TURN) + String("RPM"));
  
for(int h=0;h<150;h++){
    RDE1=RDE1+1;
    GRE1=0;
    BLE1=255;
    for(int l=0;l<NUMPIXELS;l++){
    pixels.setPixelColor(l, pixels.Color(RDE1,GRE1,BLE1)); 
    pixels.show(); 
    }
    delay(20);
}
}
