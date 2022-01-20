struct {
    int month;
    int year;
    int days[32];
} data;

void init_persistence() {
  EEPROM.get(MEMORY_START, data);
  if (data.month != month() || data.year != year()) clear_days();
  Serial.println("Persistence initialized");
}

void clear_days() {
  data.year = year();
  data.month = month();
  memset(data.days, -1, 32);
  memset(data.days, 0, sizeof(int) * num_days(month()));
  
  EEPROM.put(MEMORY_START, data);
  Serial.println("Days cleared");
}

void inc_day(int day) {
  data.days[day - 1] += 1;
  EEPROM.put(MEMORY_START, data);
}

int* get_days() {
  return data.days;
}

int get_day(int day) {
  return data.days[day -1];
}

int num_days(int month) {
  if (month < 8) {
    if (month == 2) return 28;
    if (month % 2 == 0) return 30;
    return 31;
  } else {
    if (month % 2 == 0) return 31;
    return 30;
  }
}
