#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "screen.h"
#include "persistence.h"
#include <TimeLib.h>
#include <DS1307RTC.h>
#include "input.h"

void setup(void) {
  Serial.begin(9600);
  setSyncProvider(RTC.get);
  if(timeStatus()!= timeSet) 
   Serial.println("Unable to sync with the RTC");
  else
   Serial.println("RTC has set the system time");

  pinMode(LED_BUILTIN, OUTPUT);
  init_screen();
  init_persistence();
  init_input();
}


void loop(void) {
  draw_tree(5);
  get_key_blocking(0);
  if (scene_last_ride()) {
    if (scene_release_key()) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void draw_common() {
  draw_date();
  draw_dots(get_days(), 200);

}

int scene_last_ride() {
  char input[50] = {0};
  clear();
  
  draw_common();
  
  draw_button((button){'*', "Bestaetigen"}, (pos){160, 124});
  draw_button((button){'#', "Loeschen"}, (pos){160, 147});
  draw_button((button){'D', "Abbrechen"}, (pos){160, 170});
  int question_yend = draw_text("Schaetzen Sie die Laenge Ihrer letzten Fahrt (km)");
  int input_offset = question_yend + 24;
  while(true) {
    draw_input(input, input_offset);
    char next_key = get_key_blocking(0);

    if (is_number(next_key)) input[strlen(input)] = next_key;
    else if (next_key == '#') {
      input[strlen(input) - 1] = 0;
    } else if (next_key == '*') break;
    else if (next_key == 'D') return 0;
    else next_key = NULL;
  }
  return 1;
}

int scene_release_key() {
  clear();
  draw_common();
  
  draw_text("Den Schluessel freigeben?");
  draw_button((button){'*', "Bestaetigen"}, (pos){160, 147});
  draw_button((button){'D', "Abbrechen"}, (pos){160, 170});

  while (true) {
    char next = get_key_blocking(0);
    if (next == '*') return 1;
    if (next == 'D') return 0;
  }
}
