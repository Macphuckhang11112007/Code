#include <iostream>

using namespace std;

int main() {
    int h, m, s;
    cin >> h >> m >> s;
    h = ((24 - h - (m != 0 || s != 0)) + 24) % 24;
    m = ((60 - m - (s != 0)) + 60) % 60;
    s = ((60 - s) + 60) % 60;
    cout << h << " " << m << " " << s;
}