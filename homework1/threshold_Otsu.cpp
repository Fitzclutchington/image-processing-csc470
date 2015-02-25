// ================================================================
// threshold_otsu.cpp - Otsu Threshold program.
//
// Written by: Andrew Fitzgerald, 2015
//			   James Chou
// =====================================================================

#include "IP.h"
using namespace std;

// function prototype
void threshold_Otsu(imageP, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to threshold image.
//
int
main(int argc, char** argv)
{
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 3) {
		cerr << "Usage: threshold_otsu image_in image_out\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// quantize image and save result in file
	threshold_Otsu(I1, I2);
	IP_saveImage(I2, argv[2]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// threshold_Otsu:
//
// Function threshold_Otsu performs Otsu's adaptive thresholding method to compute
// a superior thresholded image. The image is stored in file in and the outout is
// saved in out. The algorithm uses the inout image's histogram to compute an 
// optimal threshold based on minimizing the within class variance
// All equations are taken from:
// 'A Threshold Selection Method from Gray-Level Histograms'- Nobuyuki Otsu
//

void
threshold_Otsu(imageP I1, imageP I2)
{
	int	 i, total, ideal_k;
	float histogram[256], total_mean, denominator, numerator, max_within, class0_prob, class0_mean;
	uchar	*in, *out, lut[256]; 
    
	// total number of pixels in image and total pixels in histogram
	total = I1->width * I1->height;
	;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "threshold_otsu: Insufficient memory\n";
		exit(1);
	}

	// init lookup table ()
	for(i=0; i<MXGRAY ; i++) 
		histogram[i] = 0;

	// visit all input pixels and add 1 to it's corresponding bin
	in  = I1->image;	// input  image buffer
	for(i=0; i<total; i++) histogram[in[i]]++ ;

	// normalize histogram, from equation (15)
	for(i=0; i<MXGRAY; i++) histogram[i] /= total;

	//compute mean, from: equation (1)
	total_mean = 0;
	for(i=0; i<MXGRAY; i++) total_mean += ((i+1) *histogram[i]);
    
    max_within = 0;

    // compute ideal gray level for thresholding
    for(i=0; i<MXGRAY;i++){
    	
    	class0_prob = 0;
    	for(int j = 0; j<=i; j++){ 
    		
    		class0_prob += histogram[j];}
    	
    	class0_mean = 0;
        for(int j = 0; j<=i; j++) class0_mean += ((j+1) * histogram[j]);
        class0_mean /= class0_prob;

        numerator = (total_mean*class0_prob - class0_mean) * (total_mean*class0_prob - class0_mean);
        denominator = (class0_prob * (1 - class0_prob));

        float temp = numerator/denominator;
        
        if(temp > max_within){
        	max_within = temp;
        	ideal_k = i;
        }
      
    }

    
    for(i=0; i<ideal_k ; i++) lut[i] = 0;
	for(   ; i<MXGRAY;      i++) lut[i] = 255;

	// visit all input pixels and apply lut to threshold
	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++) out[i] = lut[ in[i] ];
}