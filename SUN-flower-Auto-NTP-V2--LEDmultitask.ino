//WIFI
#include<WiFi.h>
#define WIFI_STA_NAME "SAHOUSE2020  2G"  //SAHOUSE2020  2G
#define WIFI_STA_PASS "11012709"
//matrix LED8x16
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16minimatrix matrix;

//NTPClient time
#include "time.h"
const char* ntpServer = "th.pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;
// Variables to save time
char timeHour[3];
char timeMinute[3];
char timeSecond[3];
//NTP


int hr;
int minx;
int secx;
int sec;
int minTrig=0;
String strTime="99:99:99";
int timeX1;
int flag0=0;
int flagSound=1;


//
void setup(){
  pinMode(25,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(25,1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //printLocalTime();
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //matrix LED
  matrix.begin(0x70);
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextColor(LED_ON);
  matrix.setTextWrap(false);
  matrix.setBrightness(1);
}

void loop(){

//matrixLED
  String text = strTime;
  // คำนวณระยะที่ต้องเลื่อน
  int textWidth = text.length() * 6; // แต่ละตัวอักษรกว้าง 6 pixels
  int startX = 16; // ความกว้างจอ
  int endX = -(textWidth); // จุดสิ้นสุดคือ -ความยาวข้อความ
  
  for (int x = startX; x >= endX; x--) {
   
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(text);
    matrix.writeDisplay();
      timeX1=millis();
      strTime=String(atoi(timeHour))+":"+String(atoi(timeMinute));//+":"+String(atoi(timeSecond)
      hr=atoi(timeHour);
      minx=atoi(timeMinute);
      secx=atoi(timeSecond);
      

      if(hr<10){
        strTime="0"+String(atoi(timeHour))+":"+String(atoi(timeMinute));//+":"+String(atoi(timeSecond)
      }
      if(minx<10){
        strTime=String(atoi(timeHour))+":"+"0"+String(atoi(timeMinute));//+":"+String(atoi(timeSecond)
      }
        //if(timeX1%1000==0){
        printLocalTime();
        //}
    delay(3);
    
  }
  if((secx==59||secx==0)&&flagSound==1){
    flagSound=0;
    tone(13,4000,500);
  }else flagSound=1;
  

  //Watering every 30 minutes from 07.00-18.00
  if((hr==7||hr==12||hr==17)&&(minx==30&&(secx>=0&&secx<=4))){ //07.00-18.00 every haft of hour 60 minutes 5 second 
    digitalWrite(25,0);
    tone(13,1000,200);
    tone(13,4000,30);
  }else {
    digitalWrite(25,1);
    noTone(13);
    }
    if((hr==8)&&(minx==0&&(secx>=20&&secx<=23))){ //07.00-18.00 every haft of hour 60 minutes 5 second
    tone(13,500,500);
    tone(13,4000,500);  
    tone(13,500,500);
    tone(13,4000,500);  
    tone(13,500,500);
    tone(13,4000,500);  
    tone(13,500,500);
    tone(13,4000,500);  
    tone(13,500,500);
    tone(13,4000,500);  
    delay(5000);  // รอ 5 วินาที
    Serial.println("Restarting...");
    ESP.restart();  // รีสตาร์ท ESP32
  }
  
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    //delay(2000);
    return;
  }
  strftime(timeHour,3, "%H", &timeinfo);  //%H:%M:%S
  Serial.print(timeHour);
  Serial.print(":");
  strftime(timeMinute,3, "%M", &timeinfo);
  Serial.print(timeMinute);
  Serial.print(":");
  strftime(timeSecond,3, "%S", &timeinfo);
  Serial.println(timeSecond);
  delay(35);
}
