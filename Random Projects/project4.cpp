/*********************************************************/
/* Author:             Matthew Lingenfelter              */
/* Creation Date:      April 13, 2020                    */
/* Purpose :           This program will read the record */
/*                     of sales and output the net profit*/
/*********************************************************/

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

string getName(string);
int getItems(int);

int main()
{
  //declaring all variables that'll be used
  string name;
  int items;
  float price, gross, overhead, tax, net, cost, c, selling;
  int temp = 0;

  //gets the first item name from the user
  cout << "Enter name of the item: ";
  cin >> name;

  //will loop the program until the user enders 'fine' as the item name
  while(name != "fine") {
    //gets the amount of items sold from the user
    cout << "Enter the number of items sold: ";
    cin >> items;
    
    //sets (then resets) all values to 0 in order to
    //be correctly calculated
    price = 0;
    gross = 0;
    overhead = 0;
    tax = 0;
    net = 0;
    cost = 0;

    //determines how many times to ask the user the selling
    //price and cost based off how many items were sold
    for(temp = 1; temp <= items; temp++) {
      //gets the items selling price from the user
      cout << "Enter the selling price: ";
      cin >> selling;
      //gets the cost for that item from the user
      cout << "Enter the cost: ";
      cin >> c;
      //adds the price entered to the excisting amount
      price = price + selling;
      //adds the cost entered to the excisting amount 
      cost = cost + c;
    }
  
    //calculates the gross profit by subtracting the cost
    //from the selling price
    gross = price - cost;
    //calculates tax by multiplying the gross profit by 6%
    tax = gross * 0.06;
    //calculates the overhead by multiplying the gross
    //profit by 10%
    overhead = gross * 0.1;
    //calculates the net profit by subtracting the tax and
    //overhead from the gross profit
    net = gross - tax - overhead;

    //outputs the name of the item
    cout << endl << "Name:         " << name;
    //outputs the gross profit
    cout << endl << "Gross profit: $" << gross;
    //outputs the tax from the gross profit
    cout << endl << "Tax :         $" << setprecision(2) << fixed << tax;
    //outputs the net profit
    cout << endl << "Net profit:   $" << net;
    cout << endl << endl;
   
    //gets the name of the next item or 'fine' to terminate the loop
    cout << "Enter name of the item: ";
    cin >> name;
  }

  //outputs 'Bye.' and ends the program
  cout << "Bye." << endl;
  return 0;
}

string getName()
{
  string n;
  //gets the item name from the user
  cout << "Enter name of the item: ";
  cin >> n;
  return n;
}

int getItems()
{
  int i;
  cout << "Enter the number of items sold: ";
  cin >> i;
  return i;
}
