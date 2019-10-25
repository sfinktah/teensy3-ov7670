
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "Camera.h"

extern Adafruit_ST7735 lcd;

// Class Constructor
Camera::Camera(byte _vSync, byte _hRef, byte _pClk,
               byte _d0, byte _d1, byte _d2, byte _d3,
               byte _d4, byte _d5, byte _d6, byte _d7,
               byte _reset) {

  // Saving incoming
  vSync = _vSync;
  hRef = _hRef;
  pClk = _pClk;
  d0 = _d0;
  d1 = _d1;
  d2 = _d2;
  d3 = _d3;
  d4 = _d4;
  d5 = _d5;
  d6 = _d6;
  d7 = _d7;
  reset = _reset;

  // Setup all GPIO pins as inputs
  pinMode(vSync, INPUT);
  pinMode(hRef,  INPUT);
  pinMode(pClk,  INPUT);
  pinMode(d0,    INPUT);
  pinMode(d1,    INPUT);
  pinMode(d2,    INPUT);
  pinMode(d3,    INPUT);
  pinMode(d4,    INPUT);
  pinMode(d5,    INPUT);
  pinMode(d6,    INPUT);
  pinMode(d7,    INPUT);

  // Initialize reset signal
  pinMode(reset, OUTPUT);

  // Put OV7670 into reset
  digitalWrite(reset, LOW);
}

// Control the camera reset signal.
// Low is reset; High is run
void Camera::setCameraReset(bool b) {
  digitalWrite(reset, b);
}

// Read a byte of the pixel data
inline byte Camera::readPixel(void) {
  byte b = 0;

  if (digitalReadFast(d7)) {
    b |= 0x80;
  }
  if (digitalReadFast(d6)) {
    b |= 0x40;
  }
  if (digitalReadFast(d5)) {
    b |= 0x20;
  }
  if (digitalReadFast(d4)) {
    b |= 0x10;
  }
  if (digitalReadFast(d3)) {
    b |= 0x08;
  }
  if (digitalReadFast(d2)) {
    b |= 0x04;
  }
  if (digitalReadFast(d1)) {
    b |= 0x02;
  }
  if (digitalReadFast(d0)) {
    b |= 0x01;
  }
  return b;
}

inline bool Camera::checkVSync(void) {
  return digitalReadFast(vSync);
}

inline bool Camera::checkHRef(void) {
  return digitalReadFast(hRef);
}

inline bool Camera::checkPClk(void) {
  return digitalReadFast(pClk);
}


// Acquire and display RGB565 image from the camera
void Camera::acquireAndDisplayRGBImage(void) {
  byte hByte;
  int i = 0;

  while (checkVSync());    // Wait for the old frame to end
  while (! checkVSync());  // Wait for a new frame to start

  while (checkVSync()) {
    while (checkVSync() && ! checkHRef()); // Wait for a line to start
    if (! checkVSync()) {
      break;  // Line didn't start, but frame ended
    }
    while (checkHRef()) {    // Wait for a line to end
      while (! checkPClk()); // Wait for clock to go high

      // Read the high byte of the RGB565 pixel's value
      hByte = readPixel();

      while (checkPClk());   // Wait for clock to go back low

      while (! checkPClk()); // Wait for clock to go high
      // Read the low byte of the RGB565 pixel's value,
      // combine it with the high byte and store it
      image[i++] = (hByte << 8) | readPixel();

      while (checkPClk());   // Wait for clock to go back low
    }
  }
  Serial.println(i);

  // Now display the QQVGA image
  for (int y = 0; y < 120; y++) {
    for (int x = 0; x < 160; x++) {
      lcd.drawPixel(x, y, image[y * 160 + x]);
    }
  }
}


