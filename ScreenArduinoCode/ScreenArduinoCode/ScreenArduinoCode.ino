#include <TouchScreen.h>
#include <Wire.h>
#include "MCUFRIEND_kbv.h"

MCUFRIEND_kbv tft;

#define SLAVE_ADDRESS 0x12  // I2C slave address (7-bit format)
uint8_t SEND_POWER = 0x1;
uint8_t CASE_KTHETA = 0x2;
uint8_t CASE_KI = 0x3;
uint8_t CASE_K1 = 0x4;
uint8_t CASE_K2 = 0x5;
uint8_t CASE_T_OSCI = 0x6;
uint8_t CASE_SYSTEMORDER = 0x7;

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

//#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

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

// Defining where to press to activate the start button
#define BUTTON_MINW_START_MENU (SCREEN_SIZE_W/2-100)
#define BUTTON_MAXW_START_MENU (SCREEN_SIZE_W/2-100 + 200)
#define BUTTON_MINH_START_MENU (SCREEN_SIZE_H/2-30)
#define BUTTON_MAXH_START_MENU (SCREEN_SIZE_H/2-30 + 60)



#define TS_MINX 100
#define TS_MAXX 920

#define TS_MINY 70
#define TS_MAXY 900

#define MINPRESSURE 10
#define MAXPRESSURE 1000

unsigned long startTime = 0;
unsigned long startTime1 = 0;
const int timeBetweenMsg = 2000;
const int timeBetweenMesure = 1;
bool is_on_menu = false;
bool tab_filtre_is_open = false;
bool tab_control_is_open = false;
int Ktheta = 4;
int Ki = 99;
int K1 = 4;
int K2 = 128;
int T_osci = 8;
int systemOrder = 1;
int send_power = 0;
float curve_cursor = 0;
float sin_value;

const int numPoints = 170; // Nombre de points à tracer (correspondant à la largeur de l'écran)
float sinValues[numPoints]; // Tableau circulaire pour stocker les valeurs de sin
int currentIndex = 0; // Index actuel dans le tableau circulaire


void setup(void) {
  Serial.begin(115200);
  Wire.begin(0x12);  // Join I2C bus as master
  
  Serial.println("I2C Master: Begin transmission...");
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  //tft.reset();
  uint16_t identifier = tft.readID();

  if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  }else {
    Serial.print(F("LCD driver not recognized"));
    identifier=0x9341;
  }

  tft.begin(identifier);

  pinMode(13, OUTPUT);

  set_starting_screen();

/* Initialisation du tableau de valeurs de sin à 0 */
  for (int i = 0; i < numPoints; i++) {
    sinValues[i] = 0;
  }
}


