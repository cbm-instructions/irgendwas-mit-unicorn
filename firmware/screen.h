#ifndef screen_h
#define screen_h

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x56b5

typedef struct {
  char content;
  char* label;
} button;

typedef struct {
  int x;
  int y;
} pos;

void init_screen();
void draw_dots(const int days[], int offest);
void draw_button(button b, pos offest);
int draw_text(const char* text);
void draw_date();

#endif
