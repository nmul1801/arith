/*********************************************************************
 *                     40Images.c
 *
 *     Assignment: HW4: Arith
 *     Authors:  Grace McCarthy (gmccar04), Nick Mulligan (nmulli01)
 *     Date:     March 22, 2021
 *     Purpose: Implementation for both compressing adn decompressing a
 *              given ppm image
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
#include <seq.h>
#include <stdbool.h>
#include "RGBFloatsToCompVid.h"
#include "RGBtoFloats.h"
#include </comp/40/build/include/arith40.h>
#include "changeNums.h"
#include "DiscreteCosTrans.h"
#include "ScaleABCD.h"
#include "bitpack.h"

#define WORDSIZE 32
#define DENOMINATOR 255
#define BLOCKSIZE 2

#define A_LSB 26
#define B_LSB 20
#define C_LSB 14
#define D_LSB 8

#define PB_LSB 4
#define PR_LSB 0

#define A_WIDTH 6
#define B_WIDTH 6
#define C_WIDTH 6
#define D_WIDTH 6

#define PR_WIDTH 4
#define PB_WIDTH 4


static void (*compress_or_decompress)(FILE *input) = compress40;

Pnm_ppm scaleWidthAndHeight(Pnm_ppm origImage, A2Methods_T methods);

Pnm_ppm createNewPnmppm(int width, int height);

void applyCopy(int col, int row, A2Methods_UArray2 newPixmap, void *elem,
void *origImage);

void decompress40(FILE *input);

void checkHeader(FILE *input, unsigned *height, unsigned *width);

void applyComp(int col, int row, A2Methods_UArray2 newPixmap, void *elem,
void *cl);

void applyDecomp(int row, int col, A2Methods_UArray2 array, void *currVal,
                                                            void *filePtr);

void freeSequences(Seq_T compVidSeq, Seq_T RGBfloats);

void copyPixData(Pnm_rgb orig, Pnm_rgb copy);

UnsignedInt getWord(FILE *filePtr);

void convertToWord(Seq_T fourPixels);

float calculateAverage(float num1, float num2, float num3, float num4);

float averagePr(Seq_T componentVideos);

float averagePb(Seq_T componentVideos);

Seq_T createCompVidSeq(float *yValues, float PbVal, float PrVal);

Seq_T convertToPixels(UnsignedInt word);

int main(int argc, char *argv[])
{
        int i;
        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-c") == 0) {
                        compress_or_decompress = compress40;
                } else if (strcmp(argv[i], "-d") == 0) {
                        compress_or_decompress = decompress40;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n",
                                argv[0], argv[i]);
                        exit(1);
                } else if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s -d [filename]\n"
                                "       %s -c [filename]\n",
                                argv[0], argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                compress_or_decompress(fp);
        } else {
                compress_or_decompress(stdin);
        }

        return EXIT_SUCCESS;
}

/*
 * Function: compress40
 * Job: takes in a file pointer to a ppm  and compresses it, outputting a
 * sequence of 32 bit words that represent the compressed version of that
 * image
 * Expected input: FILE pointer to an image
 * Expected output: printing the compressed image to stdout
 * Expectations: FILE *input is not null
*/
extern void compress40(FILE *input)
{
    /* error check and set methods */
    assert(input != NULL);
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods != NULL);

    /* read in the image, close the file */
    Pnm_ppm origImage = Pnm_ppmread(input, methods);
    assert(origImage != NULL);
    fclose(input);

    /* get pixmap with even width and height, then free the original image */
    Pnm_ppm scaledImage = scaleWidthAndHeight(origImage, methods);
    assert(scaledImage != NULL);

    Pnm_ppmfree(&origImage);

    /* write out the header */
    printf("COMP40 Compressed image format 2\n%u %u\n", scaledImage -> width,
                                                    scaledImage -> height);

    /* map the image to read all pixels and compress */
    scaledImage -> methods -> map_default(scaledImage -> pixels, applyComp,
                                                                  methods);

    /* free allcated memory for scaled image */
    Pnm_ppmfree(&scaledImage);
}


