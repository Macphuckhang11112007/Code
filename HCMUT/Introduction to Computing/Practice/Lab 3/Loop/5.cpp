#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    // TODO
    int n;
    cin >> n;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        double x;
        cin >> x;
        ans += (x >= 0);
    }
    cout << ans << " " << n - ans;
}