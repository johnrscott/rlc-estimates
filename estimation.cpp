/*****************************************************************************

Title: RLC Estimation given input and output data
Filename: estimation.cpp

Version: 1.0

Created: 25th October 2016
Last Modified: 28th May 2018

Language: C++

Compilation: g++ -std=c++11 estimation.cpp

Description: 

******************************************************************************/

#include "estimation.h"
#include "spectral.h" 
#include "file.h"

int estimate_rlc(valarray<double> dvPressure_t,
		 valarray<double> dvFlow_t,
		 double SmallFreq, 
		 double LargeFreq, 
		 double FreqStep,
		 double& R, double& L, double& C)
{
  using namespace std;
  
  // Create an array of frequencies -- STILL NEEDS CHECKING
  // The type is complex to facilitate complex arithmetic later on.
  // One problem here that needs fixing is the need for a real and 
  // complex frequency array for compatibility with other function 
  // calls. It should all just be consistent.
  valarray<double> dvFrequencies;
  valarray<complex<double> > F;

  // There's something up with the way it's doing this calculation. It 
  // seems to work sometimes but not always depending on the numbers.
  double size(1 + (LargeFreq - SmallFreq)/FreqStep);
  /* There's something up with theses tests.
  if(size != (int)size)
    {
      cerr << "Error: Start and end frequencies are " 
              "incompatible with the frequency step. " 
              "Terminating program."
	   << endl;
      return 1;
    }
  */   
  dvFrequencies.resize(size);
  F.resize(size);
  for(int n=0; n<size; n++) 
    {
      F[n] = complex<double>(SmallFreq + n*FreqStep, 0); // complex 
      dvFrequencies[n] = SmallFreq + n*FreqStep; // real
    }

  cout << "******    Estimation of RLC values    ******" << endl;
  cout << "The pressure and flow data inputs have " << dvFlow_t.size() 
       << " points each." << endl;

 /*** Stage 2: Compute the power specturm **********************************/

  valarray<complex<double> > autospectrum;
  valarray<complex<double> > crossspectrum;

  int result = Sxx(dvPressure_t, autospectrum);
  if(result != 0)
    {
      cerr << "Error: Sxx function did not execute correctly. Terminating program."
	   << endl;
      return 1;
    }
  result = Sxy(dvPressure_t, dvFlow_t, crossspectrum);
  if(result != 0)
    {
      cerr << "Error: Sxx function did not execute correctly. Terminating program."
	   << endl;
      return 1;
    }

  // Write the auto power spectrum Sxx to file
  writeFile(autospectrum, "Sxx data");

  // Write the cross power spectrum Sxy to file
  writeFile(crossspectrum, "Sxy data");

  /*** Stage 3: Compute the RLC estimates ************************************/

  valarray<complex<double> > impedanceEstimate;
  
  /* First compute an estimate of the impedance, according to 
     
     Z~[f] = Sxx[f]/Sxy[f];

  */

  impedanceEstimate = crossspectrum/autospectrum;

  // Then define the real and imaginary parts U[f] and V[f]:
  valarray<complex<double> > U;
  valarray<complex<double> > V;

  // Length of U and V
  int N = dvFrequencies.size();

  // Resize U and V
  U.resize(N);
  V.resize(N);

  // This is only going to work when all frequencies in impedanceEstimate are
  // of interest. There's a bug hiding in here. Suppose that the pressure and 
  // flow data contains frequency components at half integer frequencies: 0.5Hz,
  // 1Hz, 1.5Hz, ..., 100Hz. Then F will contain 200 points. Lets say that the
  // pressure and flow data contained 1024 samples at a sampling frequency of 
  // 1024Hz. Then the resulting 1024 point fourier transform will result in 
  // a 1024 point long signal in the frequency domain, whose frequency resolution
  // is 1Hz. So the impedance estimate is going to contain 1024 values, spaced 
  // at 1Hz. This doesn't agree with the actual frequency content of the signal,
  // which has power also at half integer frequencies. Further, all the frequencies
  // above 100Hz represent noise.
  //
  // This best way to fix this is to only use frequency data from the Sxx and Sxy
  // which coincides with the simulated frequency components. This means that 
  // Sxx and Sxy somehow have to report their frequencies. 
  //
  // I think the reason for the n+1 in the array below is to remove the 0Hz
  // component from the power spectral estimates, on the assumption that there is
  // no power at DC. Then the only way to get sensible estimates is if the 
  // frequency resolution of the spectral estimates agrees with the resolution of
  // the frequency components in the pressure and flow data. 
  //
  // E.g. If the simulated data has frequency components at 0.2Hz, 0.4Hz, etc. up
  // to 100Hz, then the fft needs to have a resolution of precisely 0.2Hz. This 
  // can be achieved with, e.g., 1024 samples at a sampling rate of 204.8Hz. Note
  // also that it is important for there to be enough sampling points so that the 
  // fft goes up to 100Hz (in this case it needs at least 500 points in the non-
  // trivial half of the transform). Extra fft points are just ignored by this 
  // routine (that's also part of the bug)
  for(int n=0; n<N; n++) 
    {
      U[n] = real(impedanceEstimate[n+1]);
      V[n] = imag(impedanceEstimate[n+1]);
    }

  // valarray of 1s
  valarray<complex<double> > one(1,N);

  // // Generate the array of frequencies
  // valarray<complex<double> > F;
  // F.resize(frequencies.size());

  // // Write the frequencies
  // for(int n=0; n < dvFrequencies.size(); n++)
  //   { 
  //     F[n]=complex<double>(frequencies[n],0);
  //   }

  // Compute a, b and c
  complex<double> a = (one/pow((complex<double>(2*M_PI,0)*F),2)).sum();
  complex<double> b = pow((complex<double>(2*M_PI,0)*F),2).sum();
  complex<double> c = pow(N,2) - a*b;

  // Compute the estimates -- cast integers to double to avoid integer division
  // The functions also take the real part, on the assumption that the answer
  // is real anyway. This definitely needs checking!
  R = real(complex<double>((double)1/(double)N,0)*U.sum());
  C = real((b/c) * (V/(complex<double>(2*M_PI,0)*F)).sum() - 
       (complex<double>(N,0)/c) * (V*(complex<double>(2*M_PI,0)*F)).sum());
  L = real((complex<double>(N,0)/c) * 
    (V/(complex<double>(2*M_PI,0)*F)).sum() - 
       (a/c) * (V*(complex<double>(2*M_PI,0)*F)).sum());

  return 0;

}
