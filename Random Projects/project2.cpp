/****************************************/
/* Author:         Matthew Lingenfelter */
/* Creation Date:  March 6, 2020        */
/****************************************/

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main () 
{
  //Declaring variables
  int account, water;
  char code;
  float amount, multiplier;
  string service;

  //Gets the account number from user
  cout << "Enter the account number: ";
  cin >> account;
  //Gets the service code from user
  cout << "Enter the service code: ";
  cin >> code;
  //Gets the amount of water used from user
  cout << "Enter the amount of water used (in gallons): ";
  cin >> water;

  //Checks if the home code is used
  if (code == 'h') {
    service = "Home";
    amount = 10;
    if (water > 100 && water < 200) {
      multiplier = water - 100;
      multiplier *= 0.05;
      amount += multiplier;
    }
    else if (water > 200) {
      amount += 5;
      multiplier = water - 200;
      multiplier *= 0.02;
      amount += multiplier;
    }
  }
  //checks if the commercial code is used
  else if (code == 'c') {
    service = "Commercial";
    amount = 500;
    if (water > 1000 && water < 2500) {
      multiplier = water - 1000;
      multiplier *= 0.08;
      amount += multiplier;
    }
    else if (water > 2500) {
      amount += 120;
      multiplier = water - 2500;
      multiplier *= 0.06;
      amount += multiplier;
    }
  }
  else if (code == 'i') {
    service = "Industrial";
    if (water < 5000)
      amount = 1500;
    else {
      multiplier = water - 5000;
      multiplier *= 0.07;
      amount += multiplier;
    }
  }
  else
    cout << endl << "Service code is invalid." << endl;

  //Outputs the water bill calculator table
  cout << endl << "Water Bill Calculator" << endl;
  cout << "=========================================" << endl;
  cout << "Account number:" << setw(11) << " " << account << endl;
  cout << "Service:" << setw(18) << " " << service << endl;
  cout << "Gallons of water used:" << setw(4) << " "  << water << endl;
  cout << "Amount due:" << setprecision(2) << fixed << setw(16) << " $" << amount << endl;
}
