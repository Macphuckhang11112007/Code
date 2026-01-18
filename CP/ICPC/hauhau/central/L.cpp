#include <bits/stdc++.h>
using namespace std;
#define ll long long
const double INF = 1e18;

int n, m;
double v;
const int MAXN = 200005;
vector<pair<int,double>> adj[MAXN];
int deg[MAXN];

double stoptime(int d) {
    if (d <= 2) return 0;
    return d - 2;
}

void dijkstra(int s, int t) {
    vector<double> dist(n + 1, INF);
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;

    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du != dist[u]) continue;
        for (auto [vtx, d] : adj[u]) {
            double wait = (vtx == t ? 0 : stoptime(deg[vtx]));
            double nd = du + d / v + wait;
            if (nd < dist[vtx]) {
                dist[vtx] = nd;
                pq.push({nd, vtx});
            }
        }
    }

    cout << fixed << setprecision(6) << dist[t] << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> v;
    for (int i = 0; i < m; i++) {
        int u, vv; double d;
        cin >> u >> vv >> d;
        adj[u].push_back({vv, d});
        adj[vv].push_back({u, d});
        deg[u]++; deg[vv]++;
    }
    int s, t;
    cin >> s >> t;
    dijkstra(s, t);
}
