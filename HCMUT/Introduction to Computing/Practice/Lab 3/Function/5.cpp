#include <iostream>
using namespace std;

int getMaxElement(int arr[], int n) {
    //TODO
    int maxx = -1e9;
    for (int i = 0; i < n; i++) {
        maxx = max(maxx, arr[i]);
    }
    return maxx;
}

int main() {
    int n;
    cin >> n;
    int ar[n];
    for (int i = 0; i < n; i++)
        cin >> ar[i];
    cout << getMaxElement(ar, n);
    return 0;
}