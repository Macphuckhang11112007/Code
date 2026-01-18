#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    // TODO
    int n;
    cin >> n;
    double ans = -1e18;
    for (int i = 0; i < n; i++) {
        double x;
        cin >> x;
        ans = max(ans, x);
    }
    cout << fixed << setprecision(2) << ans;
}