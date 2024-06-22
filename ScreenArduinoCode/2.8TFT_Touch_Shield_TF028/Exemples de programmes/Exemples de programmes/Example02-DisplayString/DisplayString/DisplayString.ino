// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

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

#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

void setup(void) {
  Serial.begin(9600);
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

  tft.setRotation(1);


 
}

void loop(void) {
   tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  
  tft.setTextColor(RED);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.println(01234.56789);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.println();
  tft.setTextColor(GREEN); tft.setTextSize(2);
   tft.println("Hello World!");
  tft.println(01234.56789);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.println();
  
  tft.setTextColor(BLUE);    tft.setTextSize(3);
   tft.println("Hello World!");
  tft.println(01234.56789);
  tft.println(0xDEADBEEF, HEX);
  
  tft.setTextColor(WHITE);    tft.setTextSize(4);
   tft.println("Hello!");
   tft.setTextColor(YELLOW);    tft.setTextSize(5);
   tft.println("Hello!");
   tft.setTextColor(RED);    tft.setTextSize(6);
   tft.println("Hello!");
  tft.println();
  tft.println();

  delay(1000);delay(1000);delay(1000);delay(1000);delay(1000);
}