/*
 * Function: decompress40
 * Job: takes in a file containing 32 bit words representing an image
 * and decompresses it, making a new ppm as a result
 * Expected input: FILE pointer to a file containing the string of 32 bit
 * words
 * Expected output: printing the decompressed image to stdout
 * Expectations: FILE *input is not null
*/
extern void decompress40(FILE *input)
{
    assert(input != NULL);

    /* check that input file is in correct format */
    unsigned width, height;
    checkHeader(input, &width, &height);

    /* create a new image, then traverse it while reading in the file */
    Pnm_ppm newImage = createNewPnmppm(width, height);
    assert(newImage != NULL);

    newImage -> methods -> map_default(newImage -> pixels, applyDecomp,
                                                                input);

    Pnm_ppmwrite(stdout, newImage);
    fclose(input);
    Pnm_ppmfree(&newImage);
}


/*
 * Function: scaleWidthAndHeight
 * Job: Takes in an original image and creates a copy of it, making both
 * the width and height of the new image even
 * Expected input: Ppm representing the original image, and a copy of the
 * original image's methods suite
 * Expected output: Copy of original image with trimmed edges
 * Expectations: origImage and methods are both not null
*/
Pnm_ppm scaleWidthAndHeight(Pnm_ppm origImage, A2Methods_T methods)
{
    assert(origImage != NULL && methods != NULL);

    /* create a new pixel array for newImage */
    Pnm_ppm newImage = malloc(sizeof(*newImage));
    assert(newImage != NULL);

    /* make width and height of image even */
    newImage -> width = makeEven(origImage -> width);
    newImage -> height = makeEven(origImage -> height);

    /* set denominator and methods for newImage */
    newImage -> denominator = origImage -> denominator;
    newImage -> methods = methods;

    /* create new pixel array for newImage */
    A2Methods_UArray2 pixels = newImage -> methods -> new(newImage -> width,
                                newImage -> height, sizeof(struct Pnm_rgb));
    assert(pixels != NULL);
    newImage -> pixels = pixels;

    /* copy the pixels from the original image to the trimmed image */
    newImage -> methods -> map_default(newImage -> pixels, applyCopy,
                                                           origImage);
    return newImage;
}


/*
 * Function: applyCopy
 * Job: apply function for copying the image onto the trimmed verion
 * Expected input: two integers representing the coordinates of the current
 * position, a UArray2 representing the pixels, a void poiter representing the
 * current data, and a void pointer representing the original image
 * Expected output: none, void
 * Expectations: the map of pixels is not null, the current element is not
 * null,
 * and the original image is not null
*/
void applyCopy(int col, int row, A2Methods_UArray2 newPixmap, void *elem,
void *origImage)
{
    assert(newPixmap != NULL && elem != NULL && origImage != NULL);

    /* cast necessary variables */
    Pnm_ppm original = origImage;
    Pnm_rgb currPix = elem;

    /* get the pixel of the original image */
    Pnm_rgb origPix = original -> methods -> at(original -> pixels, col, row);

    /* copy the data */
    currPix -> red = origPix -> red;
    currPix -> blue = origPix -> blue;
    currPix -> green = origPix -> green;
}

/* Function: checkHeader
 * Job: check that the compressed image being read in is in the correct
 * format
 * Expected input: file pointer, pointer to the height and pointer to the
 * width of the compressed image being read in
 * Expected output: none
*/
void checkHeader(FILE *input, unsigned *width, unsigned *height)
{
    assert(input != NULL && width != NULL && height != NULL);
    /* scan the header */
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                                                        width, height);
    assert(read == 2);

    /* get the next character, make sure it's a newline character */
    int c = getc(input);
    assert(c == '\n');
}


/* Function: createNewPnmppm
 * Job: create a new Pnm_ppm given the width and height
 * Expected input: two integers holding the width and height of the new
 * pnm
 * Expected output: a Pnm_ppm
*/
Pnm_ppm createNewPnmppm(int width, int height)
{
    /* allocate memory */
    Pnm_ppm newImage = malloc(sizeof(*newImage));
    assert(newImage != NULL);

    /* copy the data */
    newImage -> width = width;
    newImage -> height = height;
    newImage -> denominator = DENOMINATOR;
    newImage -> methods = uarray2_methods_plain;
    newImage -> pixels = newImage -> methods -> new(width, height,
                                    sizeof(struct Pnm_rgb));
    return newImage;
}


