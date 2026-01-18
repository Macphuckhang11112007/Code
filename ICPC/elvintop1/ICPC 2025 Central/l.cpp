#include <bits/stdc++.h>
using namespace std;

const int N = 2e6 + 5;

int n, m, s, t;
long double spd;                       
vector<pair<int,int>> g[N];
long double distT[N];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m >> spd;
    for (int i = 0; i < m; ++i) {
        int u, v, w; 
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    cin >> s >> t;

    for (int i = 1; i <= n; ++i)
        distT[i] = numeric_limits<long double>::infinity();

    using State = pair<long double,int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    distT[s] = 0.0L;
    pq.push({0.0L, s});

    auto wait_time = [&](int u) -> long double {
        if (u == s || u == t) return 0.0L;                 
        long long deg = (long long)g[u].size();
        return (long double)max(0LL, deg - 2LL);           
    };

    while (!pq.empty()) {
        auto [du, u] = pq.top(); 
        pq.pop();                          
        if (du != distT[u]) continue;      
        if (u == t) break;                

        long double wwait = wait_time(u);
        for (auto [to, w] : g[u]) {
            long double nd = du + wwait + ((long double)w) / spd;
            if (nd + 1e-15L < distT[to]) {
                distT[to] = nd;
                pq.push({nd, to});
            }
        }
    }

    cout << fixed << setprecision(6) << distT[t] << '\n';
    return 0;
}
