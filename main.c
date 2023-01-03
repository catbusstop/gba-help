#include "../bg.h"

#define MODE0 0x00
#define PALETTE_SIZE 256
#define DMA_16 0x00000000
#define DMA_ENABLE 0x80000000
#define BG0_ENABLE 0x100
#define SPRITE_ENABLE 0x1000
#define SPRITE_MAP_1D 0x40


volatile unsigned short* bg0_control = (volatile unsigned short*) 0x4000008;
volatile unsigned long* display_control = (volatile unsigned long*) 0x4000000;
volatile unsigned short* bg_palette = (volatile unsigned short*) 0x5000000;

/* the scanline counter is a memory cell which is updated to indicate how
 * much of the screen has been drawn */
volatile unsigned short* scanline_counter = (volatile unsigned short*) 0x4000006;

/* wait for the screen to be fully drawn so we can do something during vblank */
void wait_vblank() {
    while (*scanline_counter < 160) { }
}

/* return a pointer to one of the 4 character blocks (0-3) */
volatile unsigned short* char_block(unsigned long block) {
    return (volatile unsigned short*) (0x6000000 + (block * 0x4000)); // 16K each
}

/* return a pointer to one of the 32 screen blocks (0-31) */
volatile unsigned short* screen_block(unsigned long block) {
    return (volatile unsigned short*) (0x6000000 + (block * 0x800)); // 2K each
}

volatile unsigned int* dma_source = (volatile unsigned int*) 0x40000D4;
volatile unsigned int* dma_destination = (volatile unsigned int*) 0x40000D8;
volatile unsigned int* dma_count = (volatile unsigned int*) 0x40000DC;

/* copy data using DMA */
void memcpy16_dma(unsigned short* dest, unsigned short* source, int amount) {
    *dma_source = (unsigned int) source;
    *dma_destination = (unsigned int) dest;
    *dma_count = amount | DMA_16 | DMA_ENABLE;
}

/* function to setup background 0 for this program */
void setup_background() {
    memcpy16_dma((unsigned short*) bg_palette, (unsigned short*) bgPal, PALETTE_SIZE);
    memcpy16_dma((unsigned short*) char_block(0), (unsigned short*) bgTiles, (mapWidth * mapHeight) / 2);

    /* set all control the bits in this register */
    *bg0_control = 0 |    /* priority, 0 is highest, 3 is lowest */
        (0 << 2)  |       /* the char block the image data is stored in */
        (0 << 6)  |       /* the mosaic flag */
        (1 << 7)  |       /* color mode, 0 is 16 colors, 1 is 256 colors */
        (16 << 8) |       /* the screen block the tile data is stored in */
        (1 << 13) |       /* wrapping flag */
        (0 << 14);        /* bg size, 0 is 256x256 */

    memcpy16_dma((unsigned short*) screen_block(16), (unsigned short*) bgMap, mapWidth * mapHeight);
}

int main() {
    *display_control = MODE0 | BG0_ENABLE | SPRITE_ENABLE | SPRITE_MAP_1D;

    setup_background();

    while (1) {
        wait_vblank();
    }
}