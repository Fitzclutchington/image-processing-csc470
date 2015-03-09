// ================================================================
// unordered_dither.cpp - Unordered dithering program.
//
// Written by: Andrew Fitzgerald, 2015
//				James Chou
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void unordered_dither(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to quantize image using dithering.
//
int
main(int argc, char** argv)
{
	int	levels;
	float gamma;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: unordered_dither infile levels gamma outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read amount of quantization levels
	levels  = atoi(argv[2]); //atoi converts strings to numbers
	if(levels <= 0 or levels > 256){
		cerr << "Usage: 0 <= levels <= 256\n";
		exit(1);
	}

	gamma = atoi(argv[3]);

	// quantize image with unordered dithering and save result in file
	unordered_dither(I1, levels, gamma, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// unordered_dither:
//
// Reads input file and applies unordered dither to it
// before quantization to n levels, saves to file out
//

void
unordered_dither(imageP I1, int levels, int gamma, imageP I2)
{
	int	 i, total, scale, midpoint, R;
	uchar	*in, *out, lut[256]; //lut is an array of size 256

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "qntz: Insufficient memory\n";
		exit(1);
	}

	// init lookup table (), midpoint is a shift to have levels biased at 128
	scale = MXGRAY / levels;
	midpoint = scale / 2;
	for(i=0; i<MXGRAY ; i++) 
		lut[i] = scale * (int) (i/scale) + midpoint;

	// visit all input pixels and apply lut to quantization
	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++){
		R = (rand() % 33) - 16;
		cerr << R << endl;
		out[i] = lut[ in[i] + R ];
	}
}

