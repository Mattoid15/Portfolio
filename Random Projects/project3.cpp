/************************************************************/
/* Author:         Matthew Lingenfelter                     */
/* Creation Date:  April 6, 2020                            */
/* Purpose:        This program will print out the duration */
/*             of a projectile's flight and it's height     */
/*             above the ground when it reaches the target. */
/************************************************************/

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

void instructions();
float getTheta();

int main()
{
  //delcareing all variables that'll be used
  float theta, distance, velocity;
  float time, height;
  float g = 32.17;
  
  //prints the welcome instructions for the program
  void instructions();
  theta = getTheta();

  //prompts the user to enter the angle of elevation
  cout << endl << "Enter the angle of elevation in radians: ";
  //saves that number as theta
  cin >> theta;
  //prompts the user to enter the distance to the target
  cout << "Enter the distance to target in feet: ";
  //saves that number as distance
  cin >> distance;
  //prompts the user to enter the projectile's velocity
  cout << "Enter the projectile's velocity in feet per second: ";
  //saves that number as velocity
  cin >> velocity;
  
  //calculates the time of the flight
  time = distance / (velocity * cos(theta));
  //calculates the height the projectile is at when it hits the target
  height = (velocity * sin(theta) * time) - (0.5 * g * (time * time));

  //reprints the angle of elevation the user input
  cout << endl << "Angle of elevation: " << setw(8) << " " << theta << " radian"<< endl;
  //reprints the distance the user input
  cout << "Distance to target: " << setw(8) << " " << distance << " ft" << endl;
  //reprints the projectile's velocity the user input
  cout << "Projectile's velocity: " << setw(5) << " " << velocity << " ft/second" << endl;
  //prints the calculated time of the flight
  cout << "Duration of flight: " << setprecision(2) << fixed << setw(13) << time << " seconds" << endl;
  //prints the calculated height at the target
  cout << "Height at impact: " << setw(10) << " " << height << " ft" << endl;

  return 0;
}

void instructions()
{
  cout << "Welcome to the projectile calculator." << endl;
  cout << "=====================================" << endl;
  cout << "This program computes the duration of a projectile's flight" << endl;
  cout << "and its height above the ground when it reaches the target." << endl;
}

float getTheta()
{
  float theta;
  cout << endl << "Enter the angle of elevation in radians: ";
  cin >> theta;
  return theta;
}
