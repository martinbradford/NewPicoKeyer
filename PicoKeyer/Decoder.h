// Decoder.h

#ifndef _DECODER.h
#define _DECODER.h 

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern float sampling_freq; 
extern float target_freq;
extern float bucket_count; 
extern int magnitudelimit_low;
extern int magnitudelimit;

extern int wpm;

void calculate_decoder_parameters();

#endif

