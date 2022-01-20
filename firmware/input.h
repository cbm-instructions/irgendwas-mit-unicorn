#ifndef input_h
#define input_h

#include <Adafruit_Keypad.h>

const byte ROWS = 4; // rows
const byte COLS = 4; // columns

//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {40, 41, 42, 43}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {44, 45, 46, 47}; //connect to the column pinouts of the keypad

void init_input();
char get_key();
char get_key_blocking(unsigned long timeout);

int is_number(char input) {
  return (input >= 48 && input <= 57);
}

int as_number(char input) {
  return input - 48;
}

#endif
