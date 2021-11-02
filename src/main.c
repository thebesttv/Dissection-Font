#include "gd32vf103.h"
#include "lcd/lcd.h"

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

// choose only one of these
#define USE_4_DISCONNECTED
// #define USE_2_DISCONNECTED
// #define USE_3_CONNECTED
// #define USE_2_CONNECTED

#include "font.h"

#define RECORD_SIZE 100

void init_uart0() {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

void draw_letter_square(int idx) {
    if (idx < FONT_SIZE) {     // index in range
        // calculate letter's top-left coordinate
        const struct Font *font = fonts_letter + idx;
        int letter_x = LETTER_X + (12 - font->width) * FONT_POINT_SIZE / 2;
        int letter_y = LETTER_Y + (12 - font->height) * FONT_POINT_SIZE / 2;

        // clear screen, print both square & letter
        LCD_Clear(BLACK);
        drawFont(SQUARE_X, SQUARE_Y, fonts_square + idx);
        drawFont(letter_x, letter_y, font);
    } else { // index out of range, indicating non-displayable character
        LCD_Clear(BLACK);
    }
}

int main() {
    // init & clear LCD screen
    Lcd_Init();
    LCD_Clear(BLACK);
    BACK_COLOR = BLACK;

    // show startup message for 5s
    LCD_ShowString( 0,  0, (u8 *)"Dissection Font:",  WHITE);
    LCD_ShowString( 0, 16, (u8 *)"  4-piece discon-", WHITE);
    LCD_ShowString( 0, 32, (u8 *)"  nected font",     WHITE);
    LCD_ShowString(88, 64, (u8 *)"thebesttv",         WHITE);
    delay_1ms(5000);

    // init & start UART
    init_uart0();
    usart_data_receive(USART0);

    int record[RECORD_SIZE] = {
        char2idx('t'), char2idx('h'), char2idx('e'),
        char2idx('b'), char2idx('e'), char2idx('s'), char2idx('t'),
        char2idx('t'), char2idx('v'), char2idx(' '),
    };
    int record_tail = 10, record_idx = 0;
    uint8_t play_record = 1;
    while (1) {
        // if receives UART input
        if (RESET != usart_flag_get(USART0, USART_FLAG_RBNE)) {
            int x = usart_data_receive(USART0) & 0x7F;
            putchar(x); putchar('\n'); // echo back

            if (x == '\r') {
                if (play_record) {
                    record_idx = record_tail = 0;
                    LCD_Clear(BLACK);
                } else {
                    play_record = 1;
                }
            } else {
                if (play_record) {
                    play_record = 0;
                    record_idx = record_tail = 0;
                }
                int idx = char2idx(x);     // convert input character to font index
                draw_letter_square(idx);
                if (record_tail < RECORD_SIZE)
                    record[record_tail++] = idx;
            }
            // LCD_ShowNum(0, 0, x, 4, WHITE);
        }

        if (play_record && record_idx < record_tail) {
            draw_letter_square(record[record_idx]);
            record_idx = record_idx + 1 >= record_tail ? 0 : record_idx + 1;
            delay_1ms(500);
        }
    }
    return 0;
}

int _put_char(int ch) {
    usart_data_transmit(USART0, (uint8_t) ch);
    while (usart_flag_get(USART0, USART_FLAG_TBE)== RESET) {
    }
    return ch;
}
