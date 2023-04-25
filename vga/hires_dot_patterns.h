#pragma once

#include "common/config.h"
#include <stdint.h>

// Mapping of a hires video byte to 14 half-pixel dots.
// Bits are displayed from MSB to LSB.
static uint16_t DELAYED_COPY_DATA(hires_dot_patterns)[256] = {
    0x0000,0x3000,0x0c00,0x3c00,0x0300,0x3300,0x0f00,0x3f00,
    0x00c0,0x30c0,0x0cc0,0x3cc0,0x03c0,0x33c0,0x0fc0,0x3fc0,
    0x0030,0x3030,0x0c30,0x3c30,0x0330,0x3330,0x0f30,0x3f30,
    0x00f0,0x30f0,0x0cf0,0x3cf0,0x03f0,0x33f0,0x0ff0,0x3ff0,
    0x000c,0x300c,0x0c0c,0x3c0c,0x030c,0x330c,0x0f0c,0x3f0c,
    0x00cc,0x30cc,0x0ccc,0x3ccc,0x03cc,0x33cc,0x0fcc,0x3fcc,
    0x003c,0x303c,0x0c3c,0x3c3c,0x033c,0x333c,0x0f3c,0x3f3c,
    0x00fc,0x30fc,0x0cfc,0x3cfc,0x03fc,0x33fc,0x0ffc,0x3ffc,
    0x0003,0x3003,0x0c03,0x3c03,0x0303,0x3303,0x0f03,0x3f03,
    0x00c3,0x30c3,0x0cc3,0x3cc3,0x03c3,0x33c3,0x0fc3,0x3fc3,
    0x0033,0x3033,0x0c33,0x3c33,0x0333,0x3333,0x0f33,0x3f33,
    0x00f3,0x30f3,0x0cf3,0x3cf3,0x03f3,0x33f3,0x0ff3,0x3ff3,
    0x000f,0x300f,0x0c0f,0x3c0f,0x030f,0x330f,0x0f0f,0x3f0f,
    0x00cf,0x30cf,0x0ccf,0x3ccf,0x03cf,0x33cf,0x0fcf,0x3fcf,
    0x003f,0x303f,0x0c3f,0x3c3f,0x033f,0x333f,0x0f3f,0x3f3f,
    0x00ff,0x30ff,0x0cff,0x3cff,0x03ff,0x33ff,0x0fff,0x3fff,
    0x0000,0x1800,0x0600,0x1e00,0x0180,0x1980,0x0780,0x1f80,
    0x0060,0x1860,0x0660,0x1e60,0x01e0,0x19e0,0x07e0,0x1fe0,
    0x0018,0x1818,0x0618,0x1e18,0x0198,0x1998,0x0798,0x1f98,
    0x0078,0x1878,0x0678,0x1e78,0x01f8,0x19f8,0x07f8,0x1ff8,
    0x0006,0x1806,0x0606,0x1e06,0x0186,0x1986,0x0786,0x1f86,
    0x0066,0x1866,0x0666,0x1e66,0x01e6,0x19e6,0x07e6,0x1fe6,
    0x001e,0x181e,0x061e,0x1e1e,0x019e,0x199e,0x079e,0x1f9e,
    0x007e,0x187e,0x067e,0x1e7e,0x01fe,0x19fe,0x07fe,0x1ffe,
    0x0001,0x1801,0x0601,0x1e01,0x0181,0x1981,0x0781,0x1f81,
    0x0061,0x1861,0x0661,0x1e61,0x01e1,0x19e1,0x07e1,0x1fe1,
    0x0019,0x1819,0x0619,0x1e19,0x0199,0x1999,0x0799,0x1f99,
    0x0079,0x1879,0x0679,0x1e79,0x01f9,0x19f9,0x07f9,0x1ff9,
    0x0007,0x1807,0x0607,0x1e07,0x0187,0x1987,0x0787,0x1f87,
    0x0067,0x1867,0x0667,0x1e67,0x01e7,0x19e7,0x07e7,0x1fe7,
    0x001f,0x181f,0x061f,0x1e1f,0x019f,0x199f,0x079f,0x1f9f,
    0x007f,0x187f,0x067f,0x1e7f,0x01ff,0x19ff,0x07ff,0x1fff,
};

