
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void init_input() {
  customKeypad.begin();
}

char get_key() {
      // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    if(e.bit.EVENT == KEY_JUST_PRESSED) return (char)e.bit.KEY;
  }

  return NULL;
}

char get_key_blocking() {
  char key = NULL;

  while (key == NULL) {
    delay(10);
    key = get_key();
  }

  return key;
}