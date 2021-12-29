/*********************************************************************
 *                     ScaleABCD.h
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: ScaleABCD Interface
 *              This interface is for converting values from their
 *              float values to their quantized integer values within a
 *              given range and vice versa.
 *
 *********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include <stdbool.h>
#include "RGBFloatsToCompVid.h"
#include "RGBtoFloats.h"
#include </comp/40/build/include/arith40.h>
#include "changeNums.h"
#include <math.h>
#include "DiscreteCosTrans.h"

typedef uint32_t UnsignedInt;
typedef int32_t SignedInt;

/* Function: scaleAToInt
 * Job: scale the value of a to a 9 bit representation
 * Expected input: float value representing a
 * Expected output: an unsigned int representing a
 * Expectations: val is within the range 0 to 1
*/
UnsignedInt scaleAToInt(float val, int width);

/* Function: scaleBCDToInt
 * Job: scale the value of b, c, and d to a 5 bit representation
 * Expected input: float value representing b, c, or d
 * Expected output: a signed int representing b, c, or d
 * Expectations: val is within the range -0.3, 0.3
*/
SignedInt scaleBCDToInt(float val, int width);

/* Function: intAtoFloat
 * Job: convert the 9 bit representation of a to be in a range of 0 to 1
 * Expected input: integer value representing a
 * Expected output: float value representing a
 * Expectations: value is less than or equal to 511
*/
float intAtoFloat(UnsignedInt val, int width);

/* Function: intBCDtoFloat
 * Job: convert the 5 bit representation of b, c, or d to a float
 * Expected input: a signed integer representing b, c, or d
 * Expected output: a float representing b, c, or d
 * Expectations: value is within the range -15, 15
*/
float intBCDtoFloat(SignedInt val, int width);
