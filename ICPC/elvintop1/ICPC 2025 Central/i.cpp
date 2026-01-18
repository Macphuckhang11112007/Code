#include <bits/stdc++.h>
using namespace std;

double hyp (double a, double b) {
	return sqrt(a*a + b*b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int tc;
    cin >> tc;
    cout << fixed << setprecision(12);

    while (tc--) {
        double x, y, z, px, py;
        cin >> x >> y >> z >> px >> py;


        double dcorner = min(min(hyp(px, py), hyp(x - px, py)), min(hyp(px, y - py), hyp(x - px, y - py)) );

        double dx = min(px, x - px);
        double dy = min(py, y - py);

        double ans1 = 2.0 * sqrt((x + y) * (x + y) + z * z) + min(2.0 * dcorner, dx + dy);

        double ans2 = hyp(x, y) + hyp(x, z) + hyp(y, z) + 2.0 * dcorner;

        double ans = min(ans1, ans2);
        cout << ans << "\n";
    }
    return 0;
}
