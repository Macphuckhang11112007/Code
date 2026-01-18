#include <iostream>
using namespace std;

bool checkPrimeNumber(int n) {
    //TODO
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return (n > 1);
}

int main() {
    int n;
    cin >> n;
    if (checkPrimeNumber(n))
        cout << n << " is a prime number.";
    else
        cout << n << " is not a prime number.";
    return 0;
}