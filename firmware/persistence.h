#ifndef persistence_h
#define persistence_h

#include <EEPROM.h>
#include <TimeLib.h>

#define MEMORY_START 0

void clear_days();
int inc_day(int day);
int* get_days();
int get_day(int day);
void set_day(int day, int num);
float intensity();

#endif
