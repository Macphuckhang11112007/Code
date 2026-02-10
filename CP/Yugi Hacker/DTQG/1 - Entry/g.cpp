#include <bits/stdc++.h>

using namespace std;

const int N = 2e5, MAX = 31622, LEN = 1e6;

int         n, minn = 1e9, maxx, a[N + 5], spf[MAX + 5], d[LEN + 5], val[LEN + 5];
vector<int> primes;
int         arr[N + 5];
deque<int>  dq;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        maxx = max(maxx, a[i]);
        minn = min(minn, a[i]);
    }
    for (int i = 0; i <= maxx - minn; i++) {
        val[i] = minn + i;
    }
    for (int i = 2; i * i <= maxx; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int j : primes) {
            if (j > spf[i] || j > MAX / i) {
                break;
            }
            spf[i * j] = j;
        }
    }
    for (int i : primes) {
        for (int j = max((minn + i - 1) / i * i, i * i); j <= maxx; j += i) {
            int cnt = 0;
            while (!(val[j - minn] % i)) {
                cnt++;
                val[j - minn] /= i;
            }
            if (!d[j - minn]) {
                d[j - minn] = 1;
            }
            d[j - minn] *= ++cnt;
        }
    }
    for (int i = 0; i <= maxx - minn; i++) {
        if (val[i] > 1) {
            d[i] *= 2;
        }
    }
    for (int i = n; i; i--) {
        while (dq.size() && d[a[i] - minn] >= d[a[dq.front()] - minn]) {
            dq.pop_front();
        }
        if (dq.size()) {
            arr[i] = a[dq.front()];
        } else {
            arr[i] = -1;
        }
        dq.push_front(i);
    }
    for (int i = 1; i <= n; i++) {
        cout << arr[i] << " ";
    }
}
