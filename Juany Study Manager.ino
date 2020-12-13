#include "U8glib.h"
#include <stdio.h>
#include <DS1302.h>
#define rst  2
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); 

#define u8g_logo_width 128//定义开机画面宽度
#define u8g_logo_height 64//定义开机画面宽高度
//------------------
String t1 = "study";
String t2 = "manager";
String t3 = "verson.01";
//------开机画面----------
void vologo(unsigned int x, unsigned int y)
{
  u8g.firstPage();
  do
  {
  u8g.setFont(u8g_font_6x10);//设置字体
  u8g.setFont(u8g_font_osb18);
  u8g.drawStr(10,30,"Welcome!");
  }
  while( u8g.nextPage() );
  delay(2000);
  }

void volcdlogo(unsigned int x, unsigned int y)
{
  u8g.firstPage();
  do
  {
  u8g.setFont(u8g_font_6x10);//设置字体
  u8g.setPrintPos(0, 10);//字体出现位置，输出信息。（u8g2库中，该函数没有关键字，怀疑已经删除该函数）
  u8g.print(t1);
  u8g.setPrintPos(20, 20);
  u8g.print(t2);
  u8g.setPrintPos(40, 30);
  u8g.print(t3); 
  u8g.setFont(u8g_font_osb18);
  u8g.drawStr(0,56,"Initializing.");
  }
  while( u8g.nextPage() );
  delay(4000);//延时5秒到下一个画面
}

char oled_buf2[50];// 用于在OLED上显示
char oled_buf1[50];
namespace {

const int kCePin   = 5;  // 复位引脚
const int kIoPin   = 6;  // 数据引脚
const int kSclkPin = 7;  // 时钟引脚
DS1302 rtc(kCePin, kIoPin, kSclkPin);

void printTime() {
  // 从模块获得当前时间
  Time t = rtc.time();
  snprintf(oled_buf1, sizeof(oled_buf2), "%02d/%02d/%02d",
          t.yr, t.mon, t.date);
  snprintf(oled_buf2, sizeof(oled_buf2), "%02d:%02d:%02d",
          t.hr, t.min, t.sec);
  }

}  // namespace

void init_oled(){
 if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on
u8g.setFont(u8g_font_ncenB14);//设置字体 
}

const int TrigPin = 2; //发送超声波
const int EchoPin = 3; //接收超声波
int Sensor = 8;//接收传感器的信号
float cm;

void p_oled(){
  for(int i=0;i<=240;i++)
  {
    analogWrite(10,i);
    delay(1);
    }
 
  for(int i=240;i>=0;i--)
  {
    analogWrite(10,i);
    delay(1);
    }
}

void setup(void)
{
Serial.begin(9600);
volcdlogo(0, 28);//开始画面X,Y
vologo(10,10);
pinMode(TrigPin, OUTPUT);
pinMode(EchoPin, INPUT);
pinMode(13,OUTPUT);
pinMode(10,OUTPUT);
pinMode(Sensor, INPUT); // 红外设置为输入
pinMode(12,OUTPUT);
 init_oled();
   rtc.writeProtect(false);
   rtc.halt(false);
 Time t(2020, 12, 13, 15, 17, 00, Time::kSaturday);
   rtc.time(t);
}

void loop(){ 
  int val = digitalRead(Sensor); 
 u8g.firstPage();  
  do {
   u8g.drawFrame(5,5,118,54);
   u8g.drawStr(13,25,oled_buf1);
   u8g.drawStr(25,50,oled_buf2);
  } while( u8g.nextPage() );
  delay(500);
  printTime();
  p_oled();
 
digitalWrite(TrigPin, LOW);  //超声波
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin, LOW);
cm = pulseIn(EchoPin, HIGH) / 58.0;   // 检测脉冲宽度，并计算出距离
  Serial.println(cm);
  if (cm <=50)
{
        digitalWrite(13,HIGH);  
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_ncenB14);
    u8g.drawStr(22,33,"Too Close!");
   } while( u8g.nextPage() );

  delay(300);
         digitalWrite(13,LOW);
  delay(250);
}
 if (cm >=66&&cm<=100&&val==1){
     digitalWrite(12,HIGH);  
      u8g.firstPage();  
  do {
    u8g.drawFrame(5,15,10,10);
    u8g.drawFrame(25,15,10,10);
    u8g.drawLine(10,35 , 19, 38);
    u8g.drawLine(20,38 , 29, 35);
    u8g.setFont(u8g_font_ncenB14);
    u8g.drawStr(45,20,"study");
    u8g.drawStr(40,43,"FIGHT!");
    }while( u8g.nextPage() ); 
    delay(800);
  }
  else
  digitalWrite(13,LOW);  
  digitalWrite(12,LOW);
}
