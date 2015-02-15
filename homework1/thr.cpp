// ================================================================
// thr.c - Threshold program.
// Copyright (C) 2012 by George Wolberg
//
// Written by: George Wolberg, 2012
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void thr(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to threshold image.
//
int
main(int argc, char** argv)
{
	int	thr_low, thr_high;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: thr infile thr_low thr_high outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	thr_low  = atoi(argv[2]); //atoi converts strings to numbers
	thr_high = atoi(argv[3]);

	// threshold image and save result in file
	thr(I1, thr_low, thr_high, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// thr:
//
// Threshold I1 and save the output in I2.
// The following rule is used for thresholding:
//	v_out = 0	0	<= v_in < thr_low
//	v_out = 128	thr_low	<= v_in < thr_high
//	v_out = 255	thr_high<= v_in < 256
//
// Ordinary bilevel thresholding occurs when thr_low = thr_high.
//
void
thr(imageP I1, int thr_low, int thr_high, imageP I2)
{
	int	 i, total;
	uchar	*in, *out, lut[256]; //lut is an array of size 256

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "thr: Insufficient memory\n";
		exit(1);
	}

	// error checking: thr_low <= thr_high; swap if necessary
	if(thr_low > thr_high) SWAP_INT(thr_low, thr_high);

	// init lookup table
	for(i=0; i<thr_low ; i++) lut[i] = 0;
	for(   ; i<thr_high; i++) lut[i] = 128;
	for(   ; i<256;      i++) lut[i] = 255;

	// visit all input pixels and apply lut to threshold
	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++) out[i] = lut[ in[i] ];
}

