/*********************************************************************
 *                     RGBFloatsToCompVid.h
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: RGBFloatsToCompVid Interface
 *              This interface is for converting data from a
 *              component video color space representation to a
 *              floating point representation and back. It can convert
 *              sequences of CompVideo structs and sequences of
 *              FloatingPoints structs into each other.
 *********************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include <seq.h>
#include "RGBtoFloats.h"
#include </comp/40/build/include/arith40.h>
#include "changeNums.h"

#define WORDSIZE 32
#define DENOMINATOR 255
#define BLOCKSIZE 2

#ifndef COMPVID
#define COMPVID

/*
 * struct CompVideo, set to hold the data for the component video color
 * space representation
 */
typedef struct CompVideo {

    float Y, Pb, Pr;

} * CompVideo;


/*
 * Function: initCompVidStruct
 * Job: create a new CompVideo struct on the heap
 * Expected input: all data necessary to create a new Comp Video struct
 * Expected output: A pointer to a new Comp Video struct
 * Expectations: All data is within the clamped range
*/
CompVideo initCompVidStruct(float yVal, float pbVal, float prVal);


/*
 * Function: convertToCompVideo
 * Job: Given a sequence of floating point values, create a new sequence of
 * Comp Video structs
 * Expected input: A sequence of FloatingPoints structs
 * Expected output: A sequence of CompVideo structs
 * Expectations: The given sequence is not NULL
*/
Seq_T convertToCompVideo(Seq_T floatingPoints);


/*
 * Function: floatingPt_to_compV
 * Job: Given a FloatingPoints struct, convert the data to
 * a CompVideo struct
 * Expected input: A FloatingPoints struct
 * Expected output: A CompVideo struct pointer
 * Expectations: The pixel is not NULL
*/
CompVideo floatingPt_to_compV(FloatingPoints pixel);


/*
 * Function: convertToRGBFloats
 * Job: Given a sequence of CompVideo structs, convert the sequence
 * to a sequence of FloatingPoints structs
 * Expected input: A sequence of CompVideo struct pointers
 * Expected output: A sequence of FloatingPoints structs
 * Expectations: The sequence is not NULL
*/
Seq_T convertToRGBFloats(Seq_T compVideoSeq);


/*
 * Function: compVidToRGBFloats
 * Job: Given a CompVideo struct, convert it to a floating point
 * representation, by converting it to a FloatingPoints struct
 * Expected input: A CompVideo struct pointer
 * Expected output: A FloatingPoints struct
 * Expectations: compVideoStruct is not NULL
*/
FloatingPoints compVidToRGBFloats(CompVideo compVideoStruct);

#endif
