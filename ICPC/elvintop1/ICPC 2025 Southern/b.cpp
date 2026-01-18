#include <bits/stdc++.h>

using namespace std;

int n, m, qr;
int id_cnt;
int vst[20000001];
map<pair<int,int>, int> dr,s;
vector<int> a[1000001] ;
int cnt = 0;
map<int, vector<int>> mp, mpr;
pair<int,int> qv[1000001];
int q[1000001];

int par[1000001], par_cnt[10000001];

void DFS(int u) {
	for (int v : a[u]) {
		if (!vst[v]) {
			vst[v] = vst[u] + 1;
			DFS(v);
		}
	}
	for (int v : mp[u]) {
		if (vst[v] && vst[u] < vst[v] && dr[{v, u}]) {
			s[{u, v}] = 1;
		}
	}
	for (int v : mpr[u]) {
		if (vst[v] && vst[v] < vst[u]) {
			dr[{u, v}] = 1;
		}
	}
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin >> n >> m;
	cnt = n;
	int id = 0;
	for (int u = 1; u <= n; u++) {
		par[u] = u;
		par_cnt[u] = 0;
	}
	while (m--) {
		int u, v;
		cin >> u >> v;
		a[u].push_back(v);
	}
	cin >> qr;
	for (int i = 1; i <= qr; i++) {
		int opt;
		cin >> opt;
		if (opt == 1) {
			int x, d;
			cin >> x >> d;
			if (d == 0) {
				par[++cnt] = par[x];
				par_cnt[cnt] = par_cnt[x] + 1;
			} else {
				par[++cnt] = cnt;
				par_cnt[cnt] = 0;
			}
		} else {
			id++;
			int u, v;
			cin >> u >> v;
			qv[id] = {u, v};
			if (u > n || v > n) {
				mp[par[u]].push_back(par[v]);
				mpr[par[v]].push_back(par[u]);
				dr[{par[v], par[u]}];
			} else {
				mp[u].push_back(v);
				mpr[v].push_back(u);
				dr[{v, u}];
				q[id] = s[{u, v}];
			}
		}
	}
	for (int u = 1; u <= n; u++) {
		if (!vst[u]) {
			vst[u] = ++id_cnt;
			DFS(u);
		}
	}
	cout << id << '\n';
	for (int i = 1; i <= id; i++) {
		auto [u, v] = qv[i];
		if (u > n || v > n) {
			if ((par[u] == par[v] && par_cnt[u] < par_cnt[v]) || (par[u] != par[v] && s[{par[u], par[v]}])) {
				q[i] = 1;
			} else {
				q[i] = 0;
			}
		} else {
			q[i] = s[{u, v}];
		}
		if (q[i]) {
			cout << "YES\n";
		} else {
			cout << "NO\n";
		}
	}
}