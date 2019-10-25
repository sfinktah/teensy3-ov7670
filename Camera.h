

#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>

class Camera {

  public:

    Camera(byte _vSync, byte _hRef, byte _pClk,
           byte _d0, byte _d1, byte _d2, byte _d3,
           byte _d4, byte _d5, byte _d6, byte _d7,
           byte _reset);

    void setCameraReset(bool b);
    void acquireAndDisplayRGBImage(void);

  private:
    byte vSync, hRef, pClk, d0, d1, d2, d3, d4, d5, d6, d7, reset;

    // QQVGA image buffer in RGB565 format
    uint16_t image[160 * 120];

    bool checkVSync(void);
    bool checkHRef(void);
    bool checkPClk(void);
    byte readPixel(void);

};

#endif


