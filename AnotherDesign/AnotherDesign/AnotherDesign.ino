
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
    // display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(gImage_IMG_0001, 0, 0, 429, 384, GxEPD_BLACK);
  
  const GFXfont* f = &FreeMonoBold9pt7b;
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
  while(1){ delay(2); }
  delay(2000);
// #if !defined(__AVR)
//   //drawCornerTest();
//   showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
//   showFont("FreeMonoBold12pt7b", &FreeMonoBold12pt7b);
//   //showFont("FreeMonoBold18pt7b", &FreeMonoBold18pt7b);
//   //showFont("FreeMonoBold24pt7b", &FreeMonoBold24pt7b);
// #else
  display.drawCornerTest();
  delay(2000);
  display.drawPaged(showFontCallback);
// #endif
  delay(10000);
}

void showBitmapExample()
{
  display.drawExampleBitmap(gImage_IMG_0001, sizeof(gImage_IMG_0001));
  delay(2000);
  // display.drawExampleBitmap(BitmapExample2, sizeof(BitmapExample2));
  // delay(5000);
  display.fillScreen(GxEPD_WHITE);
  display.drawExampleBitmap(gImage_IMG_0001, 0, 0, 429, 384, GxEPD_BLACK);
  // display.drawExampleBitmap(gImage_IMG_0001, 0, 0, 384, 429, GxEPD_BLACK);
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

void showFontCallback()
{
  const char* name = "FreeMonoBold9pt7b";
  const GFXfont* f = &FreeMonoBold9pt7b;
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
}

void drawCornerTest()
{
  display.drawCornerTest();
  delay(5000);
  uint8_t rotation = display.getRotation();
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.fillScreen(GxEPD_WHITE);
    display.fillRect(0, 0, 8, 8, GxEPD_BLACK);
    display.fillRect(display.width() - 18, 0, 16, 16, GxEPD_BLACK);
    display.fillRect(display.width() - 25, display.height() - 25, 24, 24, GxEPD_BLACK);
    display.fillRect(0, display.height() - 33, 32, 32, GxEPD_BLACK);
    display.update();
    delay(5000);
  }
  display.setRotation(rotation); // restore
}
