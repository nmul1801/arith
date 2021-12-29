/*********************************************************************
 *                     DiscreteCosTrans.c
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: Implementation for the DiscreteCosTrans interface
 *              Implementation for functions used to perform discrete cosign
 *              and inverse discrete cosign transformation given a sequence
 *              of Comp Video structs or an array containing abcd
 *
 *********************************************************************/


#include "DiscreteCosTrans.h"

/*
 * Purpose: calculate the values of a b c and d and store them given a sequence
 * of Component Video struct pointers
 */
void calculateABCD(Seq_T componentVideos, float *abcd)
{
    assert(componentVideos != NULL && abcd != NULL);

    /* get Y's from each of the 4 pixels */
    float y1 = ((CompVideo)Seq_get(componentVideos, 0)) -> Y;
    float y2 = ((CompVideo)Seq_get(componentVideos, 1)) -> Y;
    float y3 = ((CompVideo)Seq_get(componentVideos, 2)) -> Y;
    float y4 = ((CompVideo)Seq_get(componentVideos, 3)) -> Y;

    float maxA = 1.0;
    float minA = 0.0;
    float maxBCD = 0.3;
    float minBCD = -0.3;

    /* calculate a b c and d with given equations */
    float a = (y4 + y3 + y2 + y1) / 4.0;
    float b = (y4 + y3 - y2 - y1) / 4.0;
    float c = (y4 - y3 + y2 - y1) / 4.0;
    float d = (y4 - y3 - y2 + y1) / 4.0;

    /* clamp values so they are within the appropriate ranges */
    a = *((float *)clampNum((void *)&a, (void *)&maxA,
                                        (void *)&minA, false));
    b = *((float *)clampNum((void *)&b, (void *)&maxBCD,
                                        (void *)&minBCD, false));
    c = *((float *)clampNum((void *)&c, (void *)&maxBCD,
                                        (void *)&minBCD, false));
    d = *((float *)clampNum((void *)&d, (void *)&maxBCD,
                                        (void *)&minBCD, false));
    /* add a, b, c, d to array */
    abcd[0] = a;
    abcd[1] = b;
    abcd[2] = c;
    abcd[3] = d;
}


/*
 * Purpose: calculate all Y values and store them given the values of a, b, c,
 * and d
 */
void calculateYs(float *abcd, float *yValues)
{
    assert(abcd != NULL && yValues != NULL);

    /* retrieve the values */
    float a = abcd[0];
    float b = abcd[1];
    float c = abcd[2];
    float d = abcd[3];

    /* convert abcd into Y values */
    yValues[0] = a - b - c + d;
    yValues[1] = a - b + c - d;
    yValues[2] = a + b - c - d;
    yValues[3] = a + b + c + d;
}