void loop(void) {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();  //stores the length (x), width (y) and compression (z) in the p object.
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  show_curve();

  if (millis() - startTime > timeBetweenMsg){
    Serial.print("is_on_menu: "); Serial.println(is_on_menu);
    Serial.print("tab_filtre_is_open: "); Serial.println(tab_filtre_is_open);
    Serial.print("tab_control_is_open: "); Serial.println(tab_control_is_open);
    Serial.print("systemOrder: "); Serial.println(systemOrder);
    Serial.println("");
    startTime = millis();
  }

  /* L'écran vient d'être pressé */
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
    /*
    Serial.print("X: "); Serial.print(p.x);
    Serial.print(", Y: "); Serial.print(p.y);
    Serial.print(", Z: "); Serial.println(p.z);
    */
    
    if (is_on_menu == false && p.y > BUTTON_MINW_START_MENU && p.y < BUTTON_MAXW_START_MENU && p.x > BUTTON_MINH_START_MENU && p.x < BUTTON_MAXH_START_MENU) {
      Serial.println("Button Start pressed");
      is_on_menu = true;
      
      send_power = 1;
      send_parameters_I2C(SEND_POWER, send_power);

      set_menu_screen();
    }

    /* Retour sur le menu de lancement avec le robot éteint */
    else if (is_on_menu == true && p.y > SCREEN_SIZE_W-35 && p.y < SCREEN_SIZE_W-5 && p.x > 5 && p.x < 35){
      Serial.println("Button End pressed");
      is_on_menu = false;
      tab_control_is_open = false;
      tab_filtre_is_open = false;
      set_starting_screen();
    }

    else if (is_on_menu == true && p.y > 0 && p.y < 100 && p.x > 0 && p.x < 70){
      Serial.println("Filtre Tab pressed");
      tab_filtre_is_open = true;
      tab_control_is_open = false;
      set_filtre_tab();
    }

    else if(is_on_menu == true && p.y > 0 && p.y < 100 && p.x > 70 && p.x < 140){
      Serial.println("Control Tab pressed");
      tab_control_is_open = true;
      tab_filtre_is_open = false;
      set_control_tab();
    }
  
    else if (is_on_menu == true && (tab_control_is_open == true || tab_filtre_is_open == true ) && p.y > 0 && p.y < 100 && p.x > 140 && p.x < 210){
      Serial.println("Display Tab pressed");
      tab_control_is_open = false;
      tab_filtre_is_open = false;

      set_menu_screen();
    }
  }

  /* Réglage des paramètres de l'onglet Filtre */
  if (tab_filtre_is_open == true){
    if (systemOrder != 1 && p.y > 135 && p.y < 175 && p.x > 160 && p.x < 200 ){
      systemOrder = 1;
      send_parameters_I2C(CASE_SYSTEMORDER, systemOrder);
      set_filtre_tab();
      //Serial.println("changing order 2 -> 1");
    }
    else if(systemOrder != 2 && p.y > 245 && p.y < 285 && p.x > 160 && p.x < 200){
      systemOrder = 2;
      send_parameters_I2C(CASE_SYSTEMORDER, systemOrder);
      set_filtre_tab();
      //Serial.println("changing order 1 -> 2");
    }

    else if(p.y > 285 && p.y < 315 && p.x > 40 && p.x < 80){
      Ktheta++;
      send_parameters_I2C(CASE_KTHETA, Ktheta);
      set_filtre_tab();
    }
    else if(p.y > 105 && p.y <135 && p.x > 40 && p.x < 80){
      Ktheta--;
      send_parameters_I2C(CASE_KTHETA, Ktheta);
      set_filtre_tab();
    }
    else if(p.y > 285 && p.y < 315 && p.x > 90 && p.x < 130){
      Ki++;
      send_parameters_I2C(CASE_KI, Ki);
      set_filtre_tab();
    }
    else if(p.y > 105 && p.y <135 && p.x > 90 && p.x < 130){
      Ki--;
      send_parameters_I2C(CASE_KI, Ki);
      set_filtre_tab();
    }

  }

  /* Réglage des paramètres de l'onglet Control */
  if (tab_control_is_open == true){
    if(p.y > 285 && p.y < 315 && p.x > 40 && p.x < 80){
      K1++;
      send_parameters_I2C(CASE_K1, K1);
      set_control_tab();
    }
    else if(p.y > 105 && p.y <135 && p.x > 40 && p.x < 80){
      K1--;
      send_parameters_I2C(CASE_K1, K1);
      set_control_tab();
    }
    else if(p.y > 285 && p.y < 315 && p.x > 90 && p.x < 130){
      K2++;
      send_parameters_I2C(CASE_K2, K2);
      set_control_tab();
    }
    else if(p.y > 105 && p.y <135 && p.x > 90 && p.x < 130){
      K2--;
      send_parameters_I2C(CASE_K2, K2);
      set_control_tab();
    }
    else if(p.y > 285 && p.y < 315 && p.x > 140 && p.x < 180){
      T_osci++;
      send_parameters_I2C(CASE_T_OSCI, T_osci);
      set_control_tab();
    }
    else if(p.y > 105 && p.y <135 && p.x > 140 && p.x < 180){
      T_osci--;
      send_parameters_I2C(CASE_T_OSCI, T_osci);
      set_control_tab();
    }
  }
}

