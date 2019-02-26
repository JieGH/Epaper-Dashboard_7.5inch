
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <stdlib.h>
#include <stdio.h>
#include <GxEPD.h>
#include <GxGDEW075T8/GxGDEW075T8.cpp>      // 7.5" b/w  640x384            

//#include GxEPD_BitmapExamples
#include "images.h"

//FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>



const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Gateshead,GB,pt&APPID=";
const String key = "/* your key */";
String payload;

const String endpoint_metoffice = "http://datapoint.metoffice.gov.uk/public/data/val/wxfcs/all/json/352793?res=3hourly&key=/* your key */";

const String newsapiOrg = "http://newsapi.org/v2/top-headlines?sources=the-guardian-uk&apiKey=/* your key */";
String news_payload;

String g_hours;
String g_minutes;
int g_minutes_int;
String g_week_day;

String g_news0;
String g_news1;
String g_news2;
String g_date;


int twT;
int tmwT;
int awT;

String today_feelsLike;
String tomorrow_feelsLike;
String after_feelsLike;

String today_humi;
String tomorrow_humi;
String after_humi;

String today_pop;
String tomorrow_pop;
String after_pop;


const char *ssid     = " your ssid";
const char *password = "your password";

String g_ssid = ssid;
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

GxIO_Class io(SPI, /*CS=D15*/ 15, /*DC=D4*/ 4, /*RST=D5*/ 5);
GxEPD_Class ePaper(io, 5, 16 /*RST=D5*/ /*BUSY=D16*/);

//FUNCTIONS

void get_time_from_ntp() {
  timeClient.update();

  Serial.print(timeClient.getHours() - 1);
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(daysOfTheWeek[timeClient.getDay()]);
  g_hours = timeClient.getHours() - 1;
  g_minutes = timeClient.getMinutes();
  g_minutes_int = timeClient.getMinutes();

  g_week_day = daysOfTheWeek[timeClient.getDay()];
}

void htte_get_news() {
  HTTPClient http;
  http.begin(newsapiOrg); //Specify the URL
  int httpCode = http.GET();  //Make the request
  if (httpCode > 0) { //Check for the returning code
    news_payload = http.getString();
    Serial.println("Http get news api, ok.");
  }
  else {
    Serial.println("Error on news HTTPs request");
  }
  http.end(); //Free the resources
}
void print_news() {
  const size_t capacity = JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 10 * JSON_OBJECT_SIZE(8) + 11000;
  DynamicJsonBuffer jsonBuffer(capacity);
  String json = news_payload;
  JsonObject& root = jsonBuffer.parseObject(json);
  const char* status = root["status"]; // "ok"
  int totalResults = root["totalResults"]; // 10
  JsonArray& articles = root["articles"];
  JsonObject& articles0 = articles[0];
  const char* articles0_title = articles0["title"]; // "Mother of three-year-old is first person convicted of FGM in UK"
  const char* articles0_publishedAt = articles0["publishedAt"]; // "2019-02-02T14:36:19Z"
  JsonObject& articles1 = articles[1];
  const char* articles1_title = articles1["title"]; // "Senior HMRC official sacked after sexual misconduct claims"
  JsonObject& articles2 = articles[2];
  const char* articles2_title = articles2["title"]; // "UK weather: heavy snow closes schools and disrupts travel"

  Serial.println(articles0_title);
  Serial.println(articles1_title);
  Serial.println(articles2_title);
  g_news0 = articles0_title;
  g_news1 = articles1_title;
  g_news2 = articles2_title;
  g_date = articles0_publishedAt;
}

void connect_wifi_set_up_time() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
}