/*
 * Function: applyComp
 * Job: apply function for the compression of the image
 * Expected input: two integers representing the coordinates of the current
 * position, a UArray2 representing the pixels, a void pointer representing
 * the current data, and a void pointer representing the methods of the pixel
 * map
 * Expected output: a 32 bit word printed to stdout representing the compressed
 * verion of the current 4 pixels
 * Expectations: the map of pixels is not null, the current element is not
 * null, and the methods are not null
*/
void applyComp(int col, int row, A2Methods_UArray2 newPixmap, void *elem,
void *methods)
{
    assert(methods != NULL && newPixmap != NULL && elem != NULL);

    if (col % BLOCKSIZE == 0 && row % BLOCKSIZE == 0) {
        A2Methods_T castMethods = methods;
        /* get top left pixel */
        Pnm_rgb first = elem;
        /* get top right pixel */
        Pnm_rgb second = castMethods->at(newPixmap, col + 1, row);
        /* get bottom left pixel */
        Pnm_rgb third = castMethods->at(newPixmap, col, row + 1);
        /* get bottom right pixel */
        Pnm_rgb fourth = castMethods->at(newPixmap, col + 1, row + 1);

        /* put 4 pixels in a sequence and convert them to a word */
        Seq_T fourPixels = Seq_seq(first, second, third, fourth, NULL);
        assert(fourPixels != NULL);

        convertToWord(fourPixels);

        Seq_free(&fourPixels);

    }
}


/*
 * Function: applyDecomp
 * Job: apply function for the decompression of the image
 * Expected input: two integers representing the coordinates of the current
 * position, a UArray2 representing the pixels, a void pointer representing
 * the current data, and a void pointer representing the file used for reading
 * in the 32 bit words
 * Expected output: none, void
 * Expectations: the map of pixels is not null, the current element is not
 * null, and the file pointer is not null
*/
void applyDecomp(int col, int row, A2Methods_UArray2 array, void *currVal,
                                                            void *filePtr)
{
    assert(filePtr != NULL && array != NULL && currVal != NULL);

    A2Methods_T methods = uarray2_methods_plain;
    assert(methods != NULL);

    /* only pack the block when on even coordinates */
    if (col % BLOCKSIZE == 0 && row % BLOCKSIZE == 0) {
        FILE *fptr = filePtr;
        /* reads in unsigned int word from file */
        UnsignedInt word = getWord(fptr);
        /* convert unsigned int word into a sequence of Pnm_rgb pixels  */
        Seq_T pixels = convertToPixels(word);
        assert(pixels != NULL && Seq_length(pixels) == BLOCKSIZE * BLOCKSIZE);

        /* retrieve the pixels map row major style */
        for (int i = 0; i < BLOCKSIZE; i++) {
            for (int j = 0; j < BLOCKSIZE; j++) {
                /* get the current pixel in the sequence and in the array */
                Pnm_rgb newPixelData = (Seq_get(pixels, i * BLOCKSIZE + j));
                Pnm_rgb uninitializedPixel = methods->at(array, col + j,
                                                                    row + i);

                assert(newPixelData != NULL && uninitializedPixel != NULL);
                /* copy the data */
                copyPixData(newPixelData, uninitializedPixel);
            }
        }

        /* free memory */
        for (int i = 0; i < Seq_length(pixels); i++) {
            Pnm_rgb currPix = Seq_get(pixels, i);
            free(currPix);
        }
        Seq_free(&pixels);
    }
}


/*
 * Function: copyPixData
 * Job: copies red, green, and blue values from the original pixel to the
 * empty pixel
 * Expected input: the original Pnm_rgb pixel that is being copied, and the
 * uninitialized Pnm_rgb pixel that it is being copied into
 * Expected output: none, void
 * Expectations: both pixels are not NULL
 */
void copyPixData(Pnm_rgb orig, Pnm_rgb uninitialized)
{
    /* error check */
    assert(orig != NULL && uninitialized != NULL);

    /* copy all data */
    uninitialized->red = orig->red;
    uninitialized->green = orig->green;
    uninitialized->blue = orig->blue;
}


