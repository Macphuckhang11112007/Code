#include <iostream>
using namespace std;

int reverseNum(int n) {
    //TODO
    int res = 0;
    do {
        res = (res * 10) + (n % 10);
        n /= 10;
    } while (n);
    return res;
}

int main() {
    int n;
    cin >> n;
    cout << reverseNum(n);
    return 0;
}