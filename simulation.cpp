/*****************************************************************************

Title: RLC Estimates for Linear Second Order Systems
Filename: simulation.cpp

Version: 1.0

Created: 16th October 2016
Last Modified: 30th May 2018

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
#include "data.h"

std::valarray<double> real(std::valarray<std::complex<double>> vec)
{
    std::valarray<double> result(vec.size());
    return result;

}

int simulate_data(dsignal<double>& pressure,
		  dsignal<double>& flow,
		  double& small_freq,
		  double& large_freq,
		  double& freq_step
    )
{
  
    /* Stage 1: Generate pressure and flow data
     * Data is generated in the following manner: first, the impedance of 
     * the system is computed in terms of values of R L and C inputted 
     * by the user.
     */
    std::valarray<std::complex<double>> f;
    double R; // Resistance
    double L; // Inertance
    double C; // Compliance

    // User inputs R, L and C
    std::cout << "Input desired values for R, L and C. "
	      << "Type a value and press enter." << std::endl;
    std::cout << "R = ";
    std::cin >> R;
    std::cout << "L = ";
    std::cin >> L;
    std::cout << "C = ";
    std::cin >> C;
  
    std::cout << std::endl;

    // User inputs frequency range
    std::cout << "The presssure and flow data will be "
	      << "computed at specific equally" << std::endl;
    std::cout << "space frequencies. Choose the smallest " 
	      << "and largest frequencies, and the step size." 
	      << std::endl;
    
    //
    // Check that the frequency range divides exactly into a multiple of 
    // the step size. The test computes a floating point division of the frequency
    // interval by the frequency step, and checks that the remainder is effectively
    // zero. I'm sure there must be a better way than this.
    do {
	std::cout << "Smallest Frequency (Hz) = ";
	std::cin >> small_freq;
	std::cout << "Largest Frequency (Hz) = ";
	std::cin >> large_freq;
	std::cout << "Frequency Step (Hz) = ";
	std::cin >> freq_step;

	if(abs(remainder((large_freq - small_freq), freq_step)) > 10e-10) {
	    std::cout << std::endl;
	    std::cout << "Invalid choice of frequency parameters. The step size is not compatible" << std::endl; 
	    std::cout << "with the frequency range. Input new values:" << std::endl;
	    std::cout << std::endl;
	}
    } while (abs(remainder((large_freq - small_freq), freq_step)) > 10e-10);



    std::cout << std::endl;
  
    // Generate an array of frequencies 
    int N(round((large_freq - small_freq)/freq_step)+1);
    f.resize(N);
   
    for(int n=0; n <= N-1; n++)
    {
	f[n] = small_freq + n * freq_step;
    }
    std::valarray<std::complex<double>> omega = 2*M_PI*f;
    
    // Print some of the frequencies. There is a special case to deal with 
    // where the list might contain 4 or less elements. If so, print all the
    // frequencies.
    std::cout << "The impedance Z(f) will be computed at " << N << " frequency point(s):" << std::endl;
 
    if(f.size() <=4 ) {
	for(int n=0; n<f.size(); n++) {
	    std::cout << f[n] << "Hz, "; 
	}
	std::cout << std::endl; 
    } else {
	std::cout << f[0] << "Hz, " << f[1] << "Hz, " 
		  << "..., " << f[N-1] << "Hz." << std::endl;
    }
    std::cout << std::endl;
    // Compute the impedance
    //std::complex<double> Z[N];
    const std::complex<double> j{0,1};
    std::valarray<std::complex<double>> Z = R + j*L*omega - C;
    
    // for(int n=0; n <= N-1; n++)
    // {
    // 	//  Z = R + (j2 pi f)L + C/(j2 pi f)
    // 	Z[n] = std::complex<double>(R, 2*M_PI*f[n]*L - C/(2*M_PI*f[n]));
    // }

    // The input pressure is taken to be the sum of N sinusoids of constant power
    // in the frequency range of interest, with random phases.
    std::valarray<double> pressure_phase(N);
    std::valarray<double> flow_magnitude(N);
    std::valarray<double> flow_phase(N);

    srand(time(NULL));

    // Compute the random phase (between 0 and 2*pi).
    // Not really the best way of doing it but it will serve
    for(int n=0; n <= N-1; n++)
    {
	pressure_phase[n] = (2*M_PI/1000)*(rand() % 1000);
    }
   
    // Compute the magnitudes of the sinusoidal components of
    // the flow 
    for(int n=0; n <= N-1; n++)
    {
	flow_magnitude[n] = std::abs(Z[n]);
    }

    // Compute the phases of the sinusoidal components of
    // the flow 
    for(int n=0; n <= N-1; n++)
    {
	flow_phase[n] = pressure_phase[n] - arg(Z[n]);
    }

    int S; // Number of sampling points
    double samplingFreq; 

    // User inputs the desired number of sampling points
    std::cout << "Input the desired total number of samples in the simulated data:" << std::endl;
    std::cout << "N = ";
    std::cin >> S;
    std::cout << std::endl;
    std::cout << "Input the sampling rate." << std::endl;
    std::cout << "Sampling rate = ";
    std::cin >> samplingFreq;

    std::valarray<double> dvPressure_f;
    dvPressure_f.resize(N);
    std::valarray<double> dvFlow_f;
    dvFlow_f.resize(N);
    //std::valarray<double> pressure;
    pressure.resize(S);
    //std::valarray<double> flow;
    flow.resize(S);

    // Generate samples of the pressure signal
    for(int n=0; n <= S-1; n++)
    {
	// Compute the contributions at each frequency, and put them in an array 
	for(int k=0; k <= N-1; k++)
	{
	    dvPressure_f[k] = sin(omega[k].real()*(n/samplingFreq) - pressure_phase[k]);
	}
	pressure[n] = dvPressure_f.sum();
    }
  
    // Generate samples of the flow signal
    for(int n=0; n <= S-1; n++)
    {
	// Compute the contributions at each frequency, and put them in an array 
	for(int k=0; k <= N-1; k++)
	{
	    dvFlow_f[k] = flow_magnitude[k]*sin(omega[k].real()*(n/samplingFreq) - flow_phase[k]);
	}
	flow[n] = dvFlow_f.sum();
    }
  
    std::cout << std::endl;

    std::cout << "Finished generating pressure and flow data." << std::endl << std::endl;

    // Write the pressure data to a file
    writeFile(pressure, "pressure data");

    // Write the flow data to a file
    writeFile(flow, "flow data");

    return 0;
}
