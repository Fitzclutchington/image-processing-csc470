// ================================================================
// histo_stretch.cpp - Histogram stretching program.
//
// Written by: Andrew Fitzgerald, 2015
// =====================================================================

#include "IP.h"
using namespace std;

// function prototype
void histo_stretch(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to stretch the histogram of an image.
//
int
main(int argc, char** argv)
{
	int t1, t2;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: histo_stretch infile t1 t2 outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read and ensure flag is corrent
	t1 = atoi(argv[2]); //atoi converts strings to numbers
	t2 = atoi(argv[3]);
	if(t1 == t2){
		cerr << "Your boundaries cannot be the same number";
		exit(1);
	}

	// quantize image and save result in file
	histo_stretch(I1, t1, t2, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histo_stretch:
//
// Function histo_stretch stretches the dynamic range of the input image in file
// in to fill the entire [0,255] range. The range that is stretched spans from
// t1 to t2
//

void
histo_stretch(imageP I1, int t1, int t2, imageP I2)
{
	int	 i, total, histogram[256], min_level, max_level;
	uchar	*in, *out, lut[256]; 
    
	// total number of pixels in image and total pixels in histogram
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "histo_stretch: Insufficient memory\n";
		exit(1);
	}

	// init histogram
	for(i=0; i<MXGRAY ; i++) 
		histogram[i] = 0;

	// visit all input pixels and add 1 to it's corresponding bin
	in  = I1->image;	// input  image buffer
	for(i=0; i<total; i++) histogram[in[i]]++ ;

	// figure out the min and max of range for negative t1 and t2
	max_level = 0;		
    min_level = MXGRAY;
    
	if(t1 < 0) {
		for(i=0; i<MXGRAY; i++) {
			if(t1 < 0 && histogram[i] < min_level && histogram[i] != 0)
				min_level = i;
		}
	}

	else min_level = t1;
	
	if(t2 < 0){
		for(i=0; i<MXGRAY; i++) {
			if(t2 < 0 && histogram[i] > max_level && histogram[i] != 0)
				max_level = i;
		}
	}
 	else max_level = t2;

	//init lookup table
	for(i=0; i<MXGRAY; i++){
		if(i < min_level) lut[i] = 0;
		if(i > max_level) lut[i] = 255;
		else lut[i] = (int) MaxGray * (i - t1) / (t2 -t1);
	}

	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++) out[i] = lut[ in[i] ];
}