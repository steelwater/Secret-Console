#include <cassert>
#include <iostream>

void frame(uint8_t buttons = 0) {
  arduboy.next = buttons;
  loop();
}

void tap(uint8_t button) {
  frame(button);
  frame();
}

void enterSecret(uint8_t index) {
  const uint8_t buttons[] = { UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, A_BUTTON, B_BUTTON };
  for (uint8_t i = 0; i < Secrets[index].length; i++) {
    tap(buttons[Secrets[index].code[i]]);
  }
}

void finishReward() {
  for (int i = 0; i < 92; i++) {
    frame();
  }
  assert(activeSecret == 255);
}

int main() {
  static_assert(ScreenWidth == 128 && ScreenHeight == 64, "Arduboy display size");
  setup();
  tap(DOWN_BUTTON);
  tap(A_BUTTON);
  assert(screen == ScreenFound);
  assert(arduboy.text.find("None found yet.") != std::string::npos);
  tap(A_BUTTON);
  assert(sound.calls == 0 && unlockedCount() == 0);
  tap(B_BUTTON);
  assert(screen == ScreenMenu);
  tap(UP_BUTTON);
  tap(A_BUTTON);
  assert(screen == ScreenConsole);

  // A noncontiguous discovered subset must never expose locked entries.
  const uint8_t order[] = { 8, 2, 0, 9, 1, 3, 4, 5, 6, 7 };
  for (uint8_t n = 0; n < SecretCount; n++) {
    uint8_t index = order[n];
    unsigned before = sound.calls;
    enterSecret(index);
    assert(activeSecret == index && unlockedCount() == n + 1);
    assert(sound.calls == before + 1 && inputCount == 0);
    finishReward();
    for (int repeat = 0; repeat < 4; repeat++) {
      enterSecret(index);
      assert(activeSecret == index && unlockedCount() == n + 1);
      finishReward();
    }
    assert(sound.calls == before + 5);
    for (int i = 0; i < 30; i++) frame(B_BUTTON);
    frame();
    assert(screen == ScreenMenu && inputCount == 0);
    tap(DOWN_BUTTON);
    tap(A_BUTTON);
    assert(screen == ScreenFound);
    uint8_t expected = 0;
    for (uint8_t i = 0; i < SecretCount; i++) {
      if (!unlocked[i]) continue;
      assert(foundSecretIndex() == i);
      std::string name = reinterpret_cast<const char *>(secretName(Secrets[i].reward));
      assert(arduboy.text.find(name) != std::string::npos);
      std::string code;
      for (uint8_t c = 0; c < Secrets[i].length; c++) {
        if (c) code += ' ';
        code += inputLabel(Secrets[i].code[c]);
      }
      assert(arduboy.text.find(code) != std::string::npos);
      before = sound.calls;
      tap(A_BUTTON);
      assert(activeSecret == i && sound.calls == before + 1);
      finishReward();
      assert(screen == ScreenFound && foundSelection == expected);
      assert(unlockedCount() == n + 1);
      tap(DOWN_BUTTON);
      expected++;
    }
    assert(foundSelection == 0);
    tap(UP_BUTTON);
    assert(foundSelection == n);
    tap(B_BUTTON);
    tap(UP_BUTTON);
    tap(A_BUTTON);
    assert(screen == ScreenConsole);
  }
  assert(unlockedCount() == 10);
  // Replay remains enabled beyond byte-counter limits.
  for (int i = 0; i < 260; i++) {
    enterSecret(0);
    assert(activeSecret == 0 && unlockedCount() == 10);
    finishReward();
  }
  // A shorter match embedded in a longer code is replayed on timeout or mismatch.
  for (int attempt = 0; attempt < 2; attempt++) {
    const uint8_t prefix[] = { UP_BUTTON, UP_BUTTON, DOWN_BUTTON, DOWN_BUTTON,
                               LEFT_BUTTON, RIGHT_BUTTON, LEFT_BUTTON, RIGHT_BUTTON };
    for (uint8_t button : prefix) tap(button);
    assert(activeSecret == 255);
    if (attempt == 0) {
      for (int i = 0; i < IdleResetFrames; i++) frame();
    } else {
      tap(DOWN_BUTTON);
    }
    assert(activeSecret == 2 && unlockedCount() == 10);
    finishReward();
  }
  // The rolling trail and idle reset still work.
  for (int i = 0; i < 20; i++) tap(RIGHT_BUTTON);
  assert(inputCount == InputBufferSize);
  for (int i = 0; i < IdleResetFrames; i++) frame();
  assert(inputCount == 0);
  // Holding B also exits a reward; short B presses above still form codes.
  enterSecret(0);
  for (int i = 0; i < 30; i++) frame(B_BUTTON);
  frame();
  assert(screen == ScreenMenu && activeSecret == 255);
  tap(UP_BUTTON);
  tap(A_BUTTON);
  assert(screen == ScreenAbout);
  for (int i = 0; i < 3; i++) tap(A_BUTTON);
  assert(aboutPage == 3);
  for (int i = 0; i < 4; i++) tap(B_BUTTON);
  assert(screen == ScreenMenu);
  // The longest code must also work before its embedded shorter secret is found.
  for (uint8_t i = 0; i < SecretCount; i++) unlocked[i] = false;
  screen = ScreenConsole;
  clearConsoleInput();
  enterSecret(7);
  assert(activeSecret == 7 && unlockedCount() == 1 && !unlocked[2]);
  finishReward();
  enterSecret(2);
  assert(activeSecret == 2 && unlockedCount() == 2);
  std::cout << "PASS: all ten discoveries, repeated code/gallery rewards, discovered-only navigation, empty gallery, menu, About, rolling inputs and idle clear\n";
}