/* Function: convertToPixels
 * Job: given a unsigned int, convert the int into a sequence
 * of 4 Pnm_rgb pixels
 * Expected input: unsigned integer
 * Expected output: sequence of 4 Pnm_rgb structs that represent the four
 * pixels in a 2 x 2 block
*/
Seq_T convertToPixels(UnsignedInt word)
{
    /* get all values from the unsigned int bit word */
    UnsignedInt getA = Bitpack_getu(word, A_WIDTH, A_LSB);
    SignedInt getB = Bitpack_gets(word, B_WIDTH, B_LSB);
    SignedInt getC = Bitpack_gets(word, C_WIDTH, C_LSB);
    SignedInt getD = Bitpack_gets(word, D_WIDTH, D_LSB);
    UnsignedInt getPr = Bitpack_getu(word, PR_WIDTH, PR_LSB);
    UnsignedInt getPb = Bitpack_getu(word, PB_WIDTH, PB_LSB);

    /* clamp Pr and Pb values so they are within range */
    float minP = -0.5;
    float maxP = 0.5;
    getPr = *(UnsignedInt *)clampNum((void *)&getPr, (void *)&maxP,
                                                     (void *)&minP, true);
    getPb = *(UnsignedInt *)clampNum((void *)&getPb, (void *)&maxP,
                                                      (void *)&minP, true);

    /* convert the values of Pb and Pr with given functions */
    float pbValAfter = Arith40_chroma_of_index(getPb);
    float prValAfter = Arith40_chroma_of_index(getPr);

    float abcd [4];
    float yValues[4];

    /* convert values of a b c d to floats */
    abcd[0] = intAtoFloat(getA, A_WIDTH);
    abcd[1] = intBCDtoFloat(getB, B_WIDTH);
    abcd[2] = intBCDtoFloat(getC, C_WIDTH);
    abcd[3] = intBCDtoFloat(getD, D_WIDTH);

    /* initialize the y values array */
    calculateYs(abcd, yValues);
    /* convert values back to comp vid, then floats, then to pixels */
    Seq_T compVidSeq = createCompVidSeq(yValues, pbValAfter, prValAfter);
    Seq_T RGBfloats = convertToRGBFloats(compVidSeq);
    Seq_T pixelsAfter = convertToScaledInts(RGBfloats);

    /* free memory */
    freeSequences(compVidSeq, RGBfloats);

    return pixelsAfter;

}


/* Function: freeSequences
 * Job: deallocate memory for a sequence of CompVideo structs and a
 * sequence of FloatingPoints structs
 * Expected input: sequence holding CompVideo structs and a sequence storing
 * FloatingPoints structs
 * Expected output: none
 * Expectations: both sequences are not NULL
*/
void freeSequences(Seq_T compVidSeq, Seq_T RGBfloats)
{
    assert(compVidSeq != NULL && RGBfloats != NULL);

    /* free sequence of comp video structs */
    for (int i = 0; i < Seq_length(compVidSeq); i++) {
        CompVideo currCompVidStruct = Seq_get(compVidSeq, i);
        free(currCompVidStruct);
    }
    Seq_free(&compVidSeq);

    /* free sequence of RGB floats */
    for (int i = 0; i < Seq_length(RGBfloats); i++) {
        FloatingPoints currFloat = Seq_get(RGBfloats, i);
        free(currFloat);
    }
    Seq_free(&RGBfloats);
}



/* Function: getWord
 * Job: given an unsigned int, convert the int into a sequence
 * of 4 Pnm_rgb pixels
 * Expected input: file pointer
 * Expected output: unsigned int
 * Expectations: both sequences are not NULL
*/
UnsignedInt getWord(FILE *filePtr)
{
    assert(filePtr != NULL && WORDSIZE >= 8);

    UnsignedInt readInWord = 0;

    /* iterating through the file 8 bits at a time, get the next word */
    for (int i = WORDSIZE - 8; i >= 0; i -= 8) {
        unsigned currChar = fgetc(filePtr);
        /* add the next byte to the word */
        readInWord = Bitpack_newu(readInWord, 8, i, currChar);
    }

    return readInWord;
}



