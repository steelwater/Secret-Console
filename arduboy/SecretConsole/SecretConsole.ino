#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

constexpr uint8_t ScreenWidth = 128;
constexpr uint8_t ScreenHeight = 64;
constexpr uint8_t InputBufferSize = 16;
constexpr uint8_t SecretCount = 10;
constexpr uint8_t IdleResetFrames = 90;

enum InputCode : uint8_t {
  InputUp,
  InputDown,
  InputLeft,
  InputRight,
  InputA,
  InputB
};

enum RewardType : uint8_t {
  RewardMoonCat,
  RewardRocket,
  RewardUfo,
  RewardRobot,
  RewardCrash,
  RewardExplosion,
  RewardCursor,
  RewardDevRoom,
  RewardHuni,
  RewardFinal
};

struct Secret {
  const InputCode *code;
  uint8_t length;
  RewardType reward;
};

const InputCode MoonCatCode[] = { InputUp, InputUp, InputA };
const InputCode RocketCode[] = { InputA, InputA, InputUp };
const InputCode UfoCode[] = { InputLeft, InputRight, InputLeft, InputRight };
const InputCode RobotCode[] = { InputB, InputA, InputB, InputA };
const InputCode CrashCode[] = { InputUp, InputDown, InputUp, InputDown };
const InputCode ExplosionCode[] = { InputA, InputB, InputUp, InputDown };
const InputCode CursorCode[] = { InputLeft, InputLeft, InputA };
const InputCode DevRoomCode[] = {
  InputUp, InputUp, InputDown, InputDown, InputLeft,
  InputRight, InputLeft, InputRight, InputB, InputA
};
const InputCode HuniCode[] = { InputA, InputUp, InputB, InputUp };
const InputCode FinalCode[] = { InputB, InputB, InputA, InputA, InputLeft, InputRight };

const Secret Secrets[SecretCount] = {
  { MoonCatCode, 3, RewardMoonCat },
  { RocketCode, 3, RewardRocket },
  { UfoCode, 4, RewardUfo },
  { RobotCode, 4, RewardRobot },
  { CrashCode, 4, RewardCrash },
  { ExplosionCode, 4, RewardExplosion },
  { CursorCode, 3, RewardCursor },
  { DevRoomCode, 10, RewardDevRoom },
  { HuniCode, 4, RewardHuni },
  { FinalCode, 6, RewardFinal }
};

InputCode inputBuffer[InputBufferSize];
uint8_t inputCount = 0;
uint8_t totalInputs = 0;
uint8_t idleFrames = 0;
uint8_t rewardTimer = 0;
uint8_t activeSecret = 255;
bool unlocked[SecretCount];
bool started = false;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.clear();
}

void loop() {
  if (!arduboy.nextFrame()) {
    return;
  }

  arduboy.pollButtons();

  if (!started) {
    drawStartup();
    if (arduboy.justPressed(UP_BUTTON | DOWN_BUTTON | LEFT_BUTTON | RIGHT_BUTTON | A_BUTTON | B_BUTTON)) {
      started = true;
    }
    return;
  }

  if (activeSecret != 255) {
    drawReward();
    return;
  }

  if (!readButtons()) {
    updateIdleReset();
  }
  drawConsole();
}

bool readButtons() {
  bool pressed = false;

  if (arduboy.justPressed(UP_BUTTON)) {
    addInput(InputUp);
    pressed = true;
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
    addInput(InputDown);
    pressed = true;
  }
  if (arduboy.justPressed(LEFT_BUTTON)) {
    addInput(InputLeft);
    pressed = true;
  }
  if (arduboy.justPressed(RIGHT_BUTTON)) {
    addInput(InputRight);
    pressed = true;
  }
  if (arduboy.justPressed(A_BUTTON)) {
    addInput(InputA);
    pressed = true;
  }
  if (arduboy.justPressed(B_BUTTON)) {
    addInput(InputB);
    pressed = true;
  }

  return pressed;
}

void addInput(InputCode input) {
  idleFrames = 0;

  if (inputCount < InputBufferSize) {
    inputBuffer[inputCount] = input;
    inputCount++;
  } else {
    for (uint8_t i = 1; i < InputBufferSize; i++) {
      inputBuffer[i - 1] = inputBuffer[i];
    }
    inputBuffer[InputBufferSize - 1] = input;
  }

  totalInputs++;
  checkSecrets();
}

