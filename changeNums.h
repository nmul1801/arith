/*********************************************************************
 *                     changeNums.h
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose:  Used to edit and scale numbers whenever necessary in the
 *               program. Contains a function for converting to an even
 *               number, a function for clamping numbers, and a function for
 *               determining the maximum and minimum value for a given number
 *               of bits, both signed and unsigned
 *
 *********************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include <stdbool.h>
#include <math.h>

#ifndef CHANGE_NUMS
#define changeNums

/* Function: makeEven(int num)
 * Job: given a value, return the closest even number to that value
 * Expected input: an integer holding the number before being converted to
 * an even number
 * Expected output: an even integer
 * Expectations: the number is a positive integer
*/
int makeEven(int num);

/* Function: clampNum
 * Job: clamp a number to given range, meaning if it's more than the max,
 * convert it to the max value, and if it's less than the min, convert it to
 * the minimum value
 * Expected input: a void pointer representing the number before clamping, a
 * void pointer representing the maximum permitted value of the number, a
 * void pointer representing the minimum permitted value of the number, and a
 * boolean to determine whether the value is an integer or a float
 * Expected output: the value post-clamping
 * Expectations: none of the void pointers should be NULL
*/
void *clampNum(void *val, void *max, void *min, bool isInt);

/* Function: calculateMin
 * Job: calculate the minimum possible value for a signed or unsigned
 * int depending on width number of bits used to store it
 * Expected input: number of bits given to store a value, and a boolean that
 * determines if the integer is signed or unsigned
 * Expected output: integer value representing minimum value of a width of
 * given number of bits
 * Expectations: width is more than or equal to 0
*/
int calculateMin(int width, bool isSigned);

/* Function: calculateMax
 * Job: calculate the maximum possible value for a signed or unsigned
 * int depending on width number of bits used to store it
 * Expected input: number of bits given to store a value, and a boolean that
 * determines if the integer is signed or unsigned
 * Expected output: integer value representing maximum value of a width of
 * given number of bits
 * Expectations: width is more than or equal to 0
*/
int calculateMax(int width, bool isSigned);


#endif
