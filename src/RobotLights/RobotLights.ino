
// knihovny pro LCD přes I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
 
#define LED_FRONT_PIN A2
#define NR_FRONT_LED 8 // front strip has 8 LEDs
#define LED_REAR_PIN A3
#define NR_REAR_LED 2 // only 2 rear LEDs

#define BLINK_PERIOD 600 // 600 ms
#define BLINK_ONTIME 300 // 300 ms
 
Adafruit_NeoPixel stripFront = Adafruit_NeoPixel(NR_FRONT_LED, LED_FRONT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripRear = Adafruit_NeoPixel(NR_REAR_LED, LED_REAR_PIN, NEO_GRB + NEO_KHZ800); 

// LCD displej pres I2C
// nastavení adresy I2C (0x27 v mém případě),
// a dále počtu znaků a řádků LCD, zde 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint32_t RED, GREEN, BLUE, BRIGHT_WHITE, WHITE, BLACK, ORANGE;

#define OFF 0x0000
#define BLINK_RIGHT 0x0101
#define BLINK_LEFT 0x0202
#define BRAKES 0x0404
#define LIGHTS_1 0x1000
#define LIGHTS_2 0x2000
#define LIGHTS_3 0x4000
#define POLICE 0x8000

void setupLED() {
  stripFront.begin();
  stripRear.begin();
  
  RED = stripFront.Color(0xFF, 0, 0);
  GREEN = stripFront.Color(0, 0xFF, 0);
  BLUE = stripFront.Color(0, 0, 0xFF);
  WHITE = stripFront.Color(0x4F, 0x4F, 0x4F);
  BRIGHT_WHITE = stripFront.Color(0xFF, 0xFF, 0xFF);
  BLACK = 0;
  ORANGE = stripRear.Color(20, 4, 0);
}

void setupLCD() {
  // LCD číslování řádků a sloupců je od 0, poslední znak má pozici [15, 1]
  // inicializace LCD
  lcd.begin();
  // zapnutí podsvícení displeje
  lcd.backlight();
  // vytisknutí hlášky na první řádek
  lcd.print("-= J&J  Robot =-");
  // nastavení kurzoru na první znak, druhý řádek
  lcd.setCursor ( 0, 1 );
  lcd.print("Uptime: ");
  lcd.setCursor ( 15, 1);
  lcd.print("♠");
}

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  setupLED();
  setupLCD();
  delay(500);
}

void updateLED(int code) {
  String frontOut = "--------"; 
  String rearOut = "--------";

  if (code & LIGHTS_1) {
    frontOut.setCharAt(3, 'O');
    frontOut.setCharAt(4, 'O');
  }
  
  if (code & LIGHTS_2) {
    frontOut.setCharAt(2, 'O');
    frontOut.setCharAt(5, 'O');
  }

  if (code & LIGHTS_3) {
    frontOut.setCharAt(1, 'O');
    frontOut.setCharAt(6, 'O');
  }

  if (code & BLINK_RIGHT) {
    frontOut.setCharAt(7, '*');
  }

  if (code & BLINK_LEFT) {
    frontOut.setCharAt(0, '*');
  }

  if (code & BRAKES) {
    rearOut.setCharAt(3, '=');
    rearOut.setCharAt(4, '=');
  }
  Serial.println(frontOut);
  Serial.println(rearOut);
  Serial.println();
}

void updateLCD() {
  // nastavení kurzoru na devátý znak, druhý řádek
  lcd.setCursor(8, 1);
  // vytisknutí počtu sekund od začátku programu
  lcd.print(millis() / 1000);
  lcd.print(" s");
}

static void chase(Adafruit_NeoPixel &strip, uint32_t c) {
  strip.clear();
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25); // FIXME no delay allowed
  }
}

static void light(Adafruit_NeoPixel &strip, uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

static void lightTwoColors(Adafruit_NeoPixel &strip, uint32_t c1, uint32_t c2) {
  int half = strip.numPixels() / 2;
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    if (i < half) {
      strip.setPixelColor(i, c1);
    }
    else {
      strip.setPixelColor(i, c2);
    }
  }
  strip.show();
}

static void lightFourColors(Adafruit_NeoPixel &strip, uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) { // cyklus i bude mít hodnoty 0,1,2,3,4,5,6,7
    if (i < 2) {
      strip.setPixelColor(i, c1);
    }
    else if (i < 4) {
      strip.setPixelColor(i, c2);
    }
    else if (i < 6) {
      strip.setPixelColor(i, c3);
    }
    else {
      strip.setPixelColor(i, c4);
    }
  }
  strip.show();
}

static void lightThreeColors(Adafruit_NeoPixel &strip, uint32_t c1, uint32_t c2, uint32_t c3) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) { // cyklus i bude mít hodnoty 0,1,2,3,4,5,6,7
    if (i == 2 || i == 5) {
      continue;
    }
    if (i < 2) {
      strip.setPixelColor(i, c1);
    }
    else if (i < 5) {
      strip.setPixelColor(i, c2);
    }
    else {
      strip.setPixelColor(i, c3);
    }
  }
  strip.show();
}

static uint32_t blinkColor(uint32_t color1, uint32_t color2) {
  bool lightOn = (millis() % BLINK_PERIOD) < BLINK_ONTIME;
  if (lightOn) {
    return color1;
  }
  else {
    return color2;
  }
}
  
void loop() {
  //chase(stripFront, WHITE);
  //light(stripFront, WHITE); // White front lights on
  //light(stripFront, blinkColor(0, WHITE)); // Blinking white front lights
  //lightTwoColors(stripFront, blinkColor(RED, BLACK), blinkColor(BLACK, BLUE)); // Police
  //lightFourColors(stripFront, ORANGE, RED, BLUE, WHITE); //
  lightThreeColors(stripFront, GREEN, RED, BLUE); //
  
  //light(stripRear, blinkColor(RED, GREEN)); // Red/Green rear lights
  //light(stripRear, blinkColor(RED, BLACK)); // Red blinking rear lights
  //light(stripRear, RED); // Brake
  lightTwoColors(stripRear, blinkColor(ORANGE, BLACK), blinkColor(ORANGE, BLACK)); // Orange blinking rear lights
  
  
  //delay(20);
  /*
  updateLED(LIGHTS_1 | LIGHTS_2 | LIGHTS_3);
  delay(1000);
  updateLED(BRAKES);
  delay(1000);
  updateLED(BLINK_RIGHT);
  delay(1000);
  updateLED(BLINK_LEFT);
  delay(1000);
  updateLED(OFF);
  
  */
  updateLCD();
}
