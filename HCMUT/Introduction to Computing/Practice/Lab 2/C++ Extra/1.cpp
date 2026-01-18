#include <iostream>

using namespace std;

int main() {
    double x, ans = 0.0;
    cin >> x;
    if (x <= 0.0) {
        cout << 0.0;
    } else {
        ans += min(x, 50.0) * 1678.0;
        x -= min(x, 50.0);
        ans += min(x, 50.0) * 1734.0;
        x -= min(x, 50.0);
        ans += min(x, 100.0) * 2014.0;
        x -= min(x, 100.0);
        ans += min(x, 100.0) * 2536.0;
        x -= min(x, 100.0);
        ans += max(x, 0.0) * 2834.0;
        x -= max(x, 0.0);
    }
    cout << ans;
}