#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <bitset>
#include <math.h>
#include "typedefs.hpp"

using namespace std;

static const int MATRIX_SIZE = 64;
static const int DATA_WIDTH = 32;
static const int FIXED_POINT_WIDTH = 11;
static const int CHAR_LEN = 8;
static const int CHARS_AMOUNT = DATA_WIDTH / CHAR_LEN;
static const int RAM_SIZE = MATRIX_SIZE * MATRIX_SIZE * CHARS_AMOUNT;


static const int TOTAL_SIZE = W;//num of bits in num_t
static const int WHOLE_SIZE = I;//num of whole bits in num_t
static const int FRAC_SIZE = TOTAL_SIZE - WHOLE_SIZE;//num of fractional bits in num_t
//static const int CHAR_LEN = 8;//num of bits in char
static const int BUFF_SIZE = (int) std::ceil( (float)TOTAL_SIZE / CHAR_LEN);//how many chars are needed for num_t
static const int BUFF_EXTRA = BUFF_SIZE*CHAR_LEN - TOTAL_SIZE;//how many chars are extra

double to_fixed (unsigned char *);
void to_char (unsigned char *, string);
void to_uchar (unsigned char *buf, num_t d);
int to_int (unsigned char *);
void to_uchar (unsigned char *, int d);

int highest_bit(int n);
int setBitNumber(int n);

#endif // _UTILS_HPP_
