#include <iostream>
#include <chrono> 
using namespace std;

/* https://projecteuler.net/problem=1
If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.
*/

int main() {
    auto start = chrono::high_resolution_clock::now();

    const int maxNumber = 1000;
    const int div1 = 3;
    const int div2 = 5;

    int numberOfMultiples = 0;
    int sumOfMultiples = 0;

    for (int num = 1; num < maxNumber; num++) {
        if (num % div1 == 0 || num % div2 == 0) {
            numberOfMultiples++;
            sumOfMultiples += num;
        }
    }
    
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); 


    cout << numberOfMultiples << endl;
    cout << sumOfMultiples << endl;
    cout << duration.count() << endl;
    return 0;
}