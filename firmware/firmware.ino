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
  //Serial.println(get_touch().z);
  draw_tree(5);
  if (get_key_blocking(60000) != NULL) {
    if (scene_last_ride()) {
      if (scene_release_key()) {
        scene_open_countdown();
      }
    }
  }
}

void draw_common() {
  draw_date();
  draw_dots(get_days(), 200);

}

void scene_open_countdown() {
  for (int i = 9; i >= 0; i--) {
    clear();
    draw_center_char(i + 48);
    draw_button((button){'D', "Abbrechen"}, (pos){160, 190});
    unsigned long start = millis();
    while (millis() < start + 1000) {
      if (get_key_blocking(start + 1000 - millis()) == 'D') return;
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  inc_day(day());
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
