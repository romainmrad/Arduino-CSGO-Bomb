#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>

// LEDs + Buzzers
const int green = 9;
const int yellow = 10;
const int red = 11;
const int b1 = 12;
const int b2 = 13;

// Keypad
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 5, 6, 7, 8 };
byte colPins[COLS] = { 2, 3, 4 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
unsigned long start;
unsigned long timer;
unsigned long plantTimer;
unsigned long defuseTimer;
unsigned long pauseTime;
int interval;
int n;
int k;
int hours;
int minutes;
int seconds;
int state;
int memory;
bool typing;

// Codes
char code[4];
char timerArr[6];
int intArr[6];
char armPass[4] = { '1', '7', '3', '9' };
char defPass[4] = { '2', '4', '8', '6' };
char editPass[4] = { '1', '1', '1', '1' };
char initPass[4] = { '0', '0', '0', '0' };
char begPass[4] = { '0', '0', '0', '1' };
char pauseTimePass[4] = { '0', '0', '0', '5' };
char endPass[4] = { '0', '0', '0', '9' };

bool isNum(char c) {
  return (c >= '0' && c <= '9');
}


// ------------------------------------- Code -------------------------------------- //


bool checkCode(char c[], char p[]) {
  return (c[0] == p[0] && c[1] == p[1] && c[2] == p[2] && c[3] == p[3]);
}

void resetCode() {
  n = 0;
  for (int i = 0; i < 4; i++)
    code[i] = 0;
  typing = false;
}

void wrongCode() {
  lcd.clear();
  typing = false;
  resetCode();
  lcd.print("Wrong code");
  tone(b1, 300, 100);
  delay(150);
  tone(b1, 500, 100);
  delay(1000);
  lcd.clear();
}

void printCode(int x, int y) {
  if (isNum(code[0])) {
    lcd.setCursor(x, y);
    lcd.print(code[0]);
    if (isNum(code[1])) {
      lcd.setCursor(x + 1, y);
      lcd.print(code[1]);
      if (isNum(code[2])) {
        lcd.setCursor(x + 2, y);
        lcd.print(code[2]);
        if (isNum(code[3])) {
          lcd.setCursor(x + 3, y);
          lcd.print(code[3]);
        }
      }
    }
  }
}


// ------------------------------------- Timer ------------------------------------- //


void printTimer(char arr[]) {
  lcd.setCursor(8, 1);
  char tmp[8];
  if (isNum(arr[0])) {
    tmp[0] = arr[0];
  } else {
    tmp[0] = ' ';
  }
  if (isNum(arr[1])) {
    tmp[1] = arr[1];
  } else {
    tmp[1] = ' ';
  }
  if (isNum(arr[2])) {
    tmp[3] = arr[2];
  } else {
    tmp[3] = ' ';
  }
  if (isNum(arr[3])) {
    tmp[4] = arr[3];
  } else {
    tmp[4] = ' ';
  }
  if (isNum(arr[4])) {
    tmp[6] = arr[4];
  } else {
    tmp[6] = ' ';
  }
  if (isNum(arr[5])) {
    tmp[7] = arr[5];
  } else {
    tmp[7] = ' ';
  }
  tmp[2] = ':';
  tmp[5] = ':';
  for (int i = 0; i < 8; i++) {
    lcd.setCursor(8 + i, 1);
    lcd.print(tmp[i]);
  }
}

void resetTimer() {
  for (int i = 0; i <= 5; i++) {
    timerArr[i] = 0;
  }
  k = 0;
}

void editTimers() {
  lcd.clear();
  k = 0;
  state = 6;
  typing = false;
  resetCode();
}

// ------------------------------------- State -------------------------------------- //


void initGame() {
  state = 0;
  interval = 1000;
  typing = false;
  timer = 0;
  n = 0;
  pauseTime = 0;
  plantTimer = 180000;
  defuseTimer = 60000;
  lcd.clear();
  resetCode();
  resetTimer();
  digitalWrite(yellow, HIGH);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
}

void resetGame() {
  state = 0;
  interval = 1000;
  typing = false;
  timer = 0;
  n = 0;
  pauseTime = 0;
  lcd.clear();
  resetCode();
  resetTimer();
  digitalWrite(yellow, HIGH);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
}

void bombArmed() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Bomb armed");
  typing = false;
  state = 2;
  start = millis();
  interval = 1000;
  timer = 0;
  resetCode();
  delay(1000);
  lcd.clear();
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
}

void bombDefused() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Bomb defused");
  resetCode();
  typing = false;
  state = 3;
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
}

void bombExploded() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Bomb exploded");
  resetCode();
  typing = false;
  state = 4;
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(b1, HIGH);
  digitalWrite(b2, HIGH);
  delay(2000);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
}

void bombLost() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Bomb lost");
  resetCode();
  typing = false;
  state = 5;
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(b1, HIGH);
  digitalWrite(b2, HIGH);
  delay(2000);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
}