void checkSecrets() {
  for (uint8_t i = 0; i < SecretCount; i++) {
    if (!unlocked[i] && matchesSecret(Secrets[i])) {
      unlocked[i] = true;
      activeSecret = i;
      rewardTimer = 0;
      clearConsoleInput();
      playRewardTone(Secrets[i].reward);
      return;
    }
  }
}

void updateIdleReset() {
  if (inputCount == 0) {
    return;
  }

  if (idleFrames < IdleResetFrames) {
    idleFrames++;
  }

  if (idleFrames >= IdleResetFrames) {
    clearConsoleInput();
  }
}

void clearConsoleInput() {
  inputCount = 0;
  idleFrames = 0;
}

bool matchesSecret(const Secret &secret) {
  if (inputCount < secret.length) {
    return false;
  }

  uint8_t start = inputCount - secret.length;
  for (uint8_t i = 0; i < secret.length; i++) {
    if (inputBuffer[start + i] != secret.code[i]) {
      return false;
    }
  }

  return true;
}

void playRewardTone(RewardType reward) {
  uint16_t base = 220 + (uint8_t)reward * 35;
  sound.tone(base, 70);
}

void drawStartup() {
  arduboy.clear();
  arduboy.setCursor(20, 18);
  arduboy.print(F("SECRET CONSOLE"));
  arduboy.setCursor(24, 40);
  arduboy.print(F("Press Any Button"));
  arduboy.display();
}

void drawConsole() {
  arduboy.clear();
  drawPrompt();
  drawInputTrail();
  drawSecretCount();
  arduboy.display();
}

void drawPrompt() {
  arduboy.setCursor(0, 0);
  arduboy.print(F(">"));

  if ((arduboy.frameCount / 15) % 2 == 0) {
    arduboy.print(F("_"));
  }
}

void drawInputTrail() {
  uint8_t shown = inputCount < 12 ? inputCount : 12;
  uint8_t start = inputCount - shown;

  arduboy.setCursor(0, 16);
  for (uint8_t i = start; i < inputCount; i++) {
    arduboy.print(inputLabel(inputBuffer[i]));
    arduboy.print(F(" "));
  }
}

char inputLabel(InputCode input) {
  switch (input) {
    case InputUp: return 'U';
    case InputDown: return 'D';
    case InputLeft: return 'L';
    case InputRight: return 'R';
    case InputA: return 'A';
    case InputB: return 'B';
  }

  return '?';
}

void drawSecretCount() {
  arduboy.setCursor(0, 56);
  arduboy.print(F("Secrets "));
  arduboy.print(unlockedCount());
  arduboy.print(F("/10"));
}

uint8_t unlockedCount() {
  uint8_t count = 0;
  for (uint8_t i = 0; i < SecretCount; i++) {
    if (unlocked[i]) {
      count++;
    }
  }
  return count;
}

void drawReward() {
  rewardTimer++;

  arduboy.clear();
  arduboy.setCursor(28, 0);
  arduboy.print(F("SECRET FOUND"));

  switch (Secrets[activeSecret].reward) {
    case RewardMoonCat:
      drawMoonCat();
      break;
    case RewardRocket:
      drawRocket();
      break;
    case RewardUfo:
      drawUfo();
      break;
    case RewardRobot:
      drawRobot();
      break;
    case RewardCrash:
      drawCrash();
      break;
    case RewardExplosion:
      drawExplosion();
      break;
    case RewardCursor:
      drawCursorEscape();
      break;
    case RewardDevRoom:
      drawDevRoom();
      break;
    case RewardHuni:
      drawHuni();
      break;
    case RewardFinal:
      drawFinal();
      break;
  }

  arduboy.display();

  if (rewardTimer > 90) {
    activeSecret = 255;
  }
}

void drawMoonCat() {
  uint8_t x = rewardTimer % 128;
  arduboy.setCursor(41, 12);
  arduboy.print(F("Moon Cat"));
  arduboy.drawCircle(12, 18, 8, WHITE);
  arduboy.drawRect(x, 37, 14, 8, WHITE);
  arduboy.drawPixel(x + 3, 35, WHITE);
  arduboy.drawPixel(x + 10, 35, WHITE);
  arduboy.setCursor(50, 52);
  arduboy.print(F("meow"));
}

