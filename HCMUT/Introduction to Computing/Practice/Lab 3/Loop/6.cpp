#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    // TODO
    int n;
    cin >> n;
    unsigned long long a = 0, b = 1, c = n - 1;
    for (int i = 2; i < n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    cout << c;
    return 0;
}