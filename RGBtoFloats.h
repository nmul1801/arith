/*********************************************************************
 *                    RGBToFloats.h
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: Interface for RGBToFloats
 *              Interface for functions used for converting
 *              values from their RGB values in Pnm_rgb structs to their
 *              floating point values in FloatingPoints structs and vice
 *              versa. Can convert sequences of Pnm_rgb structs and
 *              sequences of FloatingPoints structs into each other.
 *
 *********************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "pnm.h"
#include "changeNums.h"
#include <seq.h>
#include <stdbool.h>
#include </comp/40/build/include/arith40.h>


#define BLOCKSIZE 2
#define WORDSIZE 32
#define DENOMINATOR 255
#ifndef FLOATINGPOINTS
#define FLOATINGPOINTS

/*
 * struct FloatingPoints, set to represent the floating points
 * representation of each pixel
 */
typedef struct FloatingPoints {

    float red, green, blue;

} * FloatingPoints;

/*
 * Function: convertToFloat
 * Job: Given a sequence of Pnm_rgb struct pointers, convert the pixels to
 * a floating point representation, storing the floats in a sequence
 * Expected input: A sequence containing 4 Pnm_rgb struct pointers
 * Expected output: A sequence of FloatingPoints struct pointers
 * Expectations: Sequence of Pnm_rgb struct pointers is not NULL
*/
Seq_T convertToFloat(Seq_T fourPixels);

/*
 * Function: RGBToFloatingPts
 * Job: Given an instance of a Pnm_rgb struct pointer, convert it to a
 * floating point representation
 * Expected input: A single instance of a Pnm_rgb struct pointer
 * Expected output: A single instance of a FloatingPoints struct pointer
 * Expectations: The Pnm_rgb struct pointer is not NULL
*/
FloatingPoints RBGToFloatingPts(Pnm_rgb onePixel);

/*
 * Function: convertToScaledInts
 * Job: Given a sequence of FloatingPoints struct pointers, convert each
 * floating pointer representation to a scaled int representation, storing the
 * data in a Pnm_rgb struct pointer, then in a sequence
 * Expected input: A sequence of FloatingPoints struct pointers
 * Expected output: A sequence of Pnm_rgb struct pointers
 * Expectations: The FloatingPoints sequence was not NULL
*/
Seq_T convertToScaledInts(Seq_T fp_sequence);

/*
 * Function: floatsToScaledInts
 * Job: Convert a FloatingPoints struct pointer to a Pnm_rgb struct pointer
 * Expected input: A single instance of a FloatingPoints struct pointer
 * Expected output: A single instance of a Pnm_rgb struct pointer
 * Expectations: The FloatingPoints struct pointer is not NULL
*/
Pnm_rgb floatsToScaledInts(FloatingPoints RGBVals);


#endif
