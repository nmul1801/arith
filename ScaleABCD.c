/*********************************************************************
 *                     ScaleABCD.c
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: Implementation for ScaleABCD interface
 *              It includes implementation for functions used for converting
 *              values from their float values to their quantized integer
 *              values within a given range and vice versa.
 *
 *********************************************************************/

#include "ScaleABCD.h"
#include <stdio.h>

#define MAX_BCD_FLOAT 0.3
#define MIN_BCD_FLOAT -0.3

#define MAX_A_FLOAT 1.0
#define MIN_A_FLOAT 0.0

/* Purpose: scale the A float value to a scaled integer representation */
UnsignedInt scaleAToInt(float val, int width)
{
    /* calculate ranges for clamping */
    int max = calculateMax(width, false);
    int min = calculateMin(width, false);

    /* convert float value to scaled int value */
    UnsignedInt aVal = (UnsignedInt)round(val * max);

    /* clamp value so it is within range */
    aVal = *(UnsignedInt *)clampNum((void *)&aVal, (void *)&max, (void *)&min,
                                                                       true);

    return aVal;
}


 /* Purpose: scale the value of b, c, and d to a scaled int representation */
SignedInt scaleBCDToInt(float val, int width)
{
    /* calculate ranges for clamping */
    int max = calculateMax(width, true);
    int min = calculateMin(width, true);

    float scaler = (float) max / MAX_BCD_FLOAT;

    /* convert float value to scaled int value */
    SignedInt bcdVal = (SignedInt)round(val * scaler);

    /* clamp value so it is within range */

    bcdVal = *(SignedInt *)clampNum((void *)&bcdVal, (void *)&max,
                                                  (void *)&min, true);

    return bcdVal;
}


/* Purpose: scale the value of A to a floating point representation */
float intAtoFloat(UnsignedInt val, int width)
{
    /* calculate number to divide val by to convert it to a float */
    float scaler = calculateMax(width, false);

    /* convert val from unsigned int to float */
    float aVal = ((float)val / scaler);

    /* clamp it to the appropriate range */
    float max = MAX_A_FLOAT;
    float min = MIN_A_FLOAT;
    aVal = *(float *)clampNum((void *)&aVal, (void *)&max,
                                                  (void *)&min, false);

    return aVal;
}


/* Purpose: convert the scaled representation of b, c, or d to a float */
float intBCDtoFloat(SignedInt val, int width)
{
    /* calculate number to divide val by to convert it to a float */
    int maxIntVal = calculateMax(width, true);
    float scaler = (float) maxIntVal / MAX_BCD_FLOAT;

    /* convert val from signed int to float */
    float bcdVal = ((float)val / scaler);

    /* clamp it to the appropriate range */
    float max = MAX_BCD_FLOAT;
    float min = MIN_BCD_FLOAT;
    bcdVal = *(float *)clampNum((void *)&bcdVal, (void *)&max,
                                                  (void *)&min, false);

    return bcdVal;
}
