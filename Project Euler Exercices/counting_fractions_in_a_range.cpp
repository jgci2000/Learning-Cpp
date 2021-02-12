#include <iostream>
using namespace std;

/* https://projecteuler.net/problem=73
Consider the fraction, n/d, where n and d are positive integers. If n<d and HCF(n,d)=1, it is called a reduced proper fraction.

If we list the set of reduced proper fractions for d ≤ 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that there are 3 fractions between 1/3 and 1/2.

How many fractions lie between 1/3 and 1/2 in the sorted set of reduced proper fractions for d ≤ 12,000?
*/

int greatestCommonDivisor(int n1, int n2);

int main() {
    const int MAX_D = 12000;

    int fractionCounter = 0;

    for (int d = 1; d <= MAX_D; d++) {
        for (int n = 1; n < d; n++) {
        // n is always smaller than d
        if (greatestCommonDivisor(n, d) != 1)
            continue;
        double frac = (double)n / (double)d;
        if (frac > 1.0/3.0 && frac < 1.0/2.0)
            fractionCounter++;
        }
    }

    cout << fractionCounter << endl;
    return 0;
}

int greatestCommonDivisor(int n1, int n2) {
    // This function uses the Euclidean Algorithm
    if (n2 == 0)
        return n1;
    else
        return greatestCommonDivisor(n2, n1 % n2);
}
