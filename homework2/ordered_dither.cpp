// ================================================================
// ordered_dither.cpp - ordered dithering program.
//
// Written by: Andrew Fitzgerald, 2015
//				James Chou
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void ordered_dither(imageP, int, float, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to quantize image using dithering.
//
int
main(int argc, char** argv)
{
	int	dither_mat_size;
	float gamma;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: ordered_dither infile levels gamma outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;


	// read amount of quantization levels
	dither_mat_size  = atoi(argv[2]); //atoi converts strings to numbers
	//if(dither_mat_size != 3 || (dither_mat_size & (dither_mat_size - 1)) != 0){
	//	cerr << "matrix size must be 3 or a power of 2\n";
	//	exit(1);
	//	}

	gamma = atoi(argv[3]);

	// quantize image with ordered dithering and save result in file
	ordered_dither(I1, dither_mat_size, gamma, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ordered_dither:
//
// Reads the input in file 'in' and applies ordered dither to it, storing the output in 
// in file 'out'. Function uses a square dither matrix ith dimensions m x m
//

void
ordered_dither(imageP I1, int dither_mat_size, float gamma, imageP I2)
{
	int	 i, j, total,  midpoint, dith_elems, 
	     dither_mat2[] = {0, 2, 3, 1},
	     dither_mat3[] = {6, 8, 4, 1, 0, 3, 5, 2, 7},
	     *dither_mat;
	float scale;
	uchar	*in, *out, lut[256]; //lut is an array of size 256
    
    dith_elems = dither_mat_size* dither_mat_size;
    if(dither_mat_size == 2){
    	dither_mat = dither_mat2;
    }
    else{
    	dither_mat = dither_mat3;
    }
   	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "unordered_dither: Insufficient memory\n";
		exit(1);
	}

	// init lookup table (), midpoint is a shift to have levels biased at 128
	scale = MXGRAY/dith_elems;
	for(i=0; i< dith_elems ; i++){
		dither_mat[i] = scale * dither_mat[i];
	}
       

	// visit all input pixels and apply lut to quantization
	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer
	
	for(int y=0; y<I1->height; y++){ // visit all input rows
		for(int x=0; x<I1->width; x++){// visit all input cols
			i = x % dither_mat_size;// dither matrix index
			j = y % dither_mat_size;// dither matrix index
			
			// threshold pixel using dither value Dij(n)
			out[y*I1->width+x] = (in[y*I1->width+x] > dither_mat[j*dither_mat_size + i])? 255 : 0;
		}
	}
}

