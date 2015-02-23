// ================================================================
// histo_plot.cpp - Histogram program.
//
// Written by: Andrew Fitzgerald, 2015
// =====================================================================

#include "IP.h"
#include <algorithm>
using namespace std;

// function prototype
void create_histogram(imageP, float, int);
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
	flg  = atoi(argv[3]); //atoi converts strings to numbers
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
// create_histogram:
//
// Function create_histogram plots histogram values based off the scale to image I2

void
create_histogram(imageP I2, float scale, int histogram[])
{
	int x,y, hist_val;
	uchar *out;
	out = I2->image;	// output image buffer
	for(x=0; x<I2->width; x++){
		hist_val = (int) (scale * histogram[x]);
	
		//make entire column white
		if(hist_val >= I2->height){
			for(y=0; y<I2->height; y++){
				out[x + I2->width * (I2->width - y - 1)] = MaxGray;
			}
		}

		else{
			for(y=0; y<hist_val; y++)
				out[x + I2->width * (I2->width - y - 1)] = MaxGray;
			for(;y<I2->height; y++)
				out[x + I2->width * (I2->width - y - 1)] = BLACK;
			
		}
	}
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
	int	 i, total, hist_size, hist_val, histogram[256],  max;
	float scale, Havg;
	uchar	*in, *out; 
    
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

	//calculate Havg
	Havg = (I1->width*I1->height)/MXGRAY;

	// init lookup table ()
	for(i=0; i<MXGRAY ; i++) 
		histogram[i] = 0;

	// visit all input pixels and add 1 to it's corresponding bin
	in  = I1->image;	// input  image buffer
	for(i=0; i<total; i++) histogram[in[i]]++ ;

	//for(i=0; i<MXGRAY; i++) cout << histogram[i]<<endl;
	//max = maximum bin
	max = *std::max_element(histogram, histogram+MXGRAY);
    //cout << max;

	//create histogram
	if(flg == 0){
		scale = 128.0 / Havg;
		create_histogram(I2, scale, histogram);
	}

	if(flg == 1){
		scale = 255.0 / max;
		create_histogram(I2, scale, histogram);
	}
}


