////////////////////////////////////////////////////////////////////////
//                                                                    //
//    Author:             Matthew Lingenfelter                        //
//    Creation Date:      September 24, 2020                          //
//    Purpose:                                                        //
//       This program reads, calculates, and outputs general          //
//       information for a list of countries.                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "Country.h"

// read the data file, store in arrays
int readDatafile(Country[], int);
void outputList(Country[], int);
void sortAscend(Country[], int);
void sortDescend(Country[], int);
void swap(Country &, Country &);

int main() {
  static int arraysize= 10;  // size of Country array
  int numC=0;                // the number of countries read
  Country clist[arraysize];

  //calls to the function readDatafile to open and read the file, and returns the number of countries in the file
  numC = readDatafile(clist, arraysize);

  //calls to the function outputList to output all the information read from the file
  outputList(clist, numC);

  //calls to the sortAscend function to sort the countries by name alphebetically
  sortAscend(clist, numC);

  //outputs a header to show it was sorted and calls to outputList to output all the information of the countries again
  cout << "\nSorted Alphabetically:\n----------------------\n";
  outputList(clist, numC);

  //calls to the sortDescend function to sort the countries by GDP starting with the highest
  sortDescend(clist, numC);

  //outputs a header to show it was sorted and calls to outputList to output all the information of the countries again
  cout <<"\nSorted by per-capita CGP:";
  cout <<"\n-------------------------\n";
  outputList(clist, numC); // output again
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   readDatafile                                  //
//    Description:     Gets the file name from the user, opens       //
//                       the file, and stores the data.              //
//    Parameters:      country list[] - array of structs containing  //
//                       all information for each country            //
//                     int max - maximum array size                  //
//    Return Value:    int count - the number of countries read      //
//                                                                   //
///////////////////////////////////////////////////////////////////////

int readDatafile(Country list[], int max) {
  string fname;
  ifstream ifs;
  int count = 0;
  
  //prompts the user for the name of the file
  cout << "Entry the name of the file: ";
  cin >> fname;

  //attemps to open the file
  ifs.open(fname.c_str());

  //checks if the file opened properly
  if(ifs.fail()) {
    //outputs an error if the file didn't open
    cout << "\nError opening file.\n" << endl;
    exit(EXIT_SUCCESS);
  }

  //reads each peice of data from the file and stores them in an array
  while(count < max && ifs >> list[count]) {
    count++;
  }

  //closes the file
  ifs.close();

  return count;
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   outputData                                    //
//    Description:     Outputs all information about each country    //
//                       as it was from the file.                    //
//    Parameters:      country list[] - array of structs containing  //
//                       all information for each country            //
//                     int max - maximum array size                  //
//    Return Value:    none - outputs a list of data                 //
//                                                                   //
///////////////////////////////////////////////////////////////////////

void outputList(Country list[], int max) {
  //outputs all raw data for each country
  for(int i = 0; i < max; i++) {
    cout << list[i];
  }
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   sortAscend                                    //
//    Description:     Sorts the countries in the array by their     //
//                       name.                                       //
//    Parameters:      Country list[] - the array of country info    //
//                     int max - the number of countries in the array//
//    Return Value:    none - sorts the countries by name            //
//                                                                   //
///////////////////////////////////////////////////////////////////////

void sortAscend(Country list[], int max) {
  bool flag = true;
  //loops for one less than the number of countries
  for(int i = 1; (i <= max) && flag; i++) {
    flag = false;
    //loops again for each country
    for(int j = 0; j < (max - 1); j++) {
      //checks if the country's name comes befor the second country's
      if(list[j+1] < list[j]) {
        //calls to the function swap to swap the positions of the countries
        swap(list[j], list[j+1]);
        //sets the flag to true to show a swap happended
        flag = true;
      }
    }
  }
}

void sortDescend(Country list[], int max) {
  bool flag = true;
  //loops for one less than the number of countries
  for(int i = 1; (i <= max) && flag; i++) {
    flag = false;
    //loops again for each country
    for(int j = 0; j < (max - 1); j++) {
      //checks if the country's name comes befor the second country's
      if(list[j+1] > list[j]) {
        //calls to the function swap to swap the positions of the countries
        swap(list[j], list[j+1]);
        //sets the flag to true to show a swap happended
        flag = true;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   swap                                          //
//    Description:     Swaps the position of two countries info      //
//                        in the array list[]                        //
//    Parameters:      Country &a - reference to the first country   //
//                     Country &b - reference to the second country  //
//    Return Value:    none - swaps the position in the array        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

void swap(Country &a, Country &b) {
  Country temp = a;
  a = b;
  b = temp;
}
