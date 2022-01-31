#ifndef SONG_H
#define SONG_H

#include <string>
using namespace std;

// struct definition for a child object
class Country {
public:

  // constructor
  Country(string n="", float g=0, float p=0, char s='m', int c=0);

  // Mutators (modifiers)
  void setName(string n) { name= n; }
  void setGDP(float g) { GDP= g; }
  void setPopulation(float p) { population= p; }
  void setPopScale(float s) { popScale= s; }
  void setContinent(int c) { continent= c; }

  // inspectors (accessors)
  string getName() const { return name; }
  float  getGDP() const { return GDP; }
  float  getPopulation() const { return population; }
  char   getPopScale() const { return popScale; }
  int    getContinent() const { return continent; }

  // compute per-capita GDP
  float getPerCapitaGDP() const;
  string getContinentName() const;

  // overloads
  // greater than - compares life expectancy
  bool operator>(const Country &) const; // greater than
  // less than - compares country name
  bool operator<(const Country &) const; // less than
  // += add more GDP
  Country &operator+=(float);            // add some GDP

private:
  string name;        // country name
  float  GDP;         // gross domestic product (trillions)
  float  population;  // population 
  char   popScale;    // population scale
  int    continent;   // Continent number
  
  // static members
  static const string Continents[]; // Continent names
};                  

// overload of external stream operators
ostream &operator<<(ostream &, const Country &);
istream &operator>>(istream &, Country &);

#endif