void get_print_metoffice() {
  String json;
  HTTPClient http;
  http.begin(endpoint_metoffice); //Specify the URL
  int httpCode = http.GET();  //Make the request
  if (httpCode > 0) { //Check for the returning code
    json  = http.getString();
    Serial.println("Http get metoffice weather api, ok.");
  }
  else {
    Serial.println("Error on metoffice weather HTTP request");

  }
  http.end(); //Free the resources

  const size_t capacity = JSON_ARRAY_SIZE(5) + JSON_ARRAY_SIZE(6) + 4 * JSON_ARRAY_SIZE(8) + JSON_ARRAY_SIZE(10) + 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 16 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(8) + 38 * JSON_OBJECT_SIZE(11) + 2950;
  DynamicJsonBuffer jsonBuffer(capacity + 10000);
  JsonObject& root = jsonBuffer.parseObject(json);
  JsonArray& SiteRep_Wx_Param = root["SiteRep"]["Wx"]["Param"];
  JsonObject& SiteRep_Wx_Param0 = SiteRep_Wx_Param[0];
  JsonObject& SiteRep_Wx_Param1 = SiteRep_Wx_Param[1];
  JsonObject& SiteRep_Wx_Param2 = SiteRep_Wx_Param[2];
  JsonObject& SiteRep_Wx_Param3 = SiteRep_Wx_Param[3];
  JsonObject& SiteRep_DV = root["SiteRep"]["DV"];
  JsonObject& SiteRep_DV_Location = SiteRep_DV["Location"];
  JsonArray& SiteRep_DV_Location_Period = SiteRep_DV_Location["Period"];
  JsonObject& SiteRep_DV_Location_Period0 = SiteRep_DV_Location_Period[0];
  JsonArray& SiteRep_DV_Location_Period0_Rep = SiteRep_DV_Location_Period0["Rep"];
  JsonObject& SiteRep_DV_Location_Period0_Rep0 = SiteRep_DV_Location_Period0_Rep[0];
  const char* SiteRep_DV_Location_Period0_Rep0_F = SiteRep_DV_Location_Period0_Rep0["F"]; // "-7" feels like
  const char* SiteRep_DV_Location_Period0_Rep0_H = SiteRep_DV_Location_Period0_Rep0["H"]; // "76" relative humidi
  const char* SiteRep_DV_Location_Period0_Rep0_Pp = SiteRep_DV_Location_Period0_Rep0["Pp"]; // "1" prob of rain
  const char* SiteRep_DV_Location_Period0_Rep0_T = SiteRep_DV_Location_Period0_Rep0["T"]; // "-5" real temp
  const char* SiteRep_DV_Location_Period0_Rep0_W = SiteRep_DV_Location_Period0_Rep0["W"]; // "2" weather type

  twT = atoi(SiteRep_DV_Location_Period0_Rep0_W);
  today_feelsLike = SiteRep_DV_Location_Period0_Rep0_F;
  today_humi = SiteRep_DV_Location_Period0_Rep0_H;
  today_pop = SiteRep_DV_Location_Period0_Rep0_Pp;
  Serial.println(twT);

  JsonObject& SiteRep_DV_Location_Period1 = SiteRep_DV_Location_Period[1];

  JsonArray& SiteRep_DV_Location_Period1_Rep = SiteRep_DV_Location_Period1["Rep"];

  JsonObject& SiteRep_DV_Location_Period1_Rep4 = SiteRep_DV_Location_Period1_Rep[4];
  const char* SiteRep_DV_Location_Period1_Rep4_F = SiteRep_DV_Location_Period1_Rep4["F"]; // "1"
  const char* SiteRep_DV_Location_Period1_Rep4_H = SiteRep_DV_Location_Period1_Rep4["H"]; // "89"
  const char* SiteRep_DV_Location_Period1_Rep4_Pp = SiteRep_DV_Location_Period1_Rep4["Pp"]; // "1"
  const char* SiteRep_DV_Location_Period1_Rep4_T = SiteRep_DV_Location_Period1_Rep4["T"]; // "6"
  const char* SiteRep_DV_Location_Period1_Rep4_W = SiteRep_DV_Location_Period1_Rep4["W"]; // "2"
  tmwT = atoi(SiteRep_DV_Location_Period1_Rep4_W);
  tomorrow_feelsLike = SiteRep_DV_Location_Period1_Rep4_F;
  tomorrow_humi = SiteRep_DV_Location_Period1_Rep4_H;
  tomorrow_pop = SiteRep_DV_Location_Period1_Rep4_Pp;


  JsonObject& SiteRep_DV_Location_Period2 = SiteRep_DV_Location_Period[2];
  JsonArray& SiteRep_DV_Location_Period2_Rep = SiteRep_DV_Location_Period2["Rep"];
  JsonObject& SiteRep_DV_Location_Period2_Rep4 = SiteRep_DV_Location_Period2_Rep[4];
  const char* SiteRep_DV_Location_Period2_Rep4_F = SiteRep_DV_Location_Period2_Rep4["F"]; // "-3"
  const char* SiteRep_DV_Location_Period2_Rep4_H = SiteRep_DV_Location_Period2_Rep4["H"]; // "95"
  const char* SiteRep_DV_Location_Period2_Rep4_Pp = SiteRep_DV_Location_Period2_Rep4["Pp"]; // "15"
  const char* SiteRep_DV_Location_Period2_Rep4_T = SiteRep_DV_Location_Period2_Rep4["T"]; // "0"
  const char* SiteRep_DV_Location_Period2_Rep4_W = SiteRep_DV_Location_Period2_Rep4["W"]; // "5"
  awT = atoi(SiteRep_DV_Location_Period2_Rep4_W);
  after_feelsLike = SiteRep_DV_Location_Period2_Rep4_F;
  after_humi = SiteRep_DV_Location_Period2_Rep4_H;
  after_pop = SiteRep_DV_Location_Period2_Rep4_Pp;
  Serial.println("metoffice");
}
void updateDisplay() {
  Serial.println("Updating Display");
  ePaper.init(115200); // enable diagnostic output on Serial
  Serial.println("Init Display");
  ePaper.drawPaged(epaperUpdate);
}

