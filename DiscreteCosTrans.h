/*********************************************************************
 *                     DiscreteCosTrans.h
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: DiscreteCosTrans Interface
 *              Interface for functions used to perform discrete cosign
 *              and inverse discrete cosign transformation given a sequence
 *              of Comp Video structs or an array containing abcd
 *              (for inverse)
 *
 *********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include <seq.h>
#include "RGBtoFloats.h"
#include "RGBFloatsToCompVid.h"
#include </comp/40/build/include/arith40.h>
#include "changeNums.h"

#ifndef DISCRETECOS
#define DISCRETECOS

/* Function: calculateABCD(Seq_T componentVideos, float *abcd)
 * Job: calculate the values of a b c and d given a sequence of comp video
 * structs, and store them in an array of float values.
 * Expected input: sequence of 4 comp video structs, along with an empty
 * array of float values
 * Expected output: none, void
 * Expectations: componentVideos sequence is not null and abcd array is not
 * NULL
*/
void calculateABCD(Seq_T componentVideos, float *abcd);

/* Function: calculateYs(float *abcd, float *yValues)
 * Job: calculate the values of Y given the values of a, b, c and d. Store
 * the calculated values of Y in an array of 4 floats
 * Expected input: an array of float values representing the values for a, b,
 * c, and d, along with an empty array of 4 float values representing the
 * values of y
 * Expected output: none, void
 * Expectations: both abcd array and yValues array are not NULL
*/
void calculateYs(float *abcd, float *yValues);

#endif
