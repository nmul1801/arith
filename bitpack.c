/*********************************************************************
 *                     bitpack.c
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: bitpack Interface, allows the client to pack bit values into
 *              64 bit integers, as well as retrive them, and make sure the
 *              values fit within a given width
 *
 *********************************************************************/
#ifndef BITPACK_INCLUDED
#define BITPACK_INCLUDED
#include <stdbool.h>
#include <stdint.h>
#include "except.h"
#include <assert.h>
#include <math.h>
#include "bitpack.h"
#include <stdio.h>

#define MAXW 64

Except_T Bitpack_Overflow = {"Overflow packing bits"};


/* Function: left_shift_u
 * Job: left shift the given unsigned int by shiftVal amount of times
 * Expected input: 2 unsigned ints, one holding the value to be shifted,
 * and the other representing the amount it will be shifted by
 * Expected output: unsigned int representing the left shifted number
 * Expectations: shift value is less than or equal to the max width
*/
uint64_t left_shift_u(uint64_t n, unsigned shiftVal)
{
    assert(shiftVal <= MAXW);

    /* don't shift MAXW */
    if (shiftVal == MAXW) {
        return 0;
    }

    return n << shiftVal;
}


/* Function: right_shift_u
 * Job: right shift the given unsigned int by shiftVal amount of times
 * Expected input: 2 unsigned ints, one holding the value to be shifted,
 * and the other representing the amount it will be shifted by
 * Expected output: unsigned int representing the right shifted number
 * Expectations: shift value is less than or equal to the max width
*/
uint64_t right_shift_u(uint64_t n, unsigned shiftVal)
{
    assert(shiftVal <= MAXW);

    /* don't shift MAXW */
    if (shiftVal == MAXW) {
        return 0;
    }

    return n >> shiftVal;
}


/* Function: left_shift_s
 * Job: left shift the given signed int by shiftVal amount of times
 * Expected input: a signed ints holding the value to be shifted, and
 * an unsigned int representing the amount it will be shifted by
 * Expected output: signed int representing the left shifted number
 * Expectations: shift value is less than or equal to the max width
*/
int64_t left_shift_s(int64_t n, unsigned shiftVal)
{
    assert(shiftVal <= MAXW);

    /* don't shift MAXW */
    if (shiftVal == MAXW) {
        return 0;
    }

    return n << shiftVal;
}


/* Function: right_shift_s
 * Job: right shift the given signed int by shiftVal amount of times
 * Expected input: a signed ints holding the value to be shifted, and
 * an unsigned int representing the amount it will be shifted by
 * Expected output: signed int representing the right shifted number
 * Expectations: shift value is less than or equal to the max width
*/
int64_t right_shift_s(int64_t n, unsigned shiftVal)
{
    assert(shiftVal <= MAXW);

    /* don't shift MAXW */
    if (shiftVal == MAXW) {
        return 0;
    }

    return n >> shiftVal;
}


/* Function: Bitpack_fitsu
 * Job: check if an unsigned int n can be represented in width number
 * of bits
 * Expected input: 2 unsigned ints, one holding the value being
 * represented and the other holding the number of bits that will be
 * used to represent the value (if it fits)
 * Expected output: a boolean that is true if n can be represented in
 * width number of bits and false otherwise
 * Expectations: width is less than or equal to max width
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= MAXW);

    /* if width is 0 and n is not 0, n doesn't fit */
    if (width == 0) {
        return (n == 0);
    }

    /* if there are leftover bits after right shift -> bit don't fit (include
     * edge case for 0)
     */
    return !(right_shift_u(n, width) != 0 || width == 0);
}


/* Function: Bitpack_fitss
 * Job: check if an signed int n can be represented in width number
 * of bits
 * Expected input: a signed int representing a value and an unsigned
 * int representing the number of bits that will be used to represent the
 * value (if it fits)
 * Expected output: a boolean that is true if n can be represented in
 * width number of bits and false otherwise
 * Expectations: width is less than or equal to max width
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= MAXW);

    /* if width is 0 and n is not 0, n doesn't fit */
    if (width == 0) {
        return (n == 0);
    }

    /* all values right of msb to 0 gets the min */
    int64_t min = left_shift_s(~0, width - 1);
    /* flipping the min gets the mask */
    int64_t max = ~min;

    return n <= max && n >= min;
}


