#pragma once
struct ArduboyTones {
  unsigned calls = 0;
  explicit ArduboyTones(bool (*)()) {}
  template<class... T> void tone(T...) { ++calls; }
  void noTone() {}
};
