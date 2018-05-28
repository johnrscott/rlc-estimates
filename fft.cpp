/*****************************************************************************

Title: Fast Fourier Transform
Filename: fft.cpp

Version: 1.0

Created: 18th October 2016
Last Modified: 28th May 2018

Language: C++

Compilation: make -k

Description: This file contains a function which computes the radix 2 Fast 
Fourier Transform of an input dataset using decimation in time. 

The Discrete Fourier Transform of a sequence x[n], where n = 0, 1, ..., N-1, 
is given by the following sum:


        N-1
        ___  
X[k] =  \    x[n] e^(-j2 pi nk/N),             k = 0, 1, ..., N-1, 
        /                                                      
        ^^^
        n=0

According to the formula above, a 2-point Discrete Fourier Transform is given
by the following operation (called a Butterfly operation)

X[0] = x[0] + x[1]
X[1] = x[0] - x[1]

Further, it is possible to rearrange the above formula so that an N-point
Discrete Fourier Transform is expressed in terms of 2 (N/2)-point Discrete 
Fourier Transforms. If N is a power of two, an N-point DFT can therefore
be computed entirely in terms of Butterfly operations.

The decimation-in-time algorithm invloves mixing up the order of the input
data array, so that the butterfly operations can be computed in-place. Then
the output data array is correctly ordered in frequency.

There are a couple of critical routines that are required for implementing 
the FFT. One is the butterfly operation. The other is the ability to bit 
reverse addresses so that the next set of butterfly operations can be
performed. It's not really clear to me the best way to do that in C++. There
is a routine for it here, but I'm not really convinced it's that good. I 
expect Intel architectures have instructions for bit reversed memory 
accesses. But perhaps a lookup table is better.

*****************************************************************************/

#include "fft.h"

using namespace std;

int butterfly(complex<double> in_p, 
	      complex<double> in_q, 
	      complex<double>& out_p, 
	      complex<double>& out_q, 
	      int r, 
	      complex<double> W_N)
/* The Butterfly Operation  */
{
  // Compute the Butterfly operation
  out_p = in_p + pow(W_N,r)*in_q;
  out_q = in_p - pow(W_N,r)*in_q; 

  return 0; // meaning data in output is valid
}

int bitReverse(int input, int N)
/* Bit reverse input, with respect to N bits.
   e.g. if N=4, 1010 -> 0101 
*/
{
  bitset<64> input_b; // input bit field
  bitset<64> output_b; // output bit field
  input_b = input;
  //cout << input_b << endl;
  for(int k=0; k < N; k++)
    {
      output_b[N-k-1] = input_b[k];
    }
  int output = output_b.to_ulong();

  return output; // output contains bit reversal of input.
}

// There's a reason for using templates here, even though it makes it
// look complicated -- you want to be able to compute the FFT with data
// which is both real and complex, which requires two different input 
// types. (I found that out by getting compile errors when I tried to
// delete the template!)
template<typename T>
int fft_dit(T input, valarray<complex<double> >& output)
{
  int N(input.size()); // Length of input.
  const complex<double> W_N(cos(2*M_PI/N),-sin(2*M_PI/N));
 
  // Check whether the input length is a power of 2.
  if((log2(N)-static_cast<int>(log2(N))) != 0)
    {
      cerr << "Error: N must be a power of 2." << endl;
      return 1; // N must be a power of two.
    }

  output.resize(N);

  // Compute the number of stages.
  int S = log2(N);

  // Order the input data by bit reversing the addresses
  valarray<complex<double> > in;
  in.resize(N);
  for(int n=0; n < N; n++)
    {
      // The point here is to put the data back into the in
      // array, but in the correct order for the butterfly 
      // operations.
      in[n] = input[bitReverse(n,S)];
    }
  
  // Compute the S = log2(N) stages of the DFT. 
  for(int s=0; s < S; s++)
    {
      // At stage s, there are 2^s values for r, given by r = 2^(S-s-1)k for 
      // k = 0, 1, ... such that r does not exceed N/2. 

      // Each value of r causes the Butterfly operations to be offset by
      // 1; define the offset
      int n = 0;

      for(int r=0; r < N/2; r += pow(2,S-s-1))
	{
	  // For a given value of r, there correspond 2^(S-s-1) Butterfly 
	  // operations: (using un-bit-reversed indexing) (2^s)k with 
	  // (2^s)(k+1), (2^s)(k+2) with (2^s)(k+3), ..., not exceeding 
	  // N.
	  for(int k=0; (pow(2,s))*k < N; k+=2)
	    {
	      // Compute the butterfly operations.
	      butterfly(in[(pow(2,s))*k+n],
			in[(pow(2,s))*(k+1)+n],
			output[(pow(2,s))*k+n],
			output[(pow(2,s))*(k+1)+n],
			r,W_N);
	    }
	  // Increment the offset
	  n++;
	}
      
      // Copy the output data register from the current stage to the input data
      // register for the next.
      in = output;

    }
  return 0;
}

template int fft_dit<valarray<complex<double > > >
(valarray<complex<double > > input, valarray<complex<double> >& output);

template int fft_dit<valarray<double > >
(valarray<double > input, valarray<complex<double> >& output);

