
// include library, include base class, make path known
// https://github.com/ZinggJM/GxEPD
#include <GxEPD.h>
#include "IMG_0001.h"
#include <GxGDEW075T8/GxGDEW075T8.h>      // 7.5" b/w 640X384

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, 15, 4, 5); 
GxEPD_Class display(io, 5, 16); 

const GFXfont* f = &FreeMonoBold9pt7b;

const GFXfont* f12pt = &FreeMonoBold12pt7b;
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init(115200); // enable diagnostic output on Serial
  Serial.println("setup done");
}

void loop()
{
  showAI_IF2();
  delay(2000);

  showAI_if();
  delay(2000);
  showPizzaApple();

  while(1){ delay(2); }
  delay(2000);

}

void showAI_if(){
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(gImage_IMG_0001, 0, 0, 429, 384, GxEPD_BLACK);
  
  display.setFont(f);
  display.setTextColor(GxEPD_BLACK);

  display.setCursor(445, 50);
  display.println("¿What is AI?");

  display.setCursor(445, 100);
  display.println("General Public:");
    display.setCursor(445, 120);

  display.println("  Killing Machine");
  display.setCursor(445, 180);
  display.println("AI Engineer:");
  display.setCursor(445, 210);
  display.println("  If statement");

  display.update();
}

void showPizzaApple(){
   display.setFont(f12pt);
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(pizzaApple, 0, 0, 640, 260, GxEPD_BLACK);
  display.setCursor(20, 270);
  display.println("What is this?");
    display.setCursor(20, 300);
  display.println("AI: PIZZA!!! ");

  display.setFont(f);
  display.setCursor(300, 360);
  display.println("Scan NFC Tag to learn more");
  display.update();
}

void showAI_IF2(){
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(AI_IF2, 0, 0, 596, 384, GxEPD_BLACK);
  display.update();
}

void showFont(const char name[], const GFXfont* f)
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println(name);
  display.println(" !\"#$%&'()*+,-./");
  display.println("0123456789:;<=>?");
  display.println("@ABCDEFGHIJKLMNO");
  display.println("PQRSTUVWXYZ[\\]^_");
// #if defined(HAS_RED_COLOR)
//   display.setTextColor(GxEPD_RED);
// #endif
  display.println("`abcdefghijklmno");
  display.println("pqrstuvwxyz{|}~ ");
  display.update();
  delay(5000);
}



