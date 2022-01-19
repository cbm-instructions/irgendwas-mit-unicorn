#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <OSFS.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x56b5

#define BUFF_SIZE 1024

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(9600);;
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);

  tft.setRotation(3);
}

struct button {
  char content;
  char* label;
};

struct pos {
  int x;
  int y;
};

int* map_colors(int num[]) {
  for (int i = 0; i < 32; i++) {
    if (num[i] == -1) {
      num[i] = WHITE;
    } else if (num[i] == 0) {
      num[i] = GREEN;
    } else if (num[i] == 1) {
      num[i] = YELLOW;
    } else if (num[i] == -2) {
      num[i] = BLACK;
    } else if (num[i] >= 2) {
      num[i] = RED;
    }
  }
  return num;
}

void drawDots(int colors[], int offset) {
  tft.fillRect(0, offset, 320, 40, BLACK);
  
  for (int i = 0; i < 2; i++) {
    int offset_y = i * 20;
    for (int j = 0; j < 16; j++) {
      int offset_x = j * 20;
      tft.fillRect(offset_x + 2, offset_y + 2 + offset, 16, 16, colors[(i * 16) + j]);
    }
  }
}

void drawButton(struct button b, struct pos offset) {
  tft.drawRect(offset.x, offset.y, 10 + 8, 14 + 8, BLACK);
  tft.setCursor(offset.x + 4,offset.y + 4);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(b.content);
  tft.setTextColor(BLACK);
  tft.setCursor(offset.x + 20,offset.y + 4);
  tft.print(b.label);
}

void drawQuestion(const char* text) {
  int max_len = 23;

  char buf[BUFF_SIZE];
  strcpy(buf, text); // copying text to prevent text from beeing overwritten
  char *ptr = strtok(buf, " "); // split string

  int line_num = 0;
  char line[BUFF_SIZE] = {0};
  strcpy(line, ptr); // set first word
  ptr = strtok(NULL, " "); // and advance pointer to next
  
  while (ptr != NULL) {
    if (strlen(line) + strlen(ptr) + 1 > max_len) {
      tft.setCursor(20, 20 + line_num * 24);
      tft.print(line);
      memset(line, 0, BUFF_SIZE);
      line_num++;
    } else {
      strcat(strcat(line, " "), ptr);
      ptr = strtok(NULL, " ");
    }
  }
  tft.setCursor(15, 20 + line_num * 24);
  tft.print(line);
}

void loop(void) {
  tft.fillScreen(WHITE);

  int numbers[] = {2,3,4};
  
  drawDots(map_colors(numbers), 200);
  button a = {'*', "Bestätigen"};
  drawButton(a, (pos){100, 100});
  drawQuestion("Wie viele Kilometer sind Sie seit letzter Woche gefahren?");
  delay(2000);
}
