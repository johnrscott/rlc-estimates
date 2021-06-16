/*****************************************************************************

Title: RLC Estimates for Linear Second Order Systems
Filename: rlc-estimates.cpp

Version: 1.0

Created: 16th October 2016
Last Modified: 2nd June 2018

Language: C++

Compilation: g++ -std=c++11 rlc-estimate.cpp simulation.cpp

Description: The object of the following program is to implement the scheme
outlined in '1979 Tsai and Pimmel - Computation of Respiratory Resistance,
Compliance and Inertance from Forced Oscillatory Impedance Data'. The paper
describes a power spectrum based method for computing the coefficients of a
second order linear differential equation from its input and output. The 
equation is

L V''(t) + R V'(t) + C V(t) = P(t).                                     (1)

In this equation, P(t) is interpreted as the input and V'(t) is interpreted 
as the output. In the context of their paper, Tsai and Pimmel interpret 
P(t) as the pressure and V'(t) as the airflow at the airway opening of the
respiratory system. Then L represents the inertia of the air; R represents
the resistance of the airways; and C represents the compliance of the 
airways and alveoli. 

P(t) and V'(t) are given because they have been measured. The object is to
determine the values of L, R and C. Tsai and Pimmel's method is to take 
the Laplace transform of the equation, and express the right hand left hand
side in terms of the Laplace transform of V'(t):

L Lap[V''(t)] + R Lap[V'(t)] + C Lap[V(t)] = Lap[P(t)],

which can be written 

L s Lap[V'(t)] + R Lap[V'(t)] + (C/s) Lap[V'0(t)] = Lap[P(t)],

using the rules from the Laplace transforms of derivatives. The ratio of
Laplace transforms Lap[V(t)]/Lap[P(t)] is called the transfer function of 
(1). The ratio Lap[V'(t)]/Lap[P(t)] is called the impedance. (In 
electronics, where equation (1) represents a series RLC circuit, V 
corresponds to the charge, so V' corresponds to the current. Consequently 
the impedance is the ratio of voltage to current.) The impedance is 
therefore given by 

Z   =   Ls + R + C/s,

    =   R + jwL + C/jw.                                             

    =   R + (j2 pi f)L + C/(j2 pi f)                                 (2)

Z can also be calculated by other means, directly from the measured V'(t)
and P(t), by a ratio of Fourier transforms, or power spectra, or some 
related method. Tsai and Pimmel use least mean squares to find the values
of R, L and C which cause equation (2) to approximate as closely as 
possible the empirical impedance. Therefore there are two parts to the 
procedure: firstly, estimate the empirical impedance; and secondly, 
use least mean squares to find the values of R, L and C. The latter 
is treated first. 

Suppose that the real and imaginary parts of the impedance have been found
to be U(f_i) and V(f_i), taking values at the discrete set of frequencies 
f_i, i = 1, 2, ..., N. Define the error criterion to be the sum of squares:

      N
     ___                                                             2
J =  \    | [U(f_i) + jV(f_i)] - [R + (j2 pi f_i)L + C/(j2 pi f_i)] |
     /    |                                                         |
     ^^^
     i=1

This expression is just the sum of the squares of the differences between
the empirical impedance and the theoretical impedance at each frequency 
f_i. Tsai and Pimmel gives  formulas for the values of R, L and C which 
minimise J:

           N
          ___   
R =  1/N  \   U(f_i)  
          /    
          ^^^
          i=1


           N                            N
          ___                          ___
L =  N/c  \   V(f_i)/(2 pi f_i) - a/c  \    (2 pi f_i) V(f_i)
          /                            /
          ^^^                          ^^^
          i=1                          i=1


           N                            N
          ___                          ___
E =  b/c  \   V(f_i)/(2 pi f_i) - N/c  \    (2 pi f_i) V(f_i)
          /                            /
          ^^^                          ^^^
          i=1                          i=1


where a, b and c are defined by 


       N
      ___             
a =   \   1/(2 pi f_i)^2  
      /    
      ^^^
      i=1


       N
      ___             
b =   \   (2 pi f_i)^2  
      /    
      ^^^
      i=1


c = N^2 - ab.

In order to apply the above formulae, the impedance must be estimated at the 
discrete frequencies f_i. The estimate is obtained by computing the power
spectrum of several adjacent windows of V'(t) and P(t), taking their ratio, 
and averaging over the windows. 

******************************************************************************/

#include "simulation.h"
#include "estimation.h"
#include "data.h"

using namespace std;

complex<double> realPart(complex<double> v) { return v.real(); }
complex<double> imagPart(complex<double> v) { return v.imag(); }

int main(int argc, char* argv[]) 
{
  dsignal<double> p;
  dsignal<double> f;
  dsignal<double> pressure;
  dsignal<double> flow;
  double SmallFreq;
  double LargeFreq;
  double FreqStep;

  // This routine prompts the user for values of R, L and C and
  // generates pressure and flow data containing a range of frequencies
  // ready for analysis. The frequency range entered by the user is
  // in the variables SmallFreq, LargeFreq and FreqStep, which are 
  // passed by reference to the function.
  int result = simulate_data(pressure, 
			     flow, 
			     SmallFreq, 
			     LargeFreq, 
			     FreqStep);
  if(result != 0)
    {
	  cerr << "Error: Simulation failed" << endl;
      return 1;
    }

  double R, L, C;
  // result = estimate_rlc(pressure, 
  // 			flow, 
  // 			SmallFreq, 
  // 			LargeFreq, 
  // 			FreqStep, 
  // 			R, L, C);

  // Print estimates
  cout << "Resistance estimate: "<< R << endl;
  cout << "Compliance estimate: "<< C << endl;
  cout << "Inertance estimate: "<< L << endl;

  return 0;
}
