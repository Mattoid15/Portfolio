/******************************************************/
/* Author:               Matthew Lingenfelter         */
/* Creation Date:        April 29, 2020               */
/* Purpose:              This program will read stocks*/
/*                       data and suggest which ones  */
/*                       to buy.                      */
/******************************************************/

#include <iostream>
#include <fstream>

using namespace std;

float readFile(string);
string getName();

int main()
{
  string name;

  //gets the file name from the user
  name = getName();
  //fp.open(name.c_str());
  readFile(name);

  return 0;
}

string getName()
{
  string n;
  cout << "Enter the file name: ";
  cin >> n;
  return n;
}

float readFile(string n)
{
  float list[14] [4];
  string symbol[14];
  fstream fp;

  fp.open(n.c_str());
  if(!fp) {
    cout << "Error opening file" << endl;
    return 0;
  }

  for(int row = 0; row <= 13; row++) {
    for(int column = 0; column <= 3; column++) {
      if(column == 0) {
	      fp >> symbol[column];
	      cout << symbol[column] << endl;
	    }
      else {
	      fp >> list[row] [column];
	    }
	    if(list[row[3] < 100 && list[row][1] <= list[row] [2])
        cout << list[row] ] << " " << list[1] [2] << " " << list[1] [3] << endl;
    }
  }
}
