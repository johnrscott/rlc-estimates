/*****************************************************************************

Title: RLC Estimates for Linear Second Order Systems
Filename: simulation.h

Version: 1.0

Created: 16th October 2016
Last Modified: 30th May 2018

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
#include "data.h"

//using namespace std;

// It would be better if this populated a data object which contained both
// the dependent and independent variables. At the moment it outputs it just
// stores the simulated data in an array and the startpoint, endpoint and
// stepsize of the independent variable.
int simulate_data(dsignal<double>& dvPressure_t, 
		  dsignal<double>& dvFlow_t, 
		  double& dSmallFreq,
		  double& dLargeFreq,
		  double& dFreqStep
);