void pauseTimeGame() {
  lcd.clear();
  resetCode();
  pauseTime = millis() - start;
  memory = state;
  state = 8;
  typing = false;
}

void resumeGame() {
  lcd.clear();
  resetCode();
  lcd.setCursor(0, 1);
  lcd.print("Resuming Game");
  state = memory;
  if (state == 1) {
    plantTimer -= pauseTime - 1000;
  } else if (state == 2) {
    defuseTimer -= pauseTime - 1000;
  }
  typing = false;
  start = millis();
  pauseTime = 0;
  delay(1000);
  lcd.clear();
}


// ------------------------------------- Flash ------------------------------------- //


float bps(int x) {
  return ((20.0 / 270) * exp(0.0383 * pow(x / 5.0, 2))) + 1;
}

void flash(int pin, bool accelerate, bool buzzer, int x) {
  if ((millis() - timer) >= interval) {
    timer = millis();
    digitalWrite(pin, !digitalRead(pin));
    if (buzzer && digitalRead(pin)) {
      buzz();
    }
    if (accelerate) {
      interval = 1000.0 / bps((millis() - start) / 1000.0 - x);
    }
  }
}

void buzz() {
  unsigned long t = millis();
  while (millis() - t < 50) {
    digitalWrite(b1, HIGH);
    digitalWrite(b2, HIGH);
  }
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
}

void keybuzz() {
  tone(b1, 2000, 100);
}


// ------------------------------------- Game ------------------------------------- //