void set_starting_screen(){
  Serial.println("Setting Starting Screen");

  send_power = 0;

  tft.fillScreen(BLACK);
  tft.setRotation(1);
  tft.fillRect(SCREEN_SIZE_W/2-100, SCREEN_SIZE_H/2-30, 200, 60, RED);
  tft.setCursor(SCREEN_SIZE_W/2-40,SCREEN_SIZE_H/2-10);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println("START");

  send_parameters_I2C(SEND_POWER, send_power);
}

void set_menu_screen(){
  Serial.println("Setting Menu Screen");

  tft.fillScreen(WHITE);
  tft.setRotation(1);

  tft.fillRect(0, SCREEN_SIZE_H-30, 320, 30, RED);
  tft.fillRect(0, 0, 100, SCREEN_SIZE_H, RED);
  tft.drawFastHLine(0, SCREEN_SIZE_H-30, SCREEN_SIZE_W, BLACK);
  tft.drawFastVLine(100, 0, SCREEN_SIZE_H, BLACK);
  tft.drawFastHLine(0, 70, 100, BLACK);
  tft.drawFastHLine(0, 140, 100, BLACK);

  //Bande bas
  /*
  tft.setCursor(15, SCREEN_SIZE_H-15);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("angle theta");
  */

  tft.setCursor(115, SCREEN_SIZE_H-15);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("PWM_D us");

  tft.setCursor(215, SCREEN_SIZE_H-15);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("PWM_G us");

  //Bande verticale
  tft.setCursor(10, 35);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.println("Filtre");

  tft.setCursor(10, 105);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.println("Control");

  tft.setCursor(10, 175);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.println("Display");  

  //Courbe
  tft.drawFastHLine(120, 190, 170, BLACK);
  tft.drawFastHLine(120, 191, 170, BLACK);
  tft.drawFastVLine(120, 30, 160, BLACK);
  tft.drawFastVLine(119, 30, 160, BLACK);

  set_off_button();
}

void set_off_button(){
  tft.fillCircle(SCREEN_SIZE_W-20, 20, 15, RED);
  tft.drawCircle(SCREEN_SIZE_W-20, 20, 7, WHITE);
  tft.drawFastVLine(SCREEN_SIZE_W-20, 10, 10, WHITE);
}

void set_filtre_tab(){
  tft.fillRect(100, 0, SCREEN_SIZE_W, SCREEN_SIZE_H-30, WHITE);

  tft.fillRect(140, 40, 140, 40, RED);
  tft.fillTriangle(285, 40, 285, 80, 315, 60, RED);
  tft.fillTriangle(135, 40, 135, 80, 105, 60,RED);
  tft.setCursor(150, 60);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("Ktheta = "); tft. println(Ktheta);

  tft.fillRect(140, 90, 140, 40, RED);
  tft.fillTriangle(285, 90, 285, 130, 315, 110, RED);
  tft.fillTriangle(135, 90, 135, 130, 105, 110,RED);
  tft.setCursor(150, 110);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("Ki = "); tft. println(Ki);

  tft.setCursor(180, 140);
  tft.setTextColor(BLACK);  tft.setTextSize(2);
  tft.println("ORDER:");

  if (systemOrder == 1){
    tft.fillCircle(155, 180, 20, RED);
    tft.setCursor(150, 175);
    tft.setTextColor(BLACK);  tft.setTextSize(2);
    tft.print("1");
    
    tft.drawCircle(SCREEN_SIZE_W-55, 180, 20, RED);
    tft.drawCircle(SCREEN_SIZE_W-55, 180, 19, RED);
    tft.setCursor(SCREEN_SIZE_W-60, 175);
    tft.setTextColor(BLACK);  tft.setTextSize(2);
    tft.print("2");
  }

  else if (systemOrder ==2){
    tft.drawCircle(155, 180, 20, RED);
    tft.drawCircle(155, 180, 19, RED);
    tft.setCursor(150, 175);
    tft.setTextColor(BLACK);  tft.setTextSize(2);
    tft.print("1");
    
    tft.fillCircle(SCREEN_SIZE_W-55, 180, 20, RED);
    tft.setCursor(SCREEN_SIZE_W-60, 175);
    tft.setTextColor(BLACK);  tft.setTextSize(2);
    tft.print("2");
  }

  set_off_button();
}

