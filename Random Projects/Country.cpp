////////////////////////////////////////////////////////////////////////
//                                                                    //
//    Author:             Matthew Lingenfelter                        //
//    Creation Date:      September 24, 2020                          //
//    Purpose:                                                        //
//       This program computs the GDP per person per country, gets    //
//       the name of the continent, defines the overflow >> operator, //
//       the > operator, the < operator, and the += operator.         //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

using namespace std;

#include "Country.h"

const string Country::Continents[] = {"Asia", "Africa", "Australia", "Antarctica", "Europe", "North America", "South America"};

// constructor
Country::Country(string n, float g, float p, char s, int c) {
  name= n;            // country name
  GDP= g;             // gross domestic product (billions)
  population= p;      // population 
  continent= c;       // Continent Code
  popScale= (s != 'b' && s != 'm') ? 'm' : s;  // default to "m"
}

// istream operator overload
istream &operator>>(istream &input, Country &C) {

  string n; float g; float p; char s; int c;
  
  input >> n >> g >> p >> s >> c;

  C.setName(n);
  C.setGDP(g);
  C.setPopulation(p);
  C.setPopScale(s);
  C.setContinent(c);

  return input;   // enables cascading
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   getPerCapitaGDP                               //
//    Description:     Calculats the per-capita GDP                  //
//    Parameters:      none                                          //
//    Return Value:    float perCapita - the calculated per-capita   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

float Country::getPerCapitaGDP() const{
  float pop = 0;
  
  //checks if the population scale is in billions
  if(popScale == 'b') {
    //converts the billions population to millions and assigns it to the pop variable
    pop = population * 1000;
  }
  else {
    //assigns the population amount to the pop variable
    pop = population;
  }

  //computes the per-capita GDP
  float perCapita = (GDP / pop) * 1000000;
  return perCapita;
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   getContinentName                              //
//    Description:     Returns the name of the continent that the    //
//                      country is in based off its continent number.//
//    Parameters:      none                                          //
//    Return Value:    string cont - name of the continent           //
//                                                                   //
///////////////////////////////////////////////////////////////////////

string Country::getContinentName() const {
  //sets cont to the name of the continent
  string cont = Continents[continent];
  return cont;
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   ostream &operator<<                           //
//    Description:     ostream operator overload                     //
//    Parameters:      ostream &output - reference to the ouput      //
//                       operator called                             //
//                     const Country&C - reference to the country    //
//                       geting called                               //
//    Return Value:    none - outputs the country information        //
//                                                                   //
///////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &output, const Country &C) {
  //gets all the information about the country
  string n = C.getName();
  float g = C.getGDP();
  float p = C.getPopulation();
  char s = C.getPopScale();
  string c = C.getContinentName();
  int pc = C.getPerCapitaGDP();

  //prints out the information for each country in colums
  output << left << setw(8) << n;
  output << right << setw(6) << g;
  output << setw(8) << p << s;
  output << setw(7) << pc;
  output << left << "  " << c << endl;
  return output;
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   bool operator<                                //
//    Description:     less than operator overload                   //
//    Parameters:      const Country &F - reference to the second    //
//                       country                                     //
//    Return Value:    true - if the second country name comes after //
//                     false - if the first country name comes after //
//                                                                   //
///////////////////////////////////////////////////////////////////////

bool Country::operator<(const Country&F) const {   
  //checks if the second country name comes after the first
  if(getName() < F.getName())
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   bool operator>                                //
//    Description:     greater than operator overload                //
//    Parameters:      const Country &F - reference to the second    //
//                       country                                     //
//    Return Value:    true - if the first country's per-capita GDP  //
//                       is higher that the second                   //
//                     false - if the second country's per-capita    //
//                       GDP is higher than the first                //
//                                                                   //
///////////////////////////////////////////////////////////////////////

bool Country::operator>(const Country&F) const {
  //checks if the second country name comes after the first
  if(getPerCapitaGDP() > F.getPerCapitaGDP())
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////
//                                                                   //
//    Function name:   operator+=                                    //
//    Description:     += operator overload                          //
//    Parameters:      float flt - the number being passed in to be  //
//                       added as a float                            //
//    Return Value:                                                  //
//                                                                   //
///////////////////////////////////////////////////////////////////////

Country &Country::operator+=( float flt ) {
  //adds the float value to the country's GDP
  float newGDP = getGDP() + flt;
  //sets the country's GDP as the new value
  setGDP(newGDP);
  return *this;
}