void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  initGame();
  state = 0;
  start = 0;
  pauseTime = 0;
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
  switch (state) {
    //Init
    case 0:
      lcd.setCursor(0, 1);
      lcd.print("Standby");
      lcd.setCursor(0, 0);
      if (!typing) {
        if (key == '*') {
          keybuzz();
          typing = true;
          n = 0;
        }
      } else {
        lcd.setCursor(0, 0);
        lcd.print("code : ");
        if (key && isNum(key)) {
          keybuzz();
          if (n <= 3) {
            code[n] = key;
            n++;
          }
        }
        if (key == '*') {
          keybuzz();
          resetCode();
          lcd.clear();
        }
        printCode(7, 0);
        if (key == '#') {
          keybuzz();
          if (checkCode(code, begPass)) {
            typing = false;
            resetCode();
            state = 1;
            start = millis();
            lcd.clear();
          } else if (checkCode(code, editPass)) {
            editTimers();
          } else if (checkCode(code, initPass)) {
            initGame();
            lcd.clear();
            lcd.print("Initialized");
            delay(1000);
            lcd.clear();
          } else {
            wrongCode();
          }
        }
      }
      break;
    //Game Started
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("Time left : " + String((plantTimer + 1000 - (millis() - start)) / 1000) + "  ");
      if ((plantTimer + 1000 - (millis() - start)) / 1000 == 0) {
        bombLost();
      } else {
        if ((millis() - start) >= plantTimer - 60000) {
          flash(yellow, true, true, (plantTimer - 60000) / 1000);
        } else {
          flash(yellow, false, false, 0);
        }
        if (!typing) {
          if (key == '*') {
            keybuzz();
            typing = true;
            n = 0;
          }
        } else {
          lcd.setCursor(0, 0);
          lcd.print("code : ");
          if (key && isNum(key)) {
            keybuzz();
            if (n <= 3) {
            code[n] = key;
            n++;
          }
          }
          if (key == '*') {
            keybuzz();
            resetCode();
            lcd.clear();
          }
          printCode(7, 0);
          if (key == '#') {
            keybuzz();
            if (checkCode(code, endPass)) {
              initGame();
            } else if (checkCode(code, armPass)) {
              bombArmed();
            } else if (checkCode(code, pauseTimePass)) {
              pauseTimeGame();
            } else {
              wrongCode();
            }
          }
        }
      }
      break;
    //Bomb planted
    case 2:
      flash(red, true, true, (defuseTimer - 60000) / 1000);
      lcd.setCursor(0, 1);
      lcd.print("Time left : " + String((defuseTimer + 2000 - (millis() - start)) / 1000) + " ");
      if ((defuseTimer + 2000 - (millis() - start)) / 1000 <= 0) {
        bombExploded();
      } else {
        if (!typing) {
          if (key == '*') {
            keybuzz();
            typing = true;
            n = 0;
          }
        } else {
          lcd.setCursor(0, 0);
          lcd.print("code : ");
          if (key && isNum(key)) {
            keybuzz();
            if (n <= 3) {
            code[n] = key;
            n++;
          }
          }
          if (key == '*') {
            keybuzz();
            typing = false;
            n = 0;
            resetCode();
            lcd.clear();
          }
          printCode(7, 0);
          if (key == '#') {
            keybuzz();
            if (checkCode(code, endPass)) {
              initGame();
            } else if (checkCode(code, defPass)) {
              bombDefused();
            } else if (checkCode(code, pauseTimePass)) {
              pauseTimeGame();
            } else {
              wrongCode();
            }
          }
        }
      }
      break;
    //Bomb defused
    case 3:
      flash(green, false, false, 0);
      lcd.setCursor(0, 1);
      lcd.print("Bomb defused");
      if (!typing) {
        if (key == '*') {
          keybuzz();
          typing = true;
          n = 0;
        }
      } else {
        lcd.setCursor(0, 0);
        lcd.print("code : ");
        if (key && isNum(key)) {
          keybuzz();
          if (n <= 3) {
            code[n] = key;
            n++;
          }
        }
        if (key == '*') {
          keybuzz();
          typing = false;
          n = 0;
          resetCode();
          lcd.clear();
        }
        printCode(7, 0);
        if (key == '#') {
          keybuzz();
          if (checkCode(code, endPass)) {
            initGame();
          } else {
            wrongCode();
          }
        }
      }
      break;
    //Bomb exploded
    case 4:
      lcd.setCursor(0, 1);
      lcd.print("Bomb exploded");
      if (!typing) {
        if (key == '*') {
          keybuzz();
          typing = true;
          n = 0;
        }
      } else {
        lcd.setCursor(0, 0);
        lcd.print("code : ");
        if (key && isNum(key)) {
          keybuzz();
          if (n <= 3) {
            code[n] = key;
            n++;
          }
        }
        if (key == '*') {
          keybuzz();
          typing = false;
          n = 0;
          resetCode();
          lcd.clear();
        }
        printCode(7, 0);
        if (key == '#') {
          keybuzz();
          if (checkCode(code, endPass)) {
            initGame();
          } else {
            wrongCode();
          }
        }
      }
      break;
    //Bomb lost
    case 5:
      lcd.setCursor(0, 1);
      lcd.print("Bomb lost");
      if (!typing) {
        if (key == '*') {
          keybuzz();
          typing = true;
          n = 0;
        }
      } else {
        lcd.setCursor(0, 0);
        lcd.print("code : ");
        if (key && isNum(key)) {
          keybuzz();
          if (n <= 3) {
            code[n] = key;
            n++;
          }
        }
        if (key == '*') {
          keybuzz();
          typing = false;
          n = 0;
          resetCode();
          lcd.clear();
        }
        printCode(7, 0);
        if (key == '#') {
          keybuzz();
          if (checkCode(code, endPass)) {
            initGame();
          } else {
            wrongCode();
          }
        }
      }
      break;
    //Edit plant timer
    case 6:
      lcd.setCursor(0, 0);
      lcd.print("Plant timer :");
      printTimer(timerArr);
      if (key && isNum(key)) {
        timerArr[k] = key;
        intArr[k] = key - '0';
        keybuzz();
        k++;
      }
      if (key == '*') {
        keybuzz();
        lcd.clear();
        resetTimer();
      } else if (key == '#') {
        hours = (intArr[0] * 10.0 + intArr[1]);
        minutes = (intArr[2] * 10.0 + intArr[3]);
        seconds = (intArr[4] * 10.0 + intArr[5]);
        plantTimer = (hours * 3600.0 + minutes * 60.0 + seconds) * 1000.0;
        k = 0;
        state++;
        resetTimer();
        lcd.clear();
      }
      break;
    //Edit defuse timer
    case 7:
      lcd.print("Defuse timer :");
      printTimer(timerArr);
      if (key && isNum(key)) {
        timerArr[k] = key;
        intArr[k] = key - '0';
        keybuzz();
        k++;
      }
      if (key == '*') {
        keybuzz();
        lcd.clear();
        resetTimer();
      } else if (key == '#') {
        hours = (intArr[0] * 10.0 + intArr[1]);
        minutes = (intArr[2] * 10.0 + intArr[3]);
        seconds = (intArr[4] * 10.0 + intArr[5]);
        defuseTimer = (hours * 3600.0 + minutes * 60.0 + seconds) * 1000.0;
        k = 0;
        resetTimer();
        lcd.clear();
        state = 0;
      }
      break;
    //Pause Game
    case 8:
      lcd.setCursor(0, 1);
      lcd.print("pauseTime");
      lcd.setCursor(0, 0);
      if (!typing) {
        if (key == '*') {
          keybuzz();
          typing = true;
          n = 0;
        }
      } else {
        lcd.setCursor(0, 0);
        lcd.print("code : ");
        if (key && isNum(key)) {
          keybuzz();
          if (n <= 3) {
            code[n] = key;
            n++;
          }
        }
        if (key == '*') {
          keybuzz();
          resetCode();
          lcd.clear();
        }
        printCode(7, 0);
        if (key == '#') {
          if (checkCode(code, pauseTimePass)) {
            resumeGame();
          } else if (checkCode(code, endPass)) {
            resetGame();
          } else {
            wrongCode();
          }
        }
      }
      break;
  }
}