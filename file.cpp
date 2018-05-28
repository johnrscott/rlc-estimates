/*****************************************************************************

Title: File I/O
Filename: file.cpp

Version: 1.0

Created: 20th October 2016
Last Modified: 20th October 2016

Language: C++

Compilation: 

Dependencies:

Description: Contains functions for reading and writing to the disk. 

*****************************************************************************/

#include <iostream>
#include <valarray>
#include <complex>
#include <fstream>
#include <string>

using namespace std;

template<typename T>
int writeFile(T fileData, string fileLabel)
{
  // Write the FFT data to a file, space seperated
  ofstream file;
  string fileName;
  
  // User inputs desired file names
  cout << "Save " << fileLabel << " as:" << endl;
  cout << "File name = ";
  cin >> fileName;

  // Write the data to a file
  file.open(fileName);
  if(!file.is_open())
    {
      cerr << "Error: Failed to open file for writing.";
      return 1; // Error writing to file.
    }
  for(int n=0; n < fileData.size(); n++)
    {
      file << fileData[n] << endl;
    }
  file.close();
  
  cout << endl;
  
return 0; // File operation successfully completed
}

// Explicit 

template int writeFile<valarray<double> >
(valarray<double> fileData, string fileLabel);

template int writeFile<valarray<complex<double> > >
(valarray<complex<double> > fileData, string fileLabel);