void set_control_tab(){
  tft.fillRect(100, 0, SCREEN_SIZE_W, SCREEN_SIZE_H-30, WHITE);

  tft.fillRect(140, 40, SCREEN_SIZE_W-180, 40, RED);
  tft.fillTriangle(285, 40, 285, 80, 315, 60, RED);
  tft.fillTriangle(135, 40, 135, 80, 105, 60,RED);
  tft.setCursor(150, 60);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("K1 = "); tft. println(K1);

  tft.fillRect(140, 90, SCREEN_SIZE_W-180, 40, RED);
  tft.fillTriangle(285, 90, 285, 130, 315, 110, RED);
  tft.fillTriangle(135, 90, 135, 130, 105, 110,RED);
  tft.setCursor(150, 110);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("K2 = "); tft. println(K2);

  tft.fillRect(140, 140, SCREEN_SIZE_W-180, 40, RED);
  tft.fillTriangle(285, 140, 285, 180, 315, 160, RED);
  tft.fillTriangle(135, 140, 135, 180, 105, 160,RED);
  tft.setCursor(150, 160);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("T_osci = "); tft. println(T_osci);

  set_off_button();
}

void send_parameters_I2C(uint8_t adress_parameter, uint8_t parameter){
  Wire.beginTransmission(SLAVE_ADDRESS);  // Begin I2C transmission to the slave
  Wire.write(&adress_parameter, sizeof(adress_parameter));
  Wire.write(&parameter, sizeof(parameter));
  uint8_t error = Wire.endTransmission(true); // End transmission and check for errors
  
  if (error == 0) {
    Serial.print("Parameter sent: ");
    Serial.print("0x");
    Serial.print(adress_parameter, HEX);
    Serial.println(" ");

    Serial.print("Value: ");
    Serial.print(parameter, DEC);
    Serial.println(" ");
  }
  else {
    Serial.print("Error: ");
    Serial.println(error);
  }

}

void show_curve(){
  if (millis() - startTime1 > timeBetweenMesure){
    curve_cursor ++;
    sin_value = sin(curve_cursor/10);

    sinValues[currentIndex] = sin_value;
    
    if (is_on_menu == true){
      tft.fillRect(0, SCREEN_SIZE_H-15, 119, 30, RED);
      tft.setCursor(15, SCREEN_SIZE_H-15);
      tft.setTextColor(WHITE);  tft.setTextSize(1);
      tft.println(sinValues[currentIndex]);
    }

    currentIndex = (currentIndex + 1) % numPoints;

    startTime1 = millis();
  }

  if (is_on_menu == true && tab_control_is_open == false && tab_filtre_is_open == false){
    tft.fillRect(121, 30, 170, 160, WHITE);
    for (int i = 0; i < numPoints; i++){
      int x = 120 + i; // Décalage de l'axe x
      int y = 190 - map(sinValues[(currentIndex + i) % numPoints] * 80 + 80, 0, 160, 0, 160); // Décalage de l'axe y
      
      tft.drawPixel(x, y, BLACK);
      tft.drawPixel(x+1, y, BLACK);
      tft.drawPixel(x-1, y, BLACK);
      tft.drawPixel(x, y+1, BLACK);
      tft.drawPixel(x, y-1, BLACK);
    }
  }
}