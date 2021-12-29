/*********************************************************************
 *                     changeNums.c
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: Implementation for changeNums
 *              Includes implementation for functions used to edit and scale
 *              numbers whenever necessary in the program. Contains a function
 *              for converting to an even number, a function for clamping
 *              numbers, and a function for determining the maximum and
 *              minimum value for a given number of bits, both signed and
 *              unsigned
 *
 *********************************************************************/

#include "changeNums.h"


/*
 * Purpose: Convert a number to its closest even representation, rounding
 * down
 */
int makeEven(int num)
{
    assert(num >= 0);

    /* if the number is even, no problem */
    if (num % 2 == 0){
        return num;
    }
    /* Otherwise, round down */
    return num - 1;
}



/* Purpose: Clamp a number within a given range */
void *clampNum(void *val, void *max, void *min, bool isInt)
{
    assert(val != NULL && max != NULL && min != NULL);

    /* different representation for whether the value is an integer or not */
    if (isInt) {
        /* retrieve the values */
        int *value = (int *)val;
        int *maxVal = (int *)max;
        int *minVal = (int *)min;

        /* clamp the value */
        if (*value > *maxVal) {
            return (void *)maxVal;
        } else if (*value < *minVal) {

            return (void *)minVal;
        }

        /* if the value doesn't need to be clamped, return it*/
        return (void *)value;

    /* if value is a float  */
    } else {
        /* retrieve the values */
        float *value = (float *)val;
        float *maxVal = (float *)max;
        float *minVal = (float *)min;

        /* clamp the value */
        if (*value > *maxVal){
            return (void *)maxVal;
        } else if (*value < *minVal) {
            return (void *)minVal;
        }

        /* if the value doesn't need to be clamped, return it*/
        return (void *)value;
    }

}



/*
 * Purpose: calculate the minimum possible value for a signed or unsigned
 * int depending on width number of bits used to store it
 */
int calculateMin(int width, bool isSigned)
{
    /* unsigned is at least 0 */
    if (!isSigned) {
        return 0;
    }

    /* mirror the maximum value */
    return -1 * calculateMax(width, true);
}


/*
* Purpose: calculate the maximum possible value for a signed or unsigned
* int depending on width number of bits used to store it
 */
int calculateMax(int width, bool isSigned)
{
    if (!isSigned) {
        return (pow(2, width) - 1);
    }

    return (pow(2, width - 1) - 1);
}
