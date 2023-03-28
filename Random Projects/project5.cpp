/*******************************************************/
/* Author:            Matthew Lingenfelter             */
/* Creation Date:     April 22, 2020                   */
/* Purpose:           This program will read all values*/
/*                    from a data file and outputs the */
/*                    net profit of sales.             */
/*******************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

int getItems(ifstream &fp);

int main()
{
  ifstream fp;
  string name, fileName;
  float price, gross, overhead, tax, net, cost, tempCost, selling;

  cout << "Enter name of the file: ";
  cin >> fileName;

  if(fileName == "sales.dat") {
    fp.open("sales.dat");
  }
  else if(fileName == "sales2.dat") {
    fp.open("sales2.dat");
  }
  else if(fileName != "sales.dat") {
    cout << "Error opening file\n";
    return 0;
  }


  
  cout << name;
  int items = getItems(fp);
  for(int temp = 0; temp < items; temp++) {
	  fp >> price;
	  cout << endl << price;
	}
  
  fp.close();
  return 0;
}

int getItems(ifstream &fp)
{
  int num = 0;
  fp >> num;
  return num;
}
