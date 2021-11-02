// convert 24 bit color to 16 bit color
// https://www.lucasgaland.com/24-bit-16-bit-color-converter-tool-for-embedded-lcd-guis/
#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.unsetf(std::ios::dec);
    cin.unsetf(std::ios::hex);
    cin.unsetf(std::ios::oct);
    while (true) {
        int c24;
        cin >> c24;

        int r8 = c24 >> 16;
        int g8 = (c24 >> 8) & 0xFF;
        int b8 = c24 & 0xFF;

        int r5 = (r8 * 249 + 1014) >> 11;
        int g6 = (g8 * 253 + 505) >> 10;
        int b5 = (b8 * 249 + 1014) >> 11;

        int res = (r5 << 11) | (g6 << 5) | b5;
        printf("0x%4X\n", res);
    }
    return 0;
}
