#include <iostream>
using namespace std;

// most basic data types
  // int age = 25 ;
  // float weigth = 75.2 ;
  // double balance = 100.00 ;
  // char gennder = 'M';
  // string username = "Bryan" ;
  // bool isTodaySunny = false ;
  // string colors[] = {"something", "like", "this"};
  // cout << colors[0];
  // cin >> colors[0];
  // cout << colors[0];
// we have to handle memory ourselves and be mindful of a var's byte size(that's weird)
int main() {
  int counter = 1 ;
  do {
    cout << counter << endl;
    counter ++ ;
  }while(counter<=10);
}
// Instead of this you could use turnanry operators
  // if (isTodaySunny = true){
  //   cout << "Go to the park" << endl;
  // }
  // else {
  //   cout << "Stay home" << endl;
// turnary operator -->
  // isTodaySunny? cout << "Go to the park": cout << "Take an ummbrella";
// next we experimented with switch cases
// while
  // while(counter<=10){
  //   cout << counter << endl;
  //   counter ++ ;
  // }