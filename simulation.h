/*****************************************************************************

Title: RLC Estimates for Linear Second Order Systems
Filename: simulation.h

Version: 1.0

Created: 16th October 2016
Last Modified: 28th May 2018

Language: C++ header

******************************************************************************/


#include <iostream>
#include <stdlib.h>
#include <valarray>
#include <time.h>
#include <cmath>
#include <complex>
#include <random>
#include <fstream>

using namespace std;

int simulate_data(valarray<double>& dvPressure_t, 
		  valarray<double>& dvFlow_t, 
		  double& dSmallFreq,
		  double& dLargeFreq,
		  double& dFreqStep
);
