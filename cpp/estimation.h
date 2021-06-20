/*****************************************************************************

Title: RLC Estimates Header File
Filename: estimation.h

Version: 1.0

Created: 25th October 2016
Last Modified: 28th May 2018

Language: C++ header

******************************************************************************/

#include "data.h"

int estimate_rlc(dsignal<double> dvPressure_t,
		 dsignal<double> dvFlow_t,
 		 double SmallFreq, 
		 double LargeFreq, 
		 double FreqStep,
		 double& R, double& L, double& C);
