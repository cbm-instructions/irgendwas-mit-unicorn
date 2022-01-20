#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "screen.h"
#include "persistence.h"
#include <TimeLib.h>
#include <DS1307RTC.h>

void setup(void) {
  Serial.begin(9600);
  setSyncProvider(RTC.get);
  if(timeStatus()!= timeSet) 
   Serial.println("Unable to sync with the RTC");
  else
   Serial.println("RTC has set the system time");      
  init_screen();
  init_persistence();
}

void loop(void) {
  clear();

  draw_date();
  draw_dots(get_days(), 200);

  button a = {'*', "Bestaetigen"};
  draw_button(a, (pos){160, 170});
  draw_text("test");
  delay(2000);
}
