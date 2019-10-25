/*
 * Teensy 3.1 - OV7670 Camera Chip (no fifo) Program
 *
 * This hardware and software combination allows the Teensy 3.1
 * to acquire and display QQVGA (160x120) images using the RGB565 format.
 * Currently image acquisition rate is approximately 1.5 images/second.
 * Acquired images are immediately displayed on LCD. They could be stored on
 * SD card as well but this code doesn't do that.
 *
 * Hardware:
 *   Teensy 3.1 microcontroller
 *   OV7670 camera chip
 *   Adafruit 1.8" TFT SPI LCD display with SD card interface ID:358
 *   
 * Software:
 *   Requires Adafruit ST7735 LCD library and the Adafruit GFX library
 *
 * You are free to use this code anyway you want.
 *
 * Concept, design and implementation by:
 * Craig A. Lindley - calhjh@gmail.com
 * Last Update: 11/20/2015
 */

// System include files
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

// Local include files
#include "Camera.h"
#include "Sccb.h"
#include "TGFunctions.h"

/**********************************************************/
/***                  Display Parameters                ***/
/**********************************************************/
// Teensy 3.1 to LCD interface
const byte LCD_LITE =  0;
const byte LCD_CS   = 10;
const byte LCD_DC   =  2;
const byte LCD_RST  =  0;

Adafruit_ST7735 lcd = Adafruit_ST7735(LCD_CS, LCD_DC, LCD_RST);

/**********************************************************/
/***                   Camera Parameters                ***/
/**********************************************************/
// Teensy 3.1 to Camera chip interface
const byte VSYNC =  4;
const byte HREF  =  5;
const byte PCLK  =  6;
const byte D7    = 20;
const byte D6    = 17;
const byte D5    = 16;
const byte D4    = 15;
const byte D3    = 14;
const byte D2    =  9;
const byte D1    =  8;
const byte D0    =  7;
const byte RESET = 21;

// Create a camera object instance
Camera camera = Camera(VSYNC, HREF, PCLK, D0, D1, D2, D3, D4, D5, D6, D7, RESET);

/**********************************************************/
/***                    Sccb Creation                   ***/
/**********************************************************/

// i2c address of camera's sccb device
#define SCCB_ADDRESS 0x21

// Create an SCCB object instance
Sccb sccb = Sccb(SCCB_ADDRESS);

/**********************************************************/
/***                    Misc Parameters                 ***/
/**********************************************************/

const byte SD_CS    =  1; // SD interface chip select
const byte XCLK     =  3; // 8 MHz output
const byte SW       = 22; // Pushbutton switch

/**********************************************************/
/***                    Program Setup                   ***/
/**********************************************************/

void setup() {

  Serial.begin(115200);  // start serial for output
  delay(1000);

  // Setup backlight and set to off
  pinMode(LCD_LITE, OUTPUT);
  digitalWrite(LCD_LITE, LOW);

  // Initialize a ST7735S display chip, black tab
  lcd.initR(INITR_BLACKTAB);

  // Set display orientation to landscape
  lcd.setRotation(3);

  // Create 8 MHz clock output 50% duty cycle for Camera's XCLK
  analogWriteFrequency(XCLK, 8000000);
  analogWrite(XCLK, 127);
  delay(100);

  // Release hardware reset line on OV7670
  camera.setCameraReset(true);
  delay(100);

  clearDisplay();

  // Display message
  drawCenteredText(62, 2, "Teensy Camera", ST7735_GREEN, ST7735_BLACK);

  // Turn backlight on
  digitalWrite(LCD_LITE, HIGH);
  delay(3000);

  // Configure the camera for operation
  sccb.initOV7670();
  sccb.slowFrameRate();
}

/**********************************************************/
/***                    Program Loop                    ***/
/**********************************************************/

void loop() {

  // Acquire and display an images as fast as possible
  camera.acquireAndDisplayRGBImage();

  delay(1000);
}




