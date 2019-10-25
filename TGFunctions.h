

#ifndef TGFUNCTIONS_H
#define TGFUNCTIONS_H

extern Adafruit_ST7735 lcd;

inline void clearDisplay() {
  lcd.fillScreen(ST7735_BLACK);
}

// Get the width in pixels of a text string of specified size
inline int getTextWidth(const char *text, int size) {
  int len = strlen(text);

  // Chars are 5 px wide with 1 px space = 6
  return 6 * len * size;
}

// Calculate the x offset necessary to center a text string of specified size
inline int xOffsetForCenteredText(const char *text, int size) {
  return (lcd.width() - getTextWidth(text, size)) / 2;
}

// Draw a text string at specified location with specified size and color
inline void drawText(int x, int y, int size, const char *text, uint16_t fgColor, uint16_t bgColor) {
  lcd.setCursor(x, y);
  lcd.setTextSize(size);
  lcd.setTextColor(fgColor, bgColor);
  lcd.print(text);
}

// Draw a text string of specified size and color centered on display with y position
inline void drawCenteredText(int y, int size, const char *text, uint16_t fgColor, uint16_t bgColor) {
  drawText(xOffsetForCenteredText(text, size), y, size, text, fgColor, bgColor);
}

// Draw display frame
inline void drawDisplayFrame() {
  // Draw frame around display
  lcd.drawRect(0, 0, lcd.width(),      lcd.height(),     ST7735_BLUE);
  lcd.drawRect(1, 1, lcd.width() - 2 , lcd.height() - 2, ST7735_RED);
  lcd.drawRect(2, 2, lcd.width() - 4 , lcd.height() - 4, ST7735_BLUE);
}

#endif

