#include <iostream>
using namespace std;

int convertBinaryToDecimal(string n) {
    //TODO
    int res = 0, x = 1;
    for (int i = int(n.size()) - 1; i >= 0; i--) {
        res += (n[i] == '1' ? x : 0);
        x *= 2;
    }
    return res;
}

int main() {
    string n;
    cin >> n;
    cout << n << " in binary = " << convertBinaryToDecimal(n) << " in decimal";
    return 0;
}