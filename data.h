/*****************************************************************************

Title: Data types
Filename: data.h

Version: 1.0

Created: 25th October 2016
Last Modified: 30th October 2018

Language: C++ header

******************************************************************************/

#ifndef DATA_H // Protection against multiple inclusion
#define DATA_H

#include <valarray>
#include <complex>

// This class is designed to contain a digital signal. The dependent
// variable is stored as an array and the independent variable is assumed
// to have a fixed step size. The type can be used to store time domain
// or frequency domain data.
//
// The question is whether it is better to store the independent variable
// as an array (faster access, more data usage), or compute values of the
// independent variable every time they are needed. The second method
// doesn't generalise if there isn't some rule which generates the
// independent variable
//
// By default, operator[] accesses the the dependent variable array. Is
// that a good idea? 
//

//using namespace std; // Is it a good idea putting these in header files?

template<typename T>
class dsignal
{
private:
    std::valarray<T> data;
    double indep_start;
    double indep_end;
    double indep_step;
    
public:
    dsignal(){}
    // Resize the signal array (does this make sense?) 
    int size(void) { return data.size(); }
    int resize(int N) { data.resize(N); return 0; }
    // Get the independent/dependent variable values at a point
    int get_x(int N, double& value) { value = indep_start + N*indep_step; return 0; }
    T operator[](int N) const { return data[N]; }
    T& operator[](int N) { return data[N]; } // Assignment version -- returns an r-value for assignment
    // Get the start/end/step of the independent variable
    int get_xstart(int N, double& value) { value = indep_start; return 0; }
    int get_xend(int N, double& value) { value = indep_end; return 0; }
    int get_xstep(int N, double& value) { value = indep_step; return 0; }
    // It would be good to have a description of the data stored in the type for debugging purposes.
    // int description(char * string) {}
};

#endif
