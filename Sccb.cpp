

#include <Wire.h>
#include "Sccb.h"

// Register values taken from numerous sources including
// https://github.com/desaster/ov7670test.

// OV7670 register values
const struct regval_list OV7670Registers[] = {

  {0x12, 0x80}, // Reset to default values
  {0x11, 0x80},
  {0x3B, 0x0A},
  {0x3A, 0x04},
  {0x3A, 0x04},
  {0x12, 0x04}, // Output format: rgb
  {0x8C, 0x00}, // Disable RGB444
  {0x40, 0xD0}, // Set RGB565
  {0x17, 0x16},
  {0x18, 0x04},
  {0x32, 0x24},
  {0x19, 0x02},
  {0x1A, 0x7A},
  {0x03, 0x0A},
  {0x15, 0x02},
  {0x0C, 0x04},
  {0x3E, 0x1A}, // Divide by 4
  {0x1E, 0x27},
  {0x72, 0x22}, // Downsample by 4
  {0x73, 0xF2}, // Divide by 4
  {0x4F, 0x80},
  {0x50, 0x80},
  {0x51, 0x00},
  {0x52, 0x22},
  {0x53, 0x5E},
  {0x54, 0x80},
  {0x56, 0x40},
  {0x58, 0x9E},
  {0x59, 0x88},
  {0x5A, 0x88},
  {0x5B, 0x44},
  {0x5C, 0x67},
  {0x5D, 0x49},
  {0x5E, 0x0E},
  {0x69, 0x00},
  {0x6A, 0x40},
  {0x6B, 0x0A},
  {0x6C, 0x0A},
  {0x6D, 0x55},
  {0x6E, 0x11},
  {0x6F, 0x9F},
  {0xB0, 0x84},
  {0xFF, 0xFF}  // End marker
};

/**********************************************************/
/***                 Sccb Class Functions               ***/
/**********************************************************/

// Class constructor
Sccb::Sccb(int _deviceAddress) {
  // Save incoming
  deviceAddress = _deviceAddress;

  // Startup the i2c engine
  Wire.begin();
}

// This function will scan the i2c bus for a valid device
// only expecting to find one in this application and that
// is the Camera's SCCB interface
bool Sccb::scanForDevices(void) {

  for (int address = 1; address <= 127; address++) {
    // This scanner uses the return value of
    // the Wire.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    int error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("i2C device found at: 0x");
      Serial.println(address, HEX);
      return true;
    }
  }
  Serial.println("No i2C devices found");
  return false;
}

// Read a single byte from address and return it as a byte
byte Sccb::readRegister(byte address) {
  byte data;

  Wire.beginTransmission(deviceAddress);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(deviceAddress, 1);
  data = Wire.read();
  Wire.endTransmission();

  return data;
}

// Writes a single byte (data) into address
int Sccb::writeRegister(byte address, byte data) {

  Wire.beginTransmission(deviceAddress);
  Wire.write(address);
  Wire.write(data);
  return Wire.endTransmission();
}

// Write an array of register-value entries to the SCCB interface
int Sccb::writeRegisterArray(const struct regval_list *vals) {

  while (vals->reg != 0xff || vals->val != 0xff) {
    int ret = writeRegister(vals->reg, vals->val);
    if (ret < 0) {
      return ret;
    }
    vals++;
  }
  return 0;
}

/**********************************************************/
/***               Camera Specific Functions            ***/
/**********************************************************/

// Reset all OV7670 registers to their default values
void Sccb::resetOV7670() {
  writeRegister(0x12, 0x80);
  delay(10);
  writeRegister(0x12, 0);
  delay(10);
}

// Read and display all of the OV7670 registers
void Sccb::readAllOV7670Registers() {
  byte reg;

  for (int i = 0; i <= 0xC9; i ++) {
    Serial.print("Register: ");
    Serial.print(i, HEX);
    Serial.print(" Value: ");
    reg = readRegister(i);
    Serial.println(reg, HEX);
  }
}

// Load OV7670 registers with appropriate values
void Sccb::initOV7670(void) {
  writeRegisterArray(OV7670Registers);
}

// Slow the frame rate enough for camera code to run with 8 MHz XCLK
// Approximately 1.5 frames/second
void Sccb::slowFrameRate(void) {

  // CLKRC register: Prescaler divide by 31
  byte reg = readRegister(0x11);
  writeRegister(0x11, (reg & 0b1000000) | 0b00011110);

  // DBLV register: PLL = 6
  reg = readRegister(0x6B);
  writeRegister(0x6B, (reg & 0b00111111) | 0b10000000);
}


