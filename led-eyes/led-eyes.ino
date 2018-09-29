#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LEDS 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


const int SW_PIN = 7; // digital pin connected to joystick switch 
const int X_PIN = A0; // analog pin connected to X joystick 
const int Y_PIN = A1; // analog pin connected to Y joystick
const int S_PIN = A2; // analog pin connected to slider

int x = 0;
int y = 0;
int longpress = 0;

uint32_t wheel;

int r = 255;
int g = 255;
int b = 255;

int direction = 0; 
//default: 0, up: 1, upright: 2, right: 3, downright: 4, down: 5, downleft: 6, left: 7 , upleft: 8 

boolean up = false;
boolean down = false;
boolean left = false;
boolean right = false;




void setup() {
  pinMode(SW_PIN, INPUT);
  digitalWrite(SW_PIN, HIGH);

  strip.begin();
  Serial.begin(9600);
}




void loop() {    
  x = analogRead(X_PIN);
  y = analogRead(Y_PIN);

  up = x > 575;
  down = x < 425;
  left = y < 425;
  right = y > 575;
  
  direction = 0;


  if (up) direction = 1;
  else if (down) direction = 5;

  if (right) {
    direction = 3;
    
    if (up) direction = 2;
    else if (down) direction = 4;
  }
  
  if (left) {
    direction = 7;
    
    if (up) direction = 8;
    else if (down) direction = 6;
  }



  switch(direction) {
    case 1: // white
      if (r != 255) r += 1;
      if (g != 255) g += 1;
      if (b != 255) b += 1;
    break;

    case 2:
      if (r != 50) r < 50 ? r += 1 : r -= 1;
      if (g != 255) g += 1;
      if (b != 50) b < 50 ? b += 1 : b -= 1;
    break;

    case 3: // green
      if (r != 0) r -= 1;
      if (g != 255) g += 1;
      if (b != 0) b -= 1;
    break;

    case 4: // yellow
      if (r != 255) r += 1;
      if (g != 255) g += 1;
      if (b != 0) b -= 1;
    break;

    case 5: // red
      if (r != 255) r += 1;
      if (g != 0) g -= 1;
      if (b != 0) b -= 1;
    break;

    case 6: // pink
      if (r != 255) r += 1;
      if (g != 0) g -= 1;
      if (b != 255) b += 1;
    break;
          
    case 7: // blue
      if (r != 0) r -= 1;
      if (g != 0) g -= 1;
      if (b != 255) b += 1;
    break;

    case 8: // light-blue
      if (r != 50) r < 50 ? r += 1 : r -= 1;
      if (g != 50) g < 50 ? g += 1 : g -= 1;
      if (b != 255) b += 1;    
    break;
  }


           
  
  if (digitalRead(SW_PIN) == 1) {
    longpress = 0;
  } else {
    longpress += 1;
   
    if (longpress < 600) lightsOut();           
    else if (longpress == 600) colorWipe();
    else if (longpress == 2000) {
      fadeOut();
      delay(400);
      while (digitalRead(SW_PIN) != 1) {rainbow();}      
    }
  }



  setColor();
  setBrightness();
  strip.show();  
}


void setColor() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }         
}


void lightsOut() {
  strip.setBrightness(0);
}


void setBrightness() {  
  strip.setBrightness(analogRead(S_PIN) / 6);
}


void colorWipe() {
  for (int i = 0; i < LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
    setBrightness();
    strip.show();
    if (i != LEDS) delay(35);
  }
}


void fadeOut() {
  int brightness = strip.getBrightness();
    
  for (int i = brightness; i-- > 0;) {
    strip.setBrightness(i);
    strip.show();
    delay(35);
  } 
}

 
void rainbow() {
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < LEDS; i++) {
      wheel = getColor(((i * 256 / LEDS) + j) & 255);
      strip.setPixelColor(i, strip.Color(wheel >> 16, wheel >> 8, wheel));
    }

    setBrightness();
    strip.show();
    delay(2);
  }  
}


uint32_t getColor(byte position) {
  position = 255 - position;
  
  if (position < 85) {
    return strip.Color(255 - position * 3, 0, position * 3, 0);
  }
  
  if (position < 170) {
    position -= 85;
    return strip.Color(0, position * 3, 255 - position * 3, 0);
  }
  
  position -= 170;
  return strip.Color(position * 3, 255 - position * 3, 0, 0);
}
