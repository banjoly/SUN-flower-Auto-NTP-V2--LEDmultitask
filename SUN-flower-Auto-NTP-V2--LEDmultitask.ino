//WIFI
#include<WiFi.h>
#define WIFI_STA_NAME "SAHOUSE2020  2G"  //SAHOUSE2020  2G
#define WIFI_STA_PASS "11012709"

//matrix LED8x16
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16minimatrix matrix;
// ตัวแปรสำหรับควบคุมการเลื่อน
int scrollX;                    // ตำแหน่ง X ปัจจุบัน
int startX = 16;               // ตำแหน่งเริ่มต้น
int endX;                      // ตำแหน่งสุดท้าย
unsigned long previousMillis = 0;  // เวลาที่ update ครั้งล่าสุด
const long scrollInterval = 35;    // ความเร็วในการเลื่อน (ms)
String scrollText;             // ข้อความที่จะแสดง
bool isScrolling = false;      // สถานะการเลื่อน



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
int sec;
String strTime="99:99:99";


int timeX1;  


  

//
void setup(){

 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 printLocalTime();
 // pinMode(17,OUTPUT);
 // digitalWrite(17,1);
  // Debug console
  Serial.begin(115200);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
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
  matrix.setBrightness(8);
}

void loop(){
  timeX1=millis();
  strTime=String(atoi(timeHour))+":"+String(atoi(timeMinute))+":"+String(atoi(timeSecond));
  hr=atoi(timeHour);
  minx=atoi(timeMinute);
//matrixLED
  String text = strTime;
  // คำนวณระยะที่ต้องเลื่อน
  int textWidth = text.length() * 6; // แต่ละตัวอักษรกว้าง 6 pixels
  int startX = 16; // ความกว้างจอ
  int endX = -(textWidth); // จุดสิ้นสุดคือ -ความยาวข้อความ
//LED bug Coz delay
/*  for (int x = startX; x >= endX; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(text);
    matrix.writeDisplay();
    delay(35);
  }
*/
startScroll("Hello World!");
updateScroll();
static unsigned long messageChangeMillis = 0;
  if (millis() - messageChangeMillis >= 5000) {  // ทุก 5 วินาที
    messageChangeMillis = millis();
    
    // สลับระหว่างสองข้อความ
    static bool toggleMessage = false;
    if (toggleMessage) {
      startScroll("Message 1");
    } else {
      startScroll("Message 2");
    }
    toggleMessage = !toggleMessage;
  }

 /* Blynk.run();
  Blynk.virtualWrite(V1,strTime);
  if(digitalRead(17)==1){
    Blynk.virtualWrite(V3,"OFF");
  }
  if(digitalRead(17)==0){
    Blynk.virtualWrite(V3,"ON");
  }
  
  if((digitalRead(4)==1)||((hr>=9&&hr<=18)&&(minx>=5&&minx<=10))||((hr>=9&&hr<=18)&&(minx>=35&&minx<=40))){
    digitalWrite(17,0);
  }else digitalWrite(17,1);
  if(digitalRead(0)==1){
    digitalWrite(17,1);
  }*/
  if(timeX1%1000==0){
    printLocalTime();
  }
  //Watering every 30 minutes from 07.00-18.00
  if((hr>=7&&hr<=18)&&(minx==30)){
    digitalWrite(25,0);
    tone(13,100,200);
    tone(13,300,30);
    delay(2000);
    digitalWrite(25,1);
    noTone(13);
    delay(500);
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
  delay(100); 
}

// ฟังก์ชันเริ่มการเลื่อนข้อความใหม่
void startScroll(String text) {
  scrollText = text;
  scrollX = startX;
  endX = -(text.length() * 6);  // คำนวณจุดสิ้นสุด
  isScrolling = true;
}

// ฟังก์ชันอัพเดทการเลื่อน
void updateScroll() {
  unsigned long currentMillis = millis();
  
  if (isScrolling && (currentMillis - previousMillis >= scrollInterval)) {
    previousMillis = currentMillis;
    
    // วาดข้อความที่ตำแหน่งปัจจุบัน
    matrix.clear();
    matrix.setCursor(scrollX, 0);
    matrix.print(scrollText);
    matrix.writeDisplay();
    
    // เลื่อนตำแหน่ง
    scrollX--;
    
    // ตรวจสอบว่าถึงจุดสิ้นสุดหรือยัง
    if (scrollX < endX) {
      // เริ่มใหม่
      scrollX = startX;
      // หรือถ้าต้องการหยุด ใช้:
      // isScrolling = false;
    }
  }
}

