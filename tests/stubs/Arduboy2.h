#pragma once
#include <cstdint>
#include <string>
#define UP_BUTTON 1
#define DOWN_BUTTON 2
#define LEFT_BUTTON 4
#define RIGHT_BUTTON 8
#define A_BUTTON 16
#define B_BUTTON 32
#define WHITE 1
#define BLACK 0
struct __FlashStringHelper {};
#define F(s) reinterpret_cast<const __FlashStringHelper *>(s)
struct Arduboy2 {
  struct { static bool enabled() { return true; } } audio;
  uint8_t buttons = 0, previous = 0, next = 0;
  unsigned frameCount = 0;
  std::string text;
  void begin() {}
  void setFrameRate(int) {}
  bool nextFrame() { ++frameCount; return true; }
  void pollButtons() { previous = buttons; buttons = next; }
  bool justPressed(uint8_t mask) { return (buttons & ~previous & mask) != 0; }
  bool pressed(uint8_t mask) { return (buttons & mask) == mask; }
  void clear() { text.clear(); }
  void display() {}
  void setCursor(int, int) {}
  void print(const __FlashStringHelper *s) { text += reinterpret_cast<const char *>(s); }
  void print(char c) { text += c; }
  void print(int n) { text += std::to_string(n); }
  template<class... T> void drawCircle(T...) {}
  template<class... T> void drawRect(T...) {}
  template<class... T> void drawPixel(T...) {}
  template<class... T> void drawTriangle(T...) {}
  template<class... T> void drawFastHLine(T...) {}
  template<class... T> void drawLine(T...) {}
  template<class... T> void fillScreen(T...) {}
  template<class... T> void setTextColor(T...) {}
  template<class... T> void fillRect(T...) {}
  template<class... T> void drawRoundRect(T...) {}
};
