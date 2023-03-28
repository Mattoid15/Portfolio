////////////////////////////////////////////////////////////////////////
//                                                                    //
//    Author:             Matthew Lingenfelter                        //
//    Creation Date:      September 3, 2020                           //
//    Purpose:                                                        //
//       This program reads, calculates, and outputs general          // 
//       information for a list of countries.                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int readDatafile(string[], float[], float[], char[], float[], int);
void outputData(string[], float[], float[], char[], float[], int);
void calcPerCapita(float[], float[], char[], float[], int);
void outputResults(string[], float[], float[], float[], float[], int);
void alphOrder(string[], float[], float[], float[], float[], int);

int main() {
  static int ARRAYSIZE= 10;
  string name[ARRAYSIZE];           // country name
  float  GDP[ARRAYSIZE];            // gross domestic product (billions)
  float  population[ARRAYSIZE];     // population 
  char   scale[ARRAYSIZE];          // population scale
  float  lifeExpectancy[ARRAYSIZE]; // Life Expectancy (years)
  float  perCapita[ARRAYSIZE];      // per-Capita GDP

  //calls to open, read, and store the data from the file
  readDatafile(name, GDP, population, scale, lifeExpectancy, ARRAYSIZE);

  //calls to output the raw data from the file
  outputData(name, GDP, population, scale, lifeExpectancy, ARRAYSIZE);

  //calls to calculate the per-capita GDP
  calcPerCapita(GDP, population, scale, perCapita, ARRAYSIZE);

  //calls to output the data again, now including the per-capita GDP
  outputResults(name, GDP, population, lifeExpectancy, perCapita, ARRAYSIZE);

  //calls to reorder the data into alphebetical order
  alphOrder(name, GDP, population, lifeExpectancy, perCapita, ARRAYSIZE);

  //calls to output the data one last time, now in alphebetical order
  outputResults(name, GDP, population, lifeExpectancy, perCapita, ARRAYSIZE);
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//    Function name:   alphOrder                                           //
//    Description:     Sets the information stored in each array to        //
//                       alphebetical order based on the countries name.   //
//    Parameters:      string nm[] - array of names for each country       //
//                     float gdp[] - gdp array of each country             //
//                     float pop[] - population array of each country      //
//                     float le[] - life expextancy array for each country //
//                     float prc[] - calculated GDP array for each country //
//                     int max - maximum array size                        //
//    Return Value:    none - reorders the elements in each array          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void alphOrder(string nm[], float gdp[], float pop[], float le[], float prc[], int max) {
  string temp;
  float ftemp;
  bool flag = true;
  
  //loops for every element in the array, but stops if no swaps occurr
  for(int i = 1; (i <= max) && flag; i++) {
    flag = false;
    
    //loops for every element in the array, not counting the last one
    for(int j = 0; j < (max-1); j++) {
      //checks if the name of one country comes before the next country's name in alphebetical order
      if(nm[j] > nm[j+1]) {
        //swaps the position in the array of all the information of the two countries
        temp = nm[j]; nm[j] = nm[j+1]; nm[j+1] = temp;
        ftemp = gdp[j]; gdp[j] = gdp[j+1]; gdp[j+1] = ftemp;
        ftemp = pop[j]; pop[j] = pop[j+1]; pop[j+1] = ftemp;
        ftemp = le[j]; le[j] = le[j+1]; le[j+1] = ftemp;
        ftemp = prc[j]; prc[j] = prc[j+1]; prc[j+1] = ftemp;
        
        //sets flag to true to indicate that a swap occurred
        flag = true;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//    Funtion name:  readDatafile                                        //
//    Desreiption:   Gets the file name from the user,                   //
//       opens the file, and stores the data in arrays.                  //
//    Parameters:    string nm[] - array of names for each country       //
//                   float gdp[] - gdp array of each country             //
//                   float pop[] - population array of each country      //
//                   char sc[] - population scale array for each country //
//                   float le[] - life expextancy array for each country //
//                   int max - maximum array size                        //
//    Return Value:  none - but saves all the data in arrays             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

int readDatafile(string nm[], float gdp[], float pop[], char sc[], float le[], int max) {
  string fname;
  ifstream ifs;
  int num=0;

  // prompts the user for the data file name
  cout << "Enter the name of the data file: ";
  cin >> fname;

  //attempts to open file
  ifs.open(fname.c_str());

  //checks if the file did not open
  if(ifs.fail()) {
    //outputs an error message due to the file not opening
    cout << endl << "Error opening file." << endl << endl;
    exit(EXIT_SUCCESS);
  }

  //reads each peice of data from the file and stores them
  int count = 0;
  while(count < max && ifs >> nm[count]) {
    ifs >> gdp[count];
    ifs >> pop[count];
    ifs >> sc[count];
    ifs >> le[count];
    count++;
  }
  return num;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//    Function name:  calcPerCapita                                       //
//    Description:    Calculates the per-capita GDP for each country and  //
//                      converts each population to millions.             //
//    Parameters:     float gdp[] - gdp array of each country             //
//                    float pop[] - population array of each country      //
//                    char sc[] - population scale array for each country //
//                    float extra[] - array of the calculated GDP for     //
//                                      each country                      //
//                    int max - maximum array size                        //
//    Return value:   none - calculates and stores the GDP in the         //
//                             perCapita array                            //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void calcPerCapita(float gdp[], float pop[], char sc[], float extra[], int max) {
  //checks if the country's population is in billions or millions
  for(int i = 0; i < max; i++) {
    if(sc[i] == 'b') {
      //converts the billions population to millions
      pop[i] = pop[i] * 1000;
    }
    //computes the per-capita GDP
    extra[i] = (gdp[i] / pop[i]) * 1000000;
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//    Function name:   outputData                                          //
//    Description:     Outputs all the information about each country      //
//                       as it was from the file.                          //
//    Parameters:      string nm[] - array of names for each country       //
//                     float gdp[] - gdp array of each country             //
//                     float pop[] - population array of each country      //
//                     char sc[] - population scale array for each country //
//                     float le[] - life expextancy array for each country //
//                     int max - maximum array size                        //
//    Return value:    none - outputs a list of data                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void outputData(string nm[], float gdp[], float pop[], char sc[], float le[], int max) {
  cout << endl;
  cout << "Country data:" << endl;
  cout << "-------------" << endl;
  
  //outputs all raw data for each country
  for(int i = 0; i < max; i++) {
    cout << left << setw(8) << nm[i];
    cout << right << setw(7) << fixed << setprecision(2) << gdp[i];
    cout << setw(8) << setprecision(1) << pop[i];
    cout << " " << sc[i];
    cout << setw(7) << le[i] << endl;
  }
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//    Function name:   outputResults                                        //
//    Description:     Outputs information for each country with the        //
//                       calcualed GDP and population values.               //
//    Parameters:      string nm[] - array of names for each country        //
//                     float gdp[] - gdp array of each country              //
//                     float pop[] - population array of each country       //
//                     float le[] - life expextancy array for each country  //
//                     float perc[] - calculated GDP array for each country //
//                     int max - maximum array size                         //
//    Return Value:    none - outputs a list of data                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

void outputResults(string nm[], float gdp[], float pop[], float le[], float perc[], int max) {
  cout << endl;
  cout << "Country data:" << endl;
  cout << "-------------" << endl;

  //outputs all the refined data for each country
  for(int i = 0; i < max; i++) {
    cout << left << setw(8) << nm[i];
    cout << right << setw(7) << fixed << setprecision(2) << gdp[i];
    cout << setw(10) << setprecision(1) << pop[i];
    cout << setw(7) << le[i];
    cout << setw(8) << setprecision(0) << perc[i] << endl;
  }
}