/* Function: Bitpack_getu
 * Job: given the width and least significant bit of a value, extract the
 * unsigned int value from an unsigned int (word)
 * Expected input: an unsigned int representing a word, an unsigned
 * int representing the number of bits that will be used to represent a
 * value within the word, and an unsigned int representing the least
 * signficant bit of the value
 * Expected output: an unsigned int representing the value extracted from
 * the word
 * Expectations: width is less than or equal to max width, lsb is less than
 * the max width, and the width + lsb is less than or equal to max width
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= MAXW && lsb < MAXW);
    assert(width + lsb <= MAXW);

    /* shift target bits all the way to the left, right fill w/ 0s */
    uint64_t num = left_shift_u(word, MAXW - width - lsb);

    /* shift target bits all the way to the right, left fill w/ 0s */
    return right_shift_u(num, MAXW - width);
}


/* Function: Bitpack_gets
 * Job: given the width and least significant bit of a value, extract the
 * signed int value from an unsigned int (word)
 * Expected input: an unsigned int representing a word, an unsigned
 * int representing the number of bits that will be used to represent a
 * value within the word, and an unsigned int representing the least
 * signficant bit of the value
 * Expected output: a signed int representing the value extracted from
 * the word
 * Expectations: width is less than or equal to max width, lsb is less than
 * the max width, and the width + lsb is less than or equal to max width
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= MAXW && lsb < MAXW);
    assert(width + lsb <= MAXW);

    /* shift target bits all the way to the left, right fill w/ 0s */
    int64_t shifted = left_shift_s(word, MAXW - width - lsb);

    /* left fill with 0s or 1s depending on whether the numeber is neg/pos */
    return right_shift_s(shifted, MAXW - width);
}


/* Function: Bitpack_newu
 * Job: given a unsigned int word, an unsigned value, and the value's
 * location and width within the word, return a new unsigned int word that
 * stores the value in its given location in the word
 * Expected input: an unsigned int representing a word, an unsigned int
 * representing a value, an unsigned int representing the number of bits
 * used to represent the value, and an unsigned int representing the least
 * signficant bit of the value
 * Expected output: an unsigned int representing the word where the field
 * has been changed to hold the new value
 * Expectations: width is less than or equal to max width, lsb is less than
 * the max width, and the width + lsb is less than or equal to max width,
 * the value can be represented in width number of bits
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                                                     uint64_t value)
{
    assert(width <= MAXW && lsb < MAXW);
    assert (width + lsb <= MAXW);

    /* if value doesn't fit within width, raise exception */
    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);
    }

    /* position the value appropriately */
    uint64_t newVal = left_shift_u(value, lsb);

    /* create a mask so 1's are positioned in the field, rest 0's*/
    uint64_t onesMask = left_shift_u(~0, MAXW - width);
    onesMask = right_shift_u(onesMask, MAXW - width - lsb);

    /* flip mask so everything is 1's besides the field */
    onesMask = ~onesMask;

    /* clear the spot for value, update the field */
    uint64_t mask = onesMask & word;

    return mask | newVal;
}


/* Function: Bitpack_news
 * Job: given a unsigned int word, a signed value, and the value's location
 * and width within the word, return a new unsigned int word that stores
 * the value in its given location in the word
 * Expected input: an unsigned int representing a word, a signed int
 * representing a value, an unsigned int representing the number of bits
 * used to represent the value, and an unsigned int representing the least
 * signficant bit of the value
 * Expected output: an unsigned int representing the word after the field
 * has been changed to hold the new value
 * Expectations: width is less than or equal to max width, lsb is less than
 * the max width, and the width + lsb is less than or equal to max width,
 * the value can be represented in width number of bits
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                                                     int64_t value)
{
    /* error check */
    assert(width <= MAXW && lsb < MAXW);
    assert (width + lsb <= MAXW);

    /* if value doesn't fit within width, raise exception */
    if (!Bitpack_fitss(value, width)) {
        RAISE(Bitpack_Overflow);
    }

    /* isolate the value by using a mask of 1s with the & operation */
    uint64_t valueMask = right_shift_u(~0, MAXW - width);
    uint64_t newVal = valueMask & value;
    newVal = left_shift_s(newVal, lsb);

    /* create another mask of 1s positioned within the field*/
    uint64_t onesMask = left_shift_u(~0, MAXW - width);
    onesMask = right_shift_u(onesMask, MAXW - width - lsb);

    /* flip the onesMask so the field being updated is zeroed out */
    onesMask = ~onesMask;

    /* clear the field, then update it */
    uint64_t mask = onesMask & word;

    return mask | newVal;
}

#endif
