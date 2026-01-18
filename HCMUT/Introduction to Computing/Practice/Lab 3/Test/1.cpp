#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    // TODO
    int n;
    cin >> n;
    unsigned long long fib[n] = {0, 1};
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    for (int i = 0; i < n; i++) {
        cout << fib[i] << (i == n - 1 ? "" : " ");
    }
    return 0;
}