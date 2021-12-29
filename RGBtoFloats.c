/*********************************************************************
 *                    RGBToFloats.c
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: Implementation for RGBToFloats Interface
 *              Includes implementation for functions used for converting
 *              values from their RGB values in Pnm_rgb structs to their
 *              floating point values in FloatingPoints structs and vice
 *              versa. Can convert sequences of Pnm_rgb structs and
 *              sequences of FloatingPoints structs into each other.
 *
 *********************************************************************/

#include "RGBtoFloats.h"

/*
 * Purpose: Convert a sequence of Pnm_rgb struct pointers to their floating
 * point representation
 */
Seq_T convertToFloat(Seq_T fourPixels)
{
    assert(fourPixels != NULL);

    /* create a new sequence */
    Seq_T floats = Seq_new(4);
    assert(floats != NULL);

    for (int i = 0; i < BLOCKSIZE * BLOCKSIZE; i++) {
        /* convert each pixel to a floating point representation */
        FloatingPoints newFloat = RBGToFloatingPts(Seq_get(fourPixels, i));
        assert(newFloat != NULL);

        /* store it in the sequence*/
        Seq_addhi(floats, newFloat);
    }

    return floats;
}


/*
 * Purpose: Convert a single Pnm_rgb struct, convert it to it's floating point
 * representation
 */
FloatingPoints RBGToFloatingPts(Pnm_rgb onePixel)
{
    assert(onePixel != NULL);

    /* allocate mem for a new FloatingPoints struct pointer */
    FloatingPoints newFloatStruct = malloc(sizeof(*newFloatStruct));
    assert(newFloatStruct != NULL);

    /* convert values to float representation */
    float redVal  = (float)(onePixel -> red) / (float)DENOMINATOR;
    float blueVal  = (float)(onePixel -> blue) / (float)DENOMINATOR;
    float greenVal  = (float)(onePixel -> green) / (float)DENOMINATOR;

    /* set ranges for clamping */
    float max = 1.0;
    float min = 0.0;

    /* clamp values so they are within the range 0 to 1 */
    redVal = *((float *)clampNum((void *)&redVal, (void *)&max,
                                                  (void *)&min, false));
    blueVal = *((float *)clampNum((void *)&blueVal, (void *)&max,
                                                  (void *)&min, false));
    greenVal = *((float *)clampNum((void *)&greenVal, (void *)&max,
                                                    (void *)&min, false));

    /* store the data in the float representation */
    newFloatStruct->red = redVal;
    newFloatStruct->blue = blueVal;
    newFloatStruct->green = greenVal;

    return newFloatStruct;
}


/*
 * Purpose: Convert a sequence of floating point structs to a sequence of
 * Pnm_rgb structs
 */
Seq_T convertToScaledInts(Seq_T fp_sequence)
{
    assert(fp_sequence != NULL);

    /* create a new sequence of Pnm_rgb structs */
    Seq_T pixelBlock = Seq_new(BLOCKSIZE * BLOCKSIZE);
    assert(pixelBlock != NULL);

    /* convert each floating point back to a pixel */
    for (int i = 0; i < Seq_length(fp_sequence); i++) {
        Pnm_rgb newPix = floatsToScaledInts(Seq_get(fp_sequence, i));
        assert(newPix != NULL);

        Seq_addhi(pixelBlock, newPix);
    }

    return pixelBlock;
}


/* Purpose: convert a single floating point struct to a Pnm_rgb struct */
Pnm_rgb floatsToScaledInts(FloatingPoints RGBVals)
{
    assert(RGBVals != NULL);

    /* get floating point values for RGB */
    float redFloatVal = RGBVals -> red;
    float blueFloatVal= RGBVals -> blue;
    float greenFloatVal= RGBVals -> green;

    /* convert floating point values to unsigned ints */
    unsigned scaledRed = redFloatVal * DENOMINATOR;
    unsigned scaledBlue = blueFloatVal * DENOMINATOR;
    unsigned scaledGreen = greenFloatVal * DENOMINATOR;

    /* create new Pnm_rgb struct and fill with scaled values */
    Pnm_rgb RGBStruct = malloc(sizeof(*RGBStruct));
    assert(RGBStruct != NULL);

    int max = DENOMINATOR;
    int min = 0;

    /* clamp all values to min and max */
    scaledRed = *((int *)clampNum((void *)&scaledRed, (void *)&max,
                                              (void *)&min, true));
    scaledBlue = *((int *)clampNum((void *)&scaledBlue, (void *)&max,
                                              (void *)&min, true));
    scaledGreen = *((int *)clampNum((void *)&scaledGreen, (void *)&max,
                                              (void *)&min, true));

    /* store the data in the RGB struct pointer */
    RGBStruct -> red = scaledRed;
    RGBStruct -> blue = scaledBlue;
    RGBStruct -> green = scaledGreen;

    return RGBStruct;
}
