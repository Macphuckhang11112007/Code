#include <iostream>
using namespace std;

void EvenOrOdd() {
    // TODO
    int n;
    cin >> n;
    cout << (n & 1 ? "Odd\n" : "Even\n");
}

int main() {
    EvenOrOdd();
    return 0;
}