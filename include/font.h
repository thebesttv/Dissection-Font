#include "lcd/lcd.h"

const int FONT_POINT_SIZE = 6;  // size of each pixel
// color for different categories of pixel
const uint16_t COLOR[] = {0xE24B, 0xA25C, 0xFFC8, 0x6788};


/*
  dimensions:
  +-----------------------------------------+ - - - -
  |                                         |  border height
  |                     +---------------+ - | - - - -
  |                     |               |   |
  |   +--------+        |               |   |
  |   |        |        |               |   |
  |   | square |        |    letter     |   |
  |   |        |        |               |   |
  |   +--------+        |               |   |
  |<->|        |<------>|               |   |
  |border        middle +---------------+   |
  |width         width                  |<->|
  +-----------------------------------------+
                                        border width
 */

// dimensions of square & letter
const int SQUARE_WIDTH  =  6 * FONT_POINT_SIZE;
const int SQUARE_HEIGHT =  6 * FONT_POINT_SIZE;
const int LETTER_WIDTH  = 12 * FONT_POINT_SIZE;
const int LETTER_HEIGHT = 12 * FONT_POINT_SIZE;
// dimensions of blank margin or spaces
const int BORDER_WIDTH = (LCD_W - SQUARE_WIDTH - LETTER_WIDTH) / 3;
const int MIDDLE_WIDTH = LCD_W - BORDER_WIDTH * 2 - SQUARE_WIDTH - LETTER_WIDTH;
const int BORDER_HEIGHT = (LCD_H - (SQUARE_HEIGHT > LETTER_HEIGHT ? SQUARE_HEIGHT : LETTER_HEIGHT)) / 2;
// top-left corner of square & letter
const int SQUARE_X = BORDER_WIDTH;
const int SQUARE_Y = (LCD_H - SQUARE_HEIGHT) / 2;
const int LETTER_X = BORDER_WIDTH + SQUARE_WIDTH + MIDDLE_WIDTH;
const int LETTER_Y = (LCD_H - LETTER_HEIGHT) / 2;

struct Font {
    uint8_t width;
    uint8_t height;
    uint8_t pixels[36][3];
};

// print the content of a Font
void print(const struct Font *x) {
    printf("w: %u, h: %u\n", x->width, x->height);
    for (int i = 0; i < 36;) {
        printf("  %2d: [%u, %u, %u]", i,
               x->pixels[i][0], x->pixels[i][1], x->pixels[i][2]);
        ++i;
        printf("  %2d: [%u, %u, %u]", i,
               x->pixels[i][0], x->pixels[i][1], x->pixels[i][2]);
        ++i;
        printf("  %2d: [%u, %u, %u]\n", i,
               x->pixels[i][0], x->pixels[i][1], x->pixels[i][2]);
        ++i;
    }
}

int char2idx(int ch) {
    if (isdigit(ch)) {
        return ch - '0';
    } else if (isalpha(ch)) {
        return tolower(ch) - 'a' + 10;
    } else if (ch == '&') {
        return 10 + 26;
    } else {
        return 255;
    }
}

void drawFont(uint16_t x, uint16_t y, const struct Font *font) {
    for (int i = 0; i < 36; ++i) {
        int px = x + font->pixels[i][1] * FONT_POINT_SIZE;
        int py = y + font->pixels[i][2] * FONT_POINT_SIZE;
        uint16_t color = COLOR[font->pixels[i][0]];
        LCD_Fill(px, py, px + FONT_POINT_SIZE - 1, py + FONT_POINT_SIZE - 1, color);
    }
}

#include "font_data.h"

#if   defined(USE_4_DISCONNECTED)
const struct Font *fonts_letter = n4_disconnected_letter;
const struct Font *fonts_square = n4_disconnected_square;
const int FONT_SIZE = sizeof(n4_disconnected_letter) / sizeof(struct Font);
#elif defined(USE_2_DISCONNECTED)
const struct Font *fonts_letter = n2_disconnected_letter;
const struct Font *fonts_square = n2_disconnected_square;
const int FONT_SIZE = sizeof(n2_disconnected_letter) / sizeof(struct Font);
#elif defined(USE_3_CONNECTED)
const struct Font *fonts_letter = n3_connected_letter;
const struct Font *fonts_square = n3_connected_square;
const int FONT_SIZE = sizeof(n3_connected_letter) / sizeof(struct Font);
#else
const struct Font *fonts_letter = n2_connected_letter;
const struct Font *fonts_square = n2_connected_square;
const int FONT_SIZE = sizeof(n2_connected_letter) / sizeof(struct Font);
#endif
