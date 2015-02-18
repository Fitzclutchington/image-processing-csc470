// ================================================================
// histo_plot.cpp - Histogram program.
//
// Written by: Andrew Fitzgerald, 2015
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void histo_plot(imageP, imageP, bool);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to quantize  image.
//
int
main(int argc, char** argv)
{
	bool flg;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 4) {
		cerr << "Usage: histo_plot infile outfile flag\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read and ensure flag is corrent
	flg  = atoi(argv[2]); //atoi converts strings to numbers
	if(flg != 0 and flg != 1){
		cerr << "Usage: flag must be 0 or 1\n";
		exit(1);
	}

	// quantize image and save result in file
	histo_plot(I1, I2, flg);
	IP_saveImage(I2, argv[2]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histo_plot:
//
// Function histo_plot evalutes the histogram of input image in
// and plots it into an output image out of dimensions 256 × 256.
// If flg = 0, scale the histogram pixel frequency before plotting so that
// Havg = (width * height)/256 appears at y = 128. Clip at y = 255. 
// If flg = 1, then scale the histogram pixel frequency so that the highest frequency
// value appears at y = 255 (no clipping)
//

void
histo_plot(imageP I1, imageP I2, bool flg)
{
	int	 i, total, hist_size, histogram[256];
	uchar	*in, *out; //lut is an array of size 256

	// total number of pixels in image and total pixels in histogram
	total = I1->width * I1->height;
	hist_size = MXGRAY * MXGRAY;

	// init I2 dimensions and buffer
	I2->width  = MXGRAY;
	I2->height = MXGRAY;
	I2->image  = (uchar *) malloc(hist_size);
	if(I2->image == NULL) {
		cerr << "histo_plot: Insufficient memory\n";
		exit(1);
	}

	// init lookup table ()
	for(i=0; i<MXGRAY ; i++) 
		histogram[i] = 0;

	// visit all input pixels and apply lut to quantization
	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++) histogram[in[i]]++ ;

	for(i=0; i<MXGRAY; i++) cout<< histogram[i] << endl;
}

