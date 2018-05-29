/*****************************************************************************

Title: RLC Estimates for Linear Second Order Systems
Filename: simulation.cpp

Version: 1.0

Created: 16th October 2016
Last Modified: 25th October 2016

Language: C++

Compilation: g++ -std=c++11 simulation.cpp

Description: The main function in this file is simulate_data() which constructs
pressure and flow datasets corresponding to some user specified impedance 
paramaters and containing certain user specified frequency components. The 
function writes the data to the disk and also makes it available to the calling
program.

******************************************************************************/

#include "simulation.h"
#include "file.h"

int simulate_data(valarray<double>& dvPressure_t,
		  valarray<double>& dvFlow_t,
		  double& dSmallFreq,
		  double& dLargeFreq,
		  double& dFreqStep
		  )
{
  using namespace std;

  /* Stage 1: Generate pressure and flow data
     Data is generated in the following manner: first, the impedance of the system
     is computed in terms of values of R L and C inputted by the user. */

  valarray<double> dvFrequencies;
  double dResistance; // R
  double dInertance; // L
  double dCompliance; // C

  // User inputs R, L and C
  cout << "Input desired values for R, L and C. Type a value and press enter." << endl;
  cout << "R = ";
  cin >> dResistance;
  cout << "L = ";
  cin >> dInertance;
  cout << "C = ";
  cin >> dCompliance;
  
  cout << endl;

  // User inputs frequency range
  cout << "The presssure and flow data will be computed at specific equally" << endl;
  cout << "space frequencies. Choose the smallest " 
          "and largest frequencies, and the step size." 
       << endl;

 Frequency_input:
  cout << "Smallest Frequency (Hz) = ";
  cin >> dSmallFreq;
  cout << "Largest Frequency (Hz) = ";
  cin >> dLargeFreq;
  cout << "Frequency Step (Hz) = ";
  cin >> dFreqStep;

  //
  // Check that the frequency range divides exactly into a multiple of 
  // the step size. The test computes a floating point division of the frequency
  // interval by the frequency step, and checks that the remainder is effectively
  // zero. I'm sure there must be a better way than this
  cout << remainder((dLargeFreq - dSmallFreq), dFreqStep);
  if (abs(remainder((dLargeFreq - dSmallFreq), dFreqStep)) > 10e-10)
    {
      cout << endl;
      cout << "Invalid choice of frequency parameters. The step size is not compatible" << endl; 
      cout << "with the frequency range. Input new values:" << endl;
      cout << endl;
      goto Frequency_input;
    }
  
  cout << endl;
 
  // Generate an array of frequencies 
  int N(round((dLargeFreq - dSmallFreq)/dFreqStep)+1);
  dvFrequencies.resize(N);
   
  for(int n=0; n <= N-1; n++)
    {
      dvFrequencies[n] = dSmallFreq + n * dFreqStep;
    }

  // Print some of the frequencies. There is a special case to deal with 
  // where the list might contain 4 or less elements. If so, print all the
  // frequencies.
  cout << "The impedance Z(f) will be computed at " << N << " frequency point(s):" << endl;
 
  if(dvFrequencies.size() <=4 ) {
    for(int n=0; n<dvFrequencies.size(); n++) {
      cout << dvFrequencies[n] << "Hz, "; 
    }
    cout << endl; 
  } else {
    cout << dvFrequencies[0] << "Hz, " << dvFrequencies[1] << "Hz, " 
	 << "..., " << dvFrequencies[N-1] << "Hz." << endl;
  }
  cout << endl;
  // Compute the impedance
  complex<double> Z[N];
 
  for(int n=0; n <= N-1; n++)
    {
      //  Z = R + (j2 pi f)L + C/(j2 pi f)
      Z[n] = complex<double>(dResistance, 2*M_PI*dvFrequencies[n]*dInertance - 
			     dCompliance/(2*M_PI*dvFrequencies[n]));
    }

  // The input pressure is taken to be the sum of N sinusoids of constant power
  // in the frequency range of interest, with random phases.
  
  double daPressurePhase[N];
  double daFlowMag[N];
  double daFlowPhase[N];

  srand(time(NULL));

  // Compute the random phase (between 0 and 2*pi).
  // Not really the best way of doing it but it will serve
  for(int n=0; n <= N-1; n++)
    {
      daPressurePhase[n] = (2*M_PI/1000)*(rand() % 1000);
    }
   
  // Compute the magnitudes of the sinusoidal components of
  // the flow 
  for(int n=0; n <= N-1; n++)
    {
      daFlowMag[n] = std::abs(Z[n]);
    }

  // Compute the phases of the sinusoidal components of
  // the flow 
  for(int n=0; n <= N-1; n++)
    {
      daFlowPhase[n] = daPressurePhase[n] - arg(Z[n]);
    }

  int S; // Number of sampling points
  double samplingFreq; 

  // User inputs the desired number of sampling points
  cout << "Input the desired number of samples of simulated data:" << endl;
  cout << "N = ";
  cin >> S;
  cout << endl;
  cout << "Input the sampling rate." << endl;
  cout << "Sampling rate = ";
  cin >> samplingFreq;

  valarray<double> dvPressure_f;
  dvPressure_f.resize(N);
  valarray<double> dvFlow_f;
  dvFlow_f.resize(N);
  //valarray<double> dvPressure_t;
  dvPressure_t.resize(S);
  //valarray<double> dvFlow_t;
  dvFlow_t.resize(S);

  // Generate samples of the pressure signal
  for(int n=0; n <= S-1; n++)
    {
      // Compute the contributions at each frequency, and put them in an array 
      for(int k=0; k <= N-1; k++)
	{
	  dvPressure_f[k] = 1*sin(2*M_PI*dvFrequencies[k]*(n/samplingFreq) - daPressurePhase[k]);
	}
      dvPressure_t[n] = dvPressure_f.sum();
    }
  
  // Generate samples of the flow signal
  for(int n=0; n <= S-1; n++)
    {
      // Compute the contributions at each frequency, and put them in an array 
      for(int k=0; k <= N-1; k++)
	{
	  dvFlow_f[k] = daFlowMag[k]*sin(2*M_PI*dvFrequencies[k]*(n/samplingFreq) - daFlowPhase[k]);
	}
      dvFlow_t[n] = dvFlow_f.sum();
    }
  
  cout << endl;

  cout << "Finished generating pressure and flow data." << endl << endl;

  // Write the pressure data to a file
  writeFile(dvPressure_t, "pressure data");

  // Write the flow data to a file
  writeFile(dvFlow_t, "flow data");

  return 0;
}
