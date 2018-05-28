/*****************************************************************************

Title: File I/O Header File
Filename: file.h

Version: 1.0

Created: 20th October 2016
Last Modified: 20th October 2016

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

template<typename T>
int writeFile(T fileData, string fileLabel);

