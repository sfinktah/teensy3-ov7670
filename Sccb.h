


#ifndef SCCB_H
#define SCCB_H

// Register, value list entry
struct regval_list {
  byte reg;
  byte val;
};

class Sccb {

  public:
    Sccb(int _deviceAddress);

    bool scanForDevices(void);
    void resetOV7670(void);
    void readAllOV7670Registers(void);
    void initOV7670(void);
    void slowFrameRate(void);

  private:
    int deviceAddress;

    byte readRegister(byte address);
    int writeRegister(byte address, byte data);
    int writeRegisterArray(const struct regval_list *vals);
};

#endif


