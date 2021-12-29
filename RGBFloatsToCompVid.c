/*********************************************************************
 *                     RGBFloatsToCompVid.c
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: RGBFloatsToCompVid Implementation
 *              This is the implementation file for RGBFloatsToCompVid.h.
 *              It includes functions used for converting data from a
 *              component video color space representation to a
 *              floating point representation and back. It can convert
 *              sequences of CompVideo structs and sequences of
 *              FloatingPoints structs into each other.
 *
 *********************************************************************/

#include "RGBFloatsToCompVid.h"

 /* Purpose: create a new CompVideo struct on the heap */
CompVideo initCompVidStruct(float yVal, float pbVal, float prVal)
{
    /* allocate memory */
    CompVideo newCompVid = malloc(sizeof(*newCompVid));
    assert(newCompVid != NULL);

    /* store the data */
    newCompVid -> Y = yVal;
    newCompVid -> Pb = pbVal;
    newCompVid -> Pr = prVal;

    return newCompVid;
}


/*
 * Purpose: given a sequence of floating point values, create a new sequence
 * of CompVideo structs
 */
Seq_T convertToCompVideo(Seq_T floatingPoints)
{
    assert(floatingPoints != NULL);

    /* create a new sequence of CompVideo structs */
    Seq_T compVideoSeq = Seq_new(BLOCKSIZE * BLOCKSIZE);
    assert(compVideoSeq != NULL);

    /* change all floating point representations to CompVideo structs */
    for (int i = 0; i < Seq_length(floatingPoints); i++) {
        CompVideo newCompVid = floatingPt_to_compV(Seq_get(floatingPoints,
                                                                      i));
        assert(newCompVid != NULL);

        Seq_addhi(compVideoSeq, newCompVid);
    }

    return compVideoSeq;
}


/*
 * Purpose: given a FloatingPoints struct, convert the data to
 * a CompVideo struct
 */
CompVideo floatingPt_to_compV(FloatingPoints pixel)
{
    assert(pixel != NULL);

    /* retrieve the values */
    float r = pixel -> red;
    float g = pixel -> green;
    float b = pixel -> blue;

    /* calculate data for comp vid struct */
    float yVal = (0.299 * r) + (0.587 * g) + (0.114 * b);
    float pbVal = (-0.168736 * r) - (0.331264 * g) + (0.5 * b);
    float prVal = (0.5 * r) - (0.418688 * g) - (0.081312 * b);

    /* set ranges for values for clamping */
    float Ymax = 1.0;
    float Ymin = 0.0;
    float Pmax = 0.5;
    float Pmin = -0.5;

    /* clamp Y, Pr, and Pb values so they are within the appropriate range */
    yVal = *((float *)clampNum((void *)&yVal, (void *)&Ymax,
                                              (void *)&Ymin, false));
    pbVal = *((float *)clampNum((void *)&pbVal, (void *)&Pmax,
                                                (void *)&Pmin, false));
    prVal = *((float *)clampNum((void *)&prVal, (void *)&Pmax,
                                                (void *)&Pmin, false));

    return initCompVidStruct(yVal, pbVal, prVal);
}


/*
 * Purpose: given a sequence of CompVideo structs, convert the sequence
 * to a sequence of FloatingPoints structs
 */
Seq_T convertToRGBFloats(Seq_T compVideoSeq)
{
    assert(compVideoSeq != NULL);

    /* create a new sequence of FloatingPoints structs */
    Seq_T floatPointSeq = Seq_new(BLOCKSIZE * BLOCKSIZE);

    /* convert all Comp Video representations to floating points */
    for (int i = 0; i < Seq_length(compVideoSeq); i++) {
        FloatingPoints newFloat = compVidToRGBFloats(Seq_get(compVideoSeq,
                                                                      i));
        assert(newFloat != NULL);
        Seq_addhi(floatPointSeq, newFloat);
    }

    return floatPointSeq;
}


/*
 * Purpose: given a CompVideo struct, convert it to a floating point
 * representation, by converting it to a FloatingPoints struct
 */
FloatingPoints compVidToRGBFloats(CompVideo compVideoStruct)
{
    assert(compVideoStruct != NULL);

    /* retrieve the data */
    float yVal = compVideoStruct -> Y;
    float PbVal = compVideoStruct -> Pb;
    float PrVal = compVideoStruct -> Pr;

    /* calculate values for the red, green, and blue values */
    float r = (1.0 * yVal) + (0.0 * PbVal) + (1.402 * PrVal);
    float g = (1.0 * yVal) - (0.344136 * PbVal) - (0.714136 * PrVal);
    float b = (1.0 * yVal) + (1.772 * PbVal) + (0.0 * PrVal);

    FloatingPoints newFloatStruct = malloc(sizeof(*newFloatStruct));
    assert(newFloatStruct != NULL);

    /* set ranges for clamping */
    float max = 1.0;
    float min = 0.0;

    /* clamp values so they are within the range 0 to 1 */
    r = *((float *)clampNum((void *)&r, (void *)&max,
                                                    (void *)&min, false));
    b = *((float *)clampNum((void *)&b, (void *)&max,
                                                    (void *)&min, false));
    g = *((float *)clampNum((void *)&g, (void *)&max,
                                                      (void *)&min, false));
    /* store the data */
    newFloatStruct -> red = r;
    newFloatStruct -> green = g;
    newFloatStruct -> blue = b;

    return newFloatStruct;
}
