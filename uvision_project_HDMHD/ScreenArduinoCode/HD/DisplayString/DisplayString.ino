#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <Wire.h>
#include "MCUFRIEND_kbv.h"

MCUFRIEND_kbv tft;

#define SLAVE_ADDRESS 0x12  // I2C slave address (7-bit format)
uint8_t SEND_POWER_ON= 0x1;

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define SCREEN_SIZE_H 240
#define SCREEN_SIZE_W 320

// Computed myself to correspond to the button does not work
#define BUTTON_MINW SCREEN_SIZE_W/2-100
#define BUTTON_MAXW SCREEN_SIZE_W/2-100 + 200

#define BUTTON_MINH SCREEN_SIZE_H/2-30
#define BUTTON_MAXH SCREEN_SIZE_H/2-30 + 60

#define TS_MINX 100
#define TS_MAXX 920

#define TS_MINY 70
#define TS_MAXY 900

#define MINPRESSURE 10
#define MAXPRESSURE 1000

unsigned long startTime = 0;
unsigned long startTime1 = 0;
unsigned long startTime2 = 0;
const int timeBetweenMsg = 1000;


void setup(void) {
  Wire.begin();  // Join I2C bus as master
  Serial.begin(115200);
  
  Serial.println("I2C Master: Begin transmission...");
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
  if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  }else {
    Serial.print(F("LCD driver not recognized"));
    identifier=0x9341;
   
  }

  tft.begin(identifier);

  set_starting_screen();

  pinMode(13, OUTPUT);
}


void loop(void) {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();  //stores the length (x), width (y) and compression (z) in the p object.
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    
    if (millis() - startTime1 > timeBetweenMsg){
      Serial.println("Screen pressed");
      startTime1 = millis();
    }

    // Serial.println(p.y);
    // Serial.println(p.x); 
    // Serial.print(tft.width());
    // Serial.println(tft.height());
    
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    //p.x = (tft.width()-map(p.x, TS_MINX, TS_MAXX, 0, tft.width()));
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
    //Serial.print("px:H");Serial.println(p.x);
    //Serial.print("py:W");Serial.println(p.y);
    
    if (p.y > BUTTON_MINW && p.y < BUTTON_MAXW && p.x > BUTTON_MINH+50 && p.x < BUTTON_MAXH+50) {

      // Serial.print("BUTTON_MINH ");Serial.println(BUTTON_MINH);
      // Serial.print("BUTTON_MAXH ");Serial.println(BUTTON_MAXH);
      Serial.println("Button pressed");

      Wire.beginTransmission(SLAVE_ADDRESS);  // Begin I2C transmission to the slave
      Wire.write(&SEND_POWER_ON, sizeof(SEND_POWER_ON));
      uint8_t error = Wire.endTransmission(); // End transmission and check for errors
      if (error == 0) {
       Serial.print("Data sent: ");
       Serial.print("0x");
        Serial.print(SEND_POWER_ON, HEX);
        Serial.println(" ");
      }
      else {
        Serial.print("Error: ");
        Serial.println(error);
      }

      set_menu_screen();
      
    }
    
    //delay(1000);  // Wait for 1 second before sending the next data
  }
  
  //set_starting_screen();
  else{
    if (millis() - startTime > timeBetweenMsg){
      Serial.println("Screen not pressed");
      startTime = millis();
    }
  }
}

void set_starting_screen(){
  Serial.println("Setting Starting Screen");

  tft.fillScreen(BLACK);
  tft.setRotation(1);
  tft.fillRect(SCREEN_SIZE_W/2-100, SCREEN_SIZE_H/2-30, 200, 60, RED);
  tft.setCursor(SCREEN_SIZE_W/2-40,SCREEN_SIZE_H/2-10);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println("START");
}

void set_menu_screen(){
  Serial.println("Setting Menu Screen");

  tft.fillScreen(BLACK);
  tft.setRotation(1);

  //Affichage
  tft.fillRect(SCREEN_SIZE_W/2-130, SCREEN_SIZE_H/2+30, 260, 60, RED);
  tft.setCursor(SCREEN_SIZE_W/2-60, SCREEN_SIZE_H/2+50);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println("DISPLAY"); 

  //Filtre
  tft.fillRect(SCREEN_SIZE_W/2-130, SCREEN_SIZE_H/2-100, 115, 100, RED);
  tft.setCursor(SCREEN_SIZE_W/2-115, SCREEN_SIZE_H/2-80);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.println("FILTRE");

  //Controle
  tft.fillRect(SCREEN_SIZE_W/2+15, SCREEN_SIZE_H/2-100, 115, 100, RED);
  tft.setCursor(SCREEN_SIZE_W/2+45, SCREEN_SIZE_H/2-80);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.println("CONTROL");
}