void drawRocket() {
  uint8_t y = 51 - (rewardTimer / 2);
  arduboy.setCursor(28, 12);
  arduboy.print(F("Rocket Launch"));
  arduboy.drawTriangle(64, y, 58, y + 12, 70, y + 12, WHITE);
  arduboy.drawRect(60, y + 12, 9, 14, WHITE);
  arduboy.drawPixel(59, 58, WHITE);
  arduboy.drawPixel(67, 60, WHITE);
  arduboy.drawPixel(72, 57, WHITE);
}

void drawUfo() {
  uint8_t x = (rewardTimer * 2) % 140;
  arduboy.setCursor(42, 12);
  arduboy.print(F("Tiny UFO"));
  arduboy.drawFastHLine(x - 13, 36, 27, WHITE);
  arduboy.drawFastHLine(x - 9, 32, 19, WHITE);
  arduboy.drawFastHLine(x - 9, 40, 19, WHITE);
  arduboy.drawLine(x - 13, 36, x - 9, 32, WHITE);
  arduboy.drawLine(x + 13, 36, x + 9, 32, WHITE);
  arduboy.drawLine(x - 13, 36, x - 9, 40, WHITE);
  arduboy.drawLine(x + 13, 36, x + 9, 40, WHITE);
  arduboy.drawCircle(x, 31, 5, WHITE);
  arduboy.drawPixel(x - 8, 41, WHITE);
  arduboy.drawPixel(x + 8, 41, WHITE);
}

void drawRobot() {
  arduboy.setCursor(24, 12);
  arduboy.print(F("Dancing Robot"));
  uint8_t sway = (rewardTimer / 8) % 2;
  arduboy.drawRect(54, 30, 20, 18, WHITE);
  arduboy.drawRect(58, 23, 12, 7, WHITE);
  arduboy.drawLine(54, 35, 44 + sway * 6, 28, WHITE);
  arduboy.drawLine(74, 35, 84 - sway * 6, 28, WHITE);
  arduboy.drawPixel(61, 26, WHITE);
  arduboy.drawPixel(67, 26, WHITE);
}

void drawCrash() {
  if ((rewardTimer / 4) % 2 == 0) {
    arduboy.fillScreen(WHITE);
    arduboy.setTextColor(BLACK);
  }

  arduboy.setCursor(28, 22);
  arduboy.print(F("SYSTEM ERROR"));
  arduboy.setCursor(31, 42);
  arduboy.print(F("JUST KIDDING"));
  arduboy.setTextColor(WHITE);
}

void drawExplosion() {
  arduboy.setCursor(22, 12);
  arduboy.print(F("Screen Explosion"));

  for (uint8_t i = 0; i < 24; i++) {
    uint8_t x = 64 + ((i * 11 + rewardTimer * 2) % 58) - 29;
    uint8_t y = 36 + ((i * 7 + rewardTimer) % 34) - 17;
    arduboy.drawPixel(x, y, WHITE);
  }

  arduboy.drawCircle(64, 36, rewardTimer % 24, WHITE);
}

void drawCursorEscape() {
  arduboy.setCursor(27, 12);
  arduboy.print(F("Cursor Escape"));
  uint8_t x = (rewardTimer * 3) % 118;
  uint8_t y = 28 + ((rewardTimer * 5) % 24);
  arduboy.setCursor(x, y);
  arduboy.print(F(">_"));
}

void drawDevRoom() {
  arduboy.drawRect(8, 18, 112, 38, WHITE);
  arduboy.setCursor(45, 26);
  arduboy.print(F("HI DAN"));
  arduboy.setCursor(26, 42);
  arduboy.print(F("Secret Dev Room"));
}

void drawHuni() {
  arduboy.setCursor(38, 12);
  arduboy.print(F("Huni Robot"));
  arduboy.drawRoundRect(45, 26, 38, 24, 4, WHITE);
  arduboy.fillRect(54, 34, 4, 4, WHITE);
  arduboy.fillRect(70, 34, 4, 4, WHITE);
  arduboy.drawFastHLine(57, 44, 15, WHITE);
  arduboy.setCursor(48, 54);
  arduboy.print(F("HELLO"));
}

void drawFinal() {
  arduboy.setCursor(22, 16);
  arduboy.print(F("10/10 COMPLETE"));
  arduboy.drawRect(36, 34, 56, 12, WHITE);
  arduboy.fillRect(38, 36, (rewardTimer % 52), 8, WHITE);
  arduboy.setCursor(20, 54);
  arduboy.print(F("The console smiles"));
}
