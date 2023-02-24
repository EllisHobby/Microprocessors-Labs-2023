#ifndef SEG7_H
#define SEG7_H

#define SEG_0   0x3f
#define SEG_1   0x06
#define SEG_2   0x5b
#define SEG_3   0x4f
#define SEG_4   0x66
#define SEG_5   0x6d
#define SEG_6   0x7d
#define SEG_7   0x07
#define SEG_8   0x7f
#define SEG_9   0x6f
#define SEG_10  ((SEG_1 << 8) | (SEG_0))
#define SEG_11  ((SEG_1 << 8) | (SEG_1))
#define SEG_12  ((SEG_1 << 8) | (SEG_2))
#define SEG_13  ((SEG_1 << 8) | (SEG_3))
#define SEG_14  ((SEG_1 << 8) | (SEG_4))
#define SEG_15  ((SEG_1 << 8) | (SEG_5))
#define SEG_16  ((SEG_1 << 8) | (SEG_6))
#define SEG_17  ((SEG_1 << 8) | (SEG_7))
#define SEG_18  ((SEG_1 << 8) | (SEG_8))
#define SEG_19  ((SEG_1 << 8) | (SEG_9))
#define SEG_20  ((SEG_2 << 8) | (SEG_0))
#define SEG_21  ((SEG_2 << 8) | (SEG_1))
#define SEG_22  ((SEG_2 << 8) | (SEG_2))
#define SEG_23  ((SEG_2 << 8) | (SEG_3))


const uint16_t Seg_Count[23] = { 
  SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, 
  SEG_6, SEG_7, SEG_8, SEG_9, SEG_10, 
  SEG_11, SEG_12, SEG_13, SEG_14, SEG_15,
  SEG_16, SEG_17, SEG_18, SEG_19, SEG_20,
  SEG_21, SEG_22, SEG_23
};

#endif