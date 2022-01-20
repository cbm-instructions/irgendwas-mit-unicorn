
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

char get_key_blocking(unsigned long timeout) {
  char key = NULL;
  unsigned long start = millis();
  while (key == NULL) {
    if (timeout != 0 && start + timeout <= millis()) return NULL;
    delay(10);
    key = get_key();
  }
  return key;
}