/* Function: convertToWord
 * Job: given a sequence of four pixels, convert the four pixels into
 * a unsigned integer and print it out
 * Expected input: sequence of 4 Pnm_rgb structs that represent the four
 * pixels in a 2 x 2 block
 * Expected output: none
*/
void convertToWord(Seq_T fourPixels)
{
    assert(fourPixels != NULL);

    /* convert the pixels to floating pt representation*/
    Seq_T floatingPts = convertToFloat(fourPixels);
    /* then to comp vid struct representation (pb, pr, y)*/
    Seq_T compVideos = convertToCompVideo(floatingPts);

    /* find average Pb and Pr */
    float PrVal = averagePr(compVideos);
    float PbVal = averagePb(compVideos);

    unsigned pbQuantized = Arith40_index_of_chroma(PbVal);
    unsigned prQuantized = Arith40_index_of_chroma(PrVal);

    /* initialize the values of a b c and d*/
    float abcd [4];
    calculateABCD(compVideos, abcd);

    /* retrieve values */
    UnsignedInt a = scaleAToInt(abcd[0], A_WIDTH);
    SignedInt b = scaleBCDToInt(abcd[1], B_WIDTH);
    SignedInt c = scaleBCDToInt(abcd[2], C_WIDTH);
    SignedInt d = scaleBCDToInt(abcd[3], D_WIDTH);

    UnsignedInt bitWord = 0;
    UnsignedInt prWord, pbWord, aWord, bWord, cWord, dWord;

    pbWord = Bitpack_newu(bitWord, PB_WIDTH, PB_LSB, pbQuantized);
    prWord = Bitpack_newu(pbWord, PB_WIDTH, PR_LSB, prQuantized);
    aWord = Bitpack_newu(prWord, A_WIDTH, A_LSB, a);
    bWord = Bitpack_news(aWord, B_WIDTH, B_LSB, b);
    cWord = Bitpack_news(bWord, C_WIDTH, C_LSB, c);
    dWord = Bitpack_news(cWord, D_WIDTH, D_LSB, d);


    freeSequences(compVideos, floatingPts);

    /* print out the word to stdout */
    for (int i = WORDSIZE - 8; i >= 0; i -= 8) {
        int currChar = Bitpack_getu(dWord, 8, i);
        putchar(currChar);
    }

}



/* Function: averagePr
 * Job: average the 4 Pr values stored in the sequence of CompVideo structs
 * Expected input: a sequence of CompVideo structs
 * Expected output: a float that holds the average Pr value
 * Expectations: sequence of CompVideo structs is not NULL
*/
float averagePr(Seq_T componentVideos)
{
    assert(componentVideos != NULL);

    float total = 0.0;
    /* loop through the sequence keeping track of the total */
    for (int i = 0; i < Seq_length(componentVideos); i++) {
        CompVideo currVid = Seq_get(componentVideos, i);
        assert(currVid != NULL);
        total += currVid -> Pr;
    }

    /* calculate the average */
    return total / Seq_length(componentVideos);
}



/* Function: averagePb
 * Job: average the 4 Pb values stored in the sequence of CompVideo structs
 * Expected input: a sequence of CompVideo structs
 * Expected output: a float that holds the average Pb value
 * Expectations: sequence of CompVideo structs is not NULL
*/
float averagePb(Seq_T componentVideos)
{
    assert(componentVideos != NULL);
    float total = 0.0;
    /* loop through the sequence keeping track of the total */
    for (int i = 0; i < Seq_length(componentVideos); i++) {
        CompVideo currVid = Seq_get(componentVideos, i);
        assert(currVid != NULL);
        total += currVid -> Pb;
    }

    /* calculate the average */
    return total / Seq_length(componentVideos);
}



/* Function: createCompVidSeq
 * Job: create a sequence of CompVideo structs
 * Expected input:a pointer to an array of floats, PbVal and PrVal
 * Expected output: sequence of CompVideo structs
*/
Seq_T createCompVidSeq(float *yValues, float PbVal, float PrVal)
{
    Seq_T compVidSeq = Seq_new(4);
    assert(compVidSeq != NULL);

    /* create new Comp Video struct for all y values */
    for (int i = 0; i < 4; i++) {
        CompVideo newCompVidStruct = initCompVidStruct(yValues[i], PbVal,
                                                                    PrVal);
        Seq_addhi(compVidSeq, newCompVidStruct);
    }

    return compVidSeq;
}
