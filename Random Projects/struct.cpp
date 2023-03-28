////////////////////////////////////////////////////////////////////////
//                                                                    //
//    Author:             Matthew Lingenfelter                        //
//    Creation Date:      September 4, 2020                           //
//    Purpose:                                                        //
//       This program reads, calculates, and outputs general          //
//       information for a list of countries.                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// struct definition for a child object
struct country {
  string name;           // country name
  float  GDP;            // gross domestic product (billions)
  float  population;     // population 
  char   scale;          // population scale
  float  lifeExpectancy; // Life Expectancy (years)
  float  perCapita;      // per-Capita GDP
};                  

// prototypes
int readDatafile(country[], int);
void outputData(country[], int);
void calcPerCapita(country[], int);
void outputResults(country[], int);
void alphOrder(country[], int);

// additional prototypes for sorting

int main() {
  static int ARRAYSIZE= 10;
  country clist[ARRAYSIZE];
  
  //calls to open, read, and store the data from the file
  readDatafile(clist, ARRAYSIZE);
  
  //calls to output the raw data from the file
  outputData(clist, ARRAYSIZE);

  //calls to calculate the per-capita GDP
  calcPerCapita(clist, ARRAYSIZE);
  
  //calls to ouput the data again, now including the per-capita GDP
  outputResults(clist, ARRAYSIZE);

  //calls to reorder the data into alphabetical order
  alphOrder(clist, ARRAYSIZE);
  
  //calss to output the data one last time, now in alphabetical order
  outputResults(clist, ARRAYSIZE);

}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   readDatafile                                  //
//    Description:     Gets the file name from the user, opens       //
//                       the file, and stores the data.              //
//    Parameters:      country list[] - array of structs containing  //
//                       all information for each country            //
//                     int max - maximum array size                  //
//    Return Value:    none - saves all data                         //
//                                                                   //
///////////////////////////////////////////////////////////////////////

int readDatafile(country list[], int max) {
  string fname;
  ifstream ifs;
  int num=0;

  //promts the user for the name of the file
  cout << "Enter the file name: ";
  cin >> fname;
  
  //attemps to open the file
  ifs.open(fname.c_str());

  //checks if the file opened properly
  if(ifs.fail()) {
    //outputs an error if the file didn't open
    cout << endl << "Error opening file." << endl << endl;
    exit(EXIT_SUCCESS);
  }

  //reads each peice of data from the file and stores them in an array of structs
  int count = 0;
  while(count < max && ifs >> list[count].name) {
    ifs >> list[count].GDP;
    ifs >> list[count].population;
    ifs >> list[count].scale;
    ifs >> list[count].lifeExpectancy;
    count++;
  }

  return num;
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

void outputData(country list[], int max) {
  cout << endl;
  cout << "Country data:" << endl;
  cout << "-------------" << endl;
  
  //outputs all raw data for each country
  for(int i = 0; i < max; i++) {
    cout << left << setw(8) << list[i].name;
    cout << right << setw(7) << fixed << setprecision(2) << list[i].GDP;
    cout << setw(8) << setprecision(1) << list[i].population;
    cout << " " << list[i].scale;
    cout << setw(7) << list[i].lifeExpectancy << endl;
  }
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   calcPerCapita                                 //
//    Description:     Calulated the per-capita GDP for each country //
//                       and converts each population to millions.   //
//    Parameters:      country list[] - array of structs containing  //
//                       all information for each country            //
//                     int max - maximum array size                  //
//    Return value:    none - calulates and stores the GDP           //
//                                                                   //
///////////////////////////////////////////////////////////////////////

void calcPerCapita(country list[], int max) {
  //checks if the country's population is in billions or millions
  for(int i = 0; i < max; i++) {
    if(list[i].scale == 'b') {
      //converts the billions population to millions
      list[i].population = list[i].population * 1000;
    }
    //computes the per-capita GDP
    list[i].perCapita = (list[i].GDP / list[i].population) * 1000000;
  }
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   outputResults                                 //
//    Description:     Outputs information for each country with the //
//                       calculated GDP and population values.       //
//    Paremeters:      country list[] - array of structs containing  //
//                       all information for each country            //
//                     int max - maximum array size                  //
//    Return Value:    none - outputs a list of data                 //
//                                                                   //
///////////////////////////////////////////////////////////////////////

void outputResults(country list[], int max) {
  cout << endl;
  cout << "Country data:" << endl;
  cout << "-------------" << endl;

  //outputs all the refined data for each country
  for(int i = 0; i < max; i++) {
    cout << left << setw(8) << list[i].name;
    cout << right << setw(7) << fixed << setprecision(2) << list[i].GDP;
    cout << setw(10) << setprecision(1) << list[i].population;
    cout << setw(7) << list[i].lifeExpectancy;
    cout << setw(8) << setprecision(0) << list[i].perCapita << endl;
  }
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   alphOrder                                     //
//    Description:     Sets the information for each country to      //
//                    alphabetical order based on the country's name //
//    Paremeters:      country list[] - array of structs containing  //
//                       all information for each country            //
//                     int max - maximum array size                  //
//    Return Value:    none - reorders the data of each country      //
//                                                                   //
///////////////////////////////////////////////////////////////////////

void alphOrder(country list[], int max) {
  country temp;
  float ftemp;
  bool flag = true;
  //loops for every element in the array, but stops if no swaps occur
  for(int i = 1; (i <= max) && flag; i++) {
    flag = false;
    //loops for every element in the array, not counting the last one
    for(int j = 0; j < (max-1); j++) {
      //checks if the name of one country comes before the next cournty's name in alphabetical order
      if(list[j].name > list[j+1].name) {
        //swaps the position in the array of the names of the two countries
        temp = list[j]; list[j] = list[j+1]; list[j+1] = temp;
        flag = true;
      }
    }
  }
}
