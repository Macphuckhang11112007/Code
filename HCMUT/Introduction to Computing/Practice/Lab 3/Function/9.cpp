#include <iostream>

using namespace std;

void swap(int &x, int &y)

{
    //TODO
    x ^= y;
    y ^= x;
    x ^= y;
}

int main()

{
    int x, y;

    cin >> x >> y;

    swap(x, y);

    cout << x << " " << y;

    return 0;
}