void epaperUpdate() {
  ePaper.eraseDisplay();
  ePaper.drawBitmap(gImage_guardian, 200, 0, 30, 30, GxEPD_BLACK);
  ePaper.setTextColor(GxEPD_BLACK);

  const GFXfont* f = &FreeMonoBold9pt7b;
  f = &FreeMonoBold18pt7b;
  ePaper.setFont(f);
  ePaper.setCursor(0, 23);
  ePaper.println("Headlines");
  f = &FreeMonoBold12pt7b;
  ePaper.setFont(f);
  ePaper.setCursor(20, 50);
  ePaper.println(g_news0);
  ePaper.setCursor(0, 50);
  ePaper.println("- ");

  ePaper.setCursor(20, 105);
  ePaper.println(g_news1);
  ePaper.setCursor(0, 105);
  ePaper.println("- ");

  ePaper.setCursor(20, 160);
  ePaper.println(g_news2);
  ePaper.setCursor(0, 160);
  ePaper.println("- ");


  f = &FreeMonoBold18pt7b;
  ePaper.setFont(f);
  ePaper.setCursor(0, 245);
  ePaper.println("Weather");
  ePaper.drawBitmap(gImage_metpofficelogo, 155, 225, 30, 30, GxEPD_BLACK);

  //g_week_day
  f = &FreeMonoBold12pt7b;
  ePaper.setFont(f);
  ePaper.setCursor(522, 310);
  ePaper.println(g_week_day);
  f = &FreeMonoBold9pt7b;
  ePaper.setFont(f);
  ePaper.setCursor(522, 340);
  ePaper.println(g_date);
  //gImage_updateAT
  ePaper.drawBitmap(gImage_updateAT, 522, 355, 20, 20, GxEPD_BLACK);
  ePaper.setCursor(552, 370);
  ePaper.println( g_hours + ":" + g_minutes);
  // A blank area
  ePaper.drawBitmap(gImage_blank, 0, 305, 150, 150, GxEPD_BLACK);


  f = &FreeMonoBold12pt7b;
  ePaper.setFont(f);
  ePaper.setCursor(30, 280);
  ePaper.println("Today");
  ePaper.setCursor(180, 280);
  ePaper.println("Tomorrow");
  ePaper.setCursor(365, 280);
  ePaper.println("Day after");

  //  Serial.println(twT);
  switch (twT) {
    case 0: ePaper.drawBitmap(gImage_0, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 1: ePaper.drawBitmap(gImage_1, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 2: ePaper.drawBitmap(gImage_2, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 3: ePaper.drawBitmap(gImage_3, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 4: ePaper.drawBitmap(gImage_4, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 5: ePaper.drawBitmap(gImage_5, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 6: ePaper.drawBitmap(gImage_6, -10, 305, 80, 40, GxEPD_BLACK); break;
    case 7: ePaper.drawBitmap(gImage_7, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 8: ePaper.drawBitmap(gImage_8, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 9: ePaper.drawBitmap(gImage_9, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 10: ePaper.drawBitmap(gImage_10, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 11: ePaper.drawBitmap(gImage_11, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 12: ePaper.drawBitmap(gImage_12, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 13: ePaper.drawBitmap(gImage_13, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 14: ePaper.drawBitmap(gImage_14, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 15: ePaper.drawBitmap(gImage_15, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 16: ePaper.drawBitmap(gImage_16, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 17: ePaper.drawBitmap(gImage_17, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 18: ePaper.drawBitmap(gImage_18, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 19: ePaper.drawBitmap(gImage_19, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 20: ePaper.drawBitmap(gImage_20, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 21: ePaper.drawBitmap(gImage_21, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 22: ePaper.drawBitmap(gImage_22, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 23: ePaper.drawBitmap(gImage_23, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 24: ePaper.drawBitmap(gImage_24, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 25: ePaper.drawBitmap(gImage_25, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 26: ePaper.drawBitmap(gImage_26, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 27: ePaper.drawBitmap(gImage_27, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 28: ePaper.drawBitmap(gImage_28, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 29: ePaper.drawBitmap(gImage_29, -10, 305, 80, 80, GxEPD_BLACK); break;
    case 30: ePaper.drawBitmap(gImage_30, -10, 305, 80, 80, GxEPD_BLACK); break;
    default : ePaper.drawBitmap(gImage_NA, -10, 305, 80, 80, GxEPD_BLACK);
  }
  switch (tmwT) {
    case 0: ePaper.drawBitmap(gImage_0, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 1: ePaper.drawBitmap(gImage_1, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 2: ePaper.drawBitmap(gImage_2, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 3: ePaper.drawBitmap(gImage_3, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 4: ePaper.drawBitmap(gImage_4, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 5: ePaper.drawBitmap(gImage_5, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 6: ePaper.drawBitmap(gImage_6, 165, 305, 80, 40, GxEPD_BLACK); break;
    case 7: ePaper.drawBitmap(gImage_7, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 8: ePaper.drawBitmap(gImage_8, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 9: ePaper.drawBitmap(gImage_9, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 10: ePaper.drawBitmap(gImage_10, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 11: ePaper.drawBitmap(gImage_11, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 12: ePaper.drawBitmap(gImage_12, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 13: ePaper.drawBitmap(gImage_13, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 14: ePaper.drawBitmap(gImage_14, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 15: ePaper.drawBitmap(gImage_15, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 16: ePaper.drawBitmap(gImage_16, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 17: ePaper.drawBitmap(gImage_17, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 18: ePaper.drawBitmap(gImage_18, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 19: ePaper.drawBitmap(gImage_19, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 20: ePaper.drawBitmap(gImage_20, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 21: ePaper.drawBitmap(gImage_21, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 22: ePaper.drawBitmap(gImage_22, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 23: ePaper.drawBitmap(gImage_23, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 24: ePaper.drawBitmap(gImage_24, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 25: ePaper.drawBitmap(gImage_25, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 26: ePaper.drawBitmap(gImage_26, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 27: ePaper.drawBitmap(gImage_27, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 28: ePaper.drawBitmap(gImage_28, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 29: ePaper.drawBitmap(gImage_29, 165, 305, 80, 80, GxEPD_BLACK); break;
    case 30: ePaper.drawBitmap(gImage_30, 165, 305, 80, 80, GxEPD_BLACK); break;
    default : ePaper.drawBitmap(gImage_NA, 165, 305, 80, 80, GxEPD_BLACK);
  }
  switch (awT) {
    case 0: ePaper.drawBitmap(gImage_0, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 1: ePaper.drawBitmap(gImage_1, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 2: ePaper.drawBitmap(gImage_2, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 3: ePaper.drawBitmap(gImage_3, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 4: ePaper.drawBitmap(gImage_4, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 5: ePaper.drawBitmap(gImage_5, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 6: ePaper.drawBitmap(gImage_6, 345, 305, 80, 40, GxEPD_BLACK); break;
    case 7: ePaper.drawBitmap(gImage_7, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 8: ePaper.drawBitmap(gImage_8, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 9: ePaper.drawBitmap(gImage_9, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 10: ePaper.drawBitmap(gImage_10, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 11: ePaper.drawBitmap(gImage_11, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 12: ePaper.drawBitmap(gImage_12, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 13: ePaper.drawBitmap(gImage_13, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 14: ePaper.drawBitmap(gImage_14, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 15: ePaper.drawBitmap(gImage_15, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 16: ePaper.drawBitmap(gImage_16, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 17: ePaper.drawBitmap(gImage_17, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 18: ePaper.drawBitmap(gImage_18, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 19: ePaper.drawBitmap(gImage_19, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 20: ePaper.drawBitmap(gImage_20, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 21: ePaper.drawBitmap(gImage_21, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 22: ePaper.drawBitmap(gImage_22, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 23: ePaper.drawBitmap(gImage_23, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 24: ePaper.drawBitmap(gImage_24, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 25: ePaper.drawBitmap(gImage_25, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 26: ePaper.drawBitmap(gImage_26, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 27: ePaper.drawBitmap(gImage_27, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 28: ePaper.drawBitmap(gImage_28, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 29: ePaper.drawBitmap(gImage_29, 345, 305, 80, 80, GxEPD_BLACK); break;
    case 30: ePaper.drawBitmap(gImage_30, 345, 305, 80, 80, GxEPD_BLACK); break;
    default : ePaper.drawBitmap(gImage_NA, 345, 305, 80, 80, GxEPD_BLACK);
  }
  f = &FreeMonoBold9pt7b;
  ePaper.setFont(f);
  ePaper.setCursor(65, 310);
  ePaper.println(today_feelsLike + "C");
  ePaper.setCursor(65, 340);
  ePaper.println(today_humi + "%");
  ePaper.setCursor(65, 370);
  ePaper.println(today_pop + "%Rain");

  // tomorrow weather
  //  ePaper.drawBitmap(gImage_buffer, 165, 305, 80, 80, GxEPD_BLACK);
  ePaper.setCursor(240, 310);
  ePaper.println(tomorrow_feelsLike + "C");
  ePaper.setCursor(240, 340);
  ePaper.println(tomorrow_humi + "%");
  ePaper.setCursor(240, 370);
  ePaper.println(tomorrow_pop + "%Rain");

  // tomorrow after weather
  //  ePaper.drawBitmap(gImage_0, 345, 305, 80, 80, GxEPD_BLACK);
  ePaper.setCursor(420, 310);
  ePaper.println(after_feelsLike + "C");
  ePaper.setCursor(420, 340);
  ePaper.println(after_humi + "%");
  ePaper.setCursor(420, 370);
  ePaper.println(after_pop + "%Rain");


}


void setup() {
  Serial.begin(115200);//debug

}



void loop() {
  Serial.println("hi.");

  connect_wifi_set_up_time();

  while (WiFi.status() == WL_CONNECTED) {
    get_time_from_ntp();
    get_print_metoffice();
    htte_get_news();
    print_news();
    //    display_epd();
    updateDisplay();
    delay(12000);
  }
}
