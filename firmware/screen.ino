#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "persistence.h"

#define BUFF_SIZE 1024

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void init_screen() {
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(3);
  randomSeed(analogRead(8));

  Serial.println("Screen initialized");
}

int* map_colors(int dest[], const int src[]) {
  for (int i = 0; i < 32; i++) {
    if (src[i] < 0) {
      dest[i] = BLACK;
    } else if (src[i] == 0) {
      dest[i] = GREEN;
    } else if (src[i] == 1) {
      dest[i] = YELLOW;
    } else if (src[i] >= 2) {
      dest[i] = RED;
    }
  }
  return dest;
}

int* cover_future_days(int dest[], const int src[]) {
  for (int i = 0; i < 32; i++) {
    if (src[i] == BLACK) {
      dest[i] = BLACK;
    } else if (i < day()) {
      dest[i] = src[i];
    } else {
      dest[i] = WHITE;
    }
  }
  return dest;
}

void draw_date() {
  char date_str[11];

  sprintf(date_str, "%04d-%02d-%02d", year(), month(), day());

  tft.setCursor(2,190);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.print(date_str);
}

void draw_input(const char* input, int offset) {
  tft.fillRect(20 + strlen(input) * 12, offset, 320 - 20 - strlen(input) * 12, 16, WHITE);
  tft.setCursor(20, offset);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print(input);
}

void draw_dots(const int days[], int offset) {
  int colors[32];
  cover_future_days(colors, map_colors(colors, days));
  
  tft.fillRect(0, offset, 320, 40, BLACK);
  
  for (int i = 0; i < 2; i++) {
    int offset_y = i * 20;
    for (int j = 0; j < 16; j++) {
      int offset_x = j * 20;
      tft.fillRect(offset_x + 2, offset_y + 2 + offset, 16, 16, colors[(i * 16) + j]);
    }
  }
}

void draw_button(button b, pos offset) {
  tft.drawRect(offset.x, offset.y, 10 + 8, 14 + 8, BLACK);
  tft.setCursor(offset.x + 4,offset.y + 4);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(b.content);
  tft.setTextColor(BLACK);
  tft.setCursor(offset.x + 20,offset.y + 4);
  tft.print(b.label);
}

int draw_center_char(char num) {
  tft.setCursor(130,80);
  tft.setTextColor(BLACK);
  tft.setTextSize(10);

  tft.print(num);
}

int draw_text(const char* text) {
  int max_len = 24;
  tft.setTextSize(2);

  char buf[BUFF_SIZE];
  strcpy(buf, text); // copying text to prevent text from beeing overwritten
  char *ptr = strtok(buf, " "); // split string

  int line_num = 0;
  char line[BUFF_SIZE] = {0};
  strcpy(line, ptr); // set first word
  ptr = strtok(NULL, " "); // and advance pointer to next
  
  while (ptr != NULL) {
    if (strlen(line) + strlen(ptr) + 1 > max_len) {
      tft.setCursor(15, 20 + line_num * 24);
      tft.print(line);
      memset(line, 0, BUFF_SIZE);
      line_num++;
    } else {
      if (line[0] != 0) strcat(line, " ");
      strcat(line, ptr);
      ptr = strtok(NULL, " ");
    }
  }
  tft.setCursor(15, 20 + line_num * 24);
  tft.print(line);
  return 20 + (line_num + 1) * 24;
}

int draw_tree(int depth) {
  clear();
  draw_branch(depth, (pos){160, 240}, 0.0, 40);
}

int select_random(int a, int b) {
  if (random(0,2)) return a;
  else return b;
}

int draw_branch(int depth, pos start, float rad, float len) {
  float percent = intensity() * (random(80, 100) / 100.);

  if (depth == 0) {
    int color;
    color += ((int)(percent * 63) << 5);
    color += ((int)((1.0 -percent) * 31) << 11);
    
    tft.fillCircle(start.x, start.y, random(5, 15), color);
    return 0;
  };
  float r_len = len * percent + 10;
  int new_x = start.x + (r_len * sin(rad));
  int new_y = start.y - (r_len * cos(rad));
  tft.drawLine(start.x, start.y, new_x, new_y, BROWN);

  int num_next_branches = random(2, 4);
  for (int i = 0; i < num_next_branches; i++){
    draw_branch(depth - 1, (pos){new_x, new_y}, rad + random(-100, 100) / 100.0, len);
  }
  return 0;
}

void clear() {
    tft.fillScreen(WHITE);
}
