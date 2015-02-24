// ================================================================
// histo_stretch.cpp - Histogram stretching program.
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
	if(flg != 0 and flg != 1){
		cerr << "Usage: flag must be 0 or 1\n";
		exit(1);
	}

	// quantize image and save result in file
	histo_stretcn(I1, t1, t2, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}