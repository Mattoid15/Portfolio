/*****************************************************/
/* Author: Matthew Lingenfelter                      */
/* Creation Date: February 3, 2020                   */
/* Purpose: This program calculates the user's pay   */
/*         given number of hours and hourly rate     */
/*****************************************************/

#inclue <iostream>
uning namespace std;

int main()
{
  double hours, rate, pay;
  
  // Get the nunber of hours worked.
  cout << "How many hours did you work? ";
  cin >> hours;
  
  // Get the hourly pay rate.
  cout << "How much do you get paid per hour? ";
  cin >> rate;
  
  // Calculate the pay.
  pay = hours * rate;
  
  // Display the pay.
  cout << "You have earned $" << pay << endl;
  return 0;
}