static uint16_t DELAYED_COPY_DATA(hires_dot_patterns2)[512] = {
    0b0000000000000000,0b0000000000000011,0b0000000000001100,0b0000000000001111,0b0000000000110000,0b0000000000110011,0b0000000000111100,0b0000000000111111,
    0b0000000011000000,0b0000000011000011,0b0000000011001100,0b0000000011001111,0b0000000011110000,0b0000000011110011,0b0000000011111100,0b0000000011111111,
    0b0000001100000000,0b0000001100000011,0b0000001100001100,0b0000001100001111,0b0000001100110000,0b0000001100110011,0b0000001100111100,0b0000001100111111,
    0b0000001111000000,0b0000001111000011,0b0000001111001100,0b0000001111001111,0b0000001111110000,0b0000001111110011,0b0000001111111100,0b0000001111111111,
    0b0000110000000000,0b0000110000000011,0b0000110000001100,0b0000110000001111,0b0000110000110000,0b0000110000110011,0b0000110000111100,0b0000110000111111,
    0b0000110011000000,0b0000110011000011,0b0000110011001100,0b0000110011001111,0b0000110011110000,0b0000110011110011,0b0000110011111100,0b0000110011111111,
    0b0000111100000000,0b0000111100000011,0b0000111100001100,0b0000111100001111,0b0000111100110000,0b0000111100110011,0b0000111100111100,0b0000111100111111,
    0b0000111111000000,0b0000111111000011,0b0000111111001100,0b0000111111001111,0b0000111111110000,0b0000111111110011,0b0000111111111100,0b0000111111111111,
    0b0011000000000000,0b0011000000000011,0b0011000000001100,0b0011000000001111,0b0011000000110000,0b0011000000110011,0b0011000000111100,0b0011000000111111,
    0b0011000011000000,0b0011000011000011,0b0011000011001100,0b0011000011001111,0b0011000011110000,0b0011000011110011,0b0011000011111100,0b0011000011111111,
    0b0011001100000000,0b0011001100000011,0b0011001100001100,0b0011001100001111,0b0011001100110000,0b0011001100110011,0b0011001100111100,0b0011001100111111,
    0b0011001111000000,0b0011001111000011,0b0011001111001100,0b0011001111001111,0b0011001111110000,0b0011001111110011,0b0011001111111100,0b0011001111111111,
    0b0011110000000000,0b0011110000000011,0b0011110000001100,0b0011110000001111,0b0011110000110000,0b0011110000110011,0b0011110000111100,0b0011110000111111,
    0b0011110011000000,0b0011110011000011,0b0011110011001100,0b0011110011001111,0b0011110011110000,0b0011110011110011,0b0011110011111100,0b0011110011111111,
    0b0011111100000000,0b0011111100000011,0b0011111100001100,0b0011111100001111,0b0011111100110000,0b0011111100110011,0b0011111100111100,0b0011111100111111,
    0b0011111111000000,0b0011111111000011,0b0011111111001100,0b0011111111001111,0b0011111111110000,0b0011111111110011,0b0011111111111100,0b0011111111111111,
    0b0000000000000000,0b0000000000000110,0b0000000000011000,0b0000000000011110,0b0000000001100000,0b0000000001100110,0b0000000001111000,0b0000000001111110,
    0b0000000110000000,0b0000000110000110,0b0000000110011000,0b0000000110011110,0b0000000111100000,0b0000000111100110,0b0000000111111000,0b0000000111111110,
    0b0000011000000000,0b0000011000000110,0b0000011000011000,0b0000011000011110,0b0000011001100000,0b0000011001100110,0b0000011001111000,0b0000011001111110,
    0b0000011110000000,0b0000011110000110,0b0000011110011000,0b0000011110011110,0b0000011111100000,0b0000011111100110,0b0000011111111000,0b0000011111111110,
    0b0001100000000000,0b0001100000000110,0b0001100000011000,0b0001100000011110,0b0001100001100000,0b0001100001100110,0b0001100001111000,0b0001100001111110,
    0b0001100110000000,0b0001100110000110,0b0001100110011000,0b0001100110011110,0b0001100111100000,0b0001100111100110,0b0001100111111000,0b0001100111111110,
    0b0001111000000000,0b0001111000000110,0b0001111000011000,0b0001111000011110,0b0001111001100000,0b0001111001100110,0b0001111001111000,0b0001111001111110,
    0b0001111110000000,0b0001111110000110,0b0001111110011000,0b0001111110011110,0b0001111111100000,0b0001111111100110,0b0001111111111000,0b0001111111111110,
    0b0110000000000000,0b0110000000000110,0b0110000000011000,0b0110000000011110,0b0110000001100000,0b0110000001100110,0b0110000001111000,0b0110000001111110,
    0b0110000110000000,0b0110000110000110,0b0110000110011000,0b0110000110011110,0b0110000111100000,0b0110000111100110,0b0110000111111000,0b0110000111111110,
    0b0110011000000000,0b0110011000000110,0b0110011000011000,0b0110011000011110,0b0110011001100000,0b0110011001100110,0b0110011001111000,0b0110011001111110,
    0b0110011110000000,0b0110011110000110,0b0110011110011000,0b0110011110011110,0b0110011111100000,0b0110011111100110,0b0110011111111000,0b0110011111111110,
    0b0111100000000000,0b0111100000000110,0b0111100000011000,0b0111100000011110,0b0111100001100000,0b0111100001100110,0b0111100001111000,0b0111100001111110,
    0b0111100110000000,0b0111100110000110,0b0111100110011000,0b0111100110011110,0b0111100111100000,0b0111100111100110,0b0111100111111000,0b0111100111111110,
    0b0111111000000000,0b0111111000000110,0b0111111000011000,0b0111111000011110,0b0111111001100000,0b0111111001100110,0b0111111001111000,0b0111111001111110,
    0b0111111110000000,0b0111111110000110,0b0111111110011000,0b0111111110011110,0b0111111111100000,0b0111111111100110,0b0111111111111000,0b0111111111111110,
    0b0000000000000000,0b0000000000000011,0b0000000000001100,0b0000000000001111,0b0000000000110000,0b0000000000110011,0b0000000000111100,0b0000000000111111,
    0b0000000011000000,0b0000000011000011,0b0000000011001100,0b0000000011001111,0b0000000011110000,0b0000000011110011,0b0000000011111100,0b0000000011111111,
    0b0000001100000000,0b0000001100000011,0b0000001100001100,0b0000001100001111,0b0000001100110000,0b0000001100110011,0b0000001100111100,0b0000001100111111,
    0b0000001111000000,0b0000001111000011,0b0000001111001100,0b0000001111001111,0b0000001111110000,0b0000001111110011,0b0000001111111100,0b0000001111111111,
    0b0000110000000000,0b0000110000000011,0b0000110000001100,0b0000110000001111,0b0000110000110000,0b0000110000110011,0b0000110000111100,0b0000110000111111,
    0b0000110011000000,0b0000110011000011,0b0000110011001100,0b0000110011001111,0b0000110011110000,0b0000110011110011,0b0000110011111100,0b0000110011111111,
    0b0000111100000000,0b0000111100000011,0b0000111100001100,0b0000111100001111,0b0000111100110000,0b0000111100110011,0b0000111100111100,0b0000111100111111,
    0b0000111111000000,0b0000111111000011,0b0000111111001100,0b0000111111001111,0b0000111111110000,0b0000111111110011,0b0000111111111100,0b0000111111111111,
    0b0011000000000000,0b0011000000000011,0b0011000000001100,0b0011000000001111,0b0011000000110000,0b0011000000110011,0b0011000000111100,0b0011000000111111,
    0b0011000011000000,0b0011000011000011,0b0011000011001100,0b0011000011001111,0b0011000011110000,0b0011000011110011,0b0011000011111100,0b0011000011111111,
    0b0011001100000000,0b0011001100000011,0b0011001100001100,0b0011001100001111,0b0011001100110000,0b0011001100110011,0b0011001100111100,0b0011001100111111,
    0b0011001111000000,0b0011001111000011,0b0011001111001100,0b0011001111001111,0b0011001111110000,0b0011001111110011,0b0011001111111100,0b0011001111111111,
    0b0011110000000000,0b0011110000000011,0b0011110000001100,0b0011110000001111,0b0011110000110000,0b0011110000110011,0b0011110000111100,0b0011110000111111,
    0b0011110011000000,0b0011110011000011,0b0011110011001100,0b0011110011001111,0b0011110011110000,0b0011110011110011,0b0011110011111100,0b0011110011111111,
    0b0011111100000000,0b0011111100000011,0b0011111100001100,0b0011111100001111,0b0011111100110000,0b0011111100110011,0b0011111100111100,0b0011111100111111,
    0b0011111111000000,0b0011111111000011,0b0011111111001100,0b0011111111001111,0b0011111111110000,0b0011111111110011,0b0011111111111100,0b0011111111111111,
    0b0000000000000001,0b0000000000000111,0b0000000000011001,0b0000000000011111,0b0000000001100001,0b0000000001100111,0b0000000001111001,0b0000000001111111,
    0b0000000110000001,0b0000000110000111,0b0000000110011001,0b0000000110011111,0b0000000111100001,0b0000000111100111,0b0000000111111001,0b0000000111111111,
    0b0000011000000001,0b0000011000000111,0b0000011000011001,0b0000011000011111,0b0000011001100001,0b0000011001100111,0b0000011001111001,0b0000011001111111,
    0b0000011110000001,0b0000011110000111,0b0000011110011001,0b0000011110011111,0b0000011111100001,0b0000011111100111,0b0000011111111001,0b0000011111111111,
    0b0001100000000001,0b0001100000000111,0b0001100000011001,0b0001100000011111,0b0001100001100001,0b0001100001100111,0b0001100001111001,0b0001100001111111,
    0b0001100110000001,0b0001100110000111,0b0001100110011001,0b0001100110011111,0b0001100111100001,0b0001100111100111,0b0001100111111001,0b0001100111111111,
    0b0001111000000001,0b0001111000000111,0b0001111000011001,0b0001111000011111,0b0001111001100001,0b0001111001100111,0b0001111001111001,0b0001111001111111,
    0b0001111110000001,0b0001111110000111,0b0001111110011001,0b0001111110011111,0b0001111111100001,0b0001111111100111,0b0001111111111001,0b0001111111111111,
    0b0110000000000001,0b0110000000000111,0b0110000000011001,0b0110000000011111,0b0110000001100001,0b0110000001100111,0b0110000001111001,0b0110000001111111,
    0b0110000110000001,0b0110000110000111,0b0110000110011001,0b0110000110011111,0b0110000111100001,0b0110000111100111,0b0110000111111001,0b0110000111111111,
    0b0110011000000001,0b0110011000000111,0b0110011000011001,0b0110011000011111,0b0110011001100001,0b0110011001100111,0b0110011001111001,0b0110011001111111,
    0b0110011110000001,0b0110011110000111,0b0110011110011001,0b0110011110011111,0b0110011111100001,0b0110011111100111,0b0110011111111001,0b0110011111111111,
    0b0111100000000001,0b0111100000000111,0b0111100000011001,0b0111100000011111,0b0111100001100001,0b0111100001100111,0b0111100001111001,0b0111100001111111,
    0b0111100110000001,0b0111100110000111,0b0111100110011001,0b0111100110011111,0b0111100111100001,0b0111100111100111,0b0111100111111001,0b0111100111111111,
    0b0111111000000001,0b0111111000000111,0b0111111000011001,0b0111111000011111,0b0111111001100001,0b0111111001100111,0b0111111001111001,0b0111111001111111,
    0b0111111110000001,0b0111111110000111,0b0111111110011001,0b0111111110011111,0b0111111111100001,0b0111111111100111,0b0111111111111001,0b0111111111111111,
};
