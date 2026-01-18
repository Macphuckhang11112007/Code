#include <iostream>
using namespace std;

void printPrimes(int lower, int upper) {
    // TODO
    bool is_prime[upper + 1] = {0, 0, 1};
    for (int i = 3; i <= upper; i += 2) {
        is_prime[i] = 1;
    }
    for (int i = 3; i * i <= upper; i += 2) {
        if (is_prime[i]) {
            int start = (lower + i - 1) / i;
            start += (~start & 1);
            start = max(3, start);
            start *= i;
            for (int j = start; j <= upper; j += 2 * i) {
                is_prime[j] = 0;
            }
        }
    }
    if (lower <= 2 && upper >= 2) {
        cout << "2 ";
    }
    for (int i = max(3, lower + (~lower & 1)); i <= upper; i += 2) {
        if (is_prime[i]) {
            cout << i << " ";
        }
    }
}

int main() {
    int lower, upper;
    cin >> lower;
    cin >> upper;
    // Calling function to print all primes between the given range.
    printPrimes(lower, upper);
    return 0;
}