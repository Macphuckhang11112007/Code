#include<bits/stdc++.h>
using namespace std;
#define ll long long
struct coor {
    int x, y;
};
int manhatan(coor i, coor j) {
    return abs(i.x - j.x) + abs(i.y - j.y);
}

int fuelcost(coor start, vector<coor> route) {
    int cost = 0, cargo = 0;
    coor cur = start;
    for (auto site : route) {
        cost += cargo * manhatan(cur, site);
        cargo += 1;
        cur = site;
    }
    cost += cargo * manhatan(cur, start);
    return cost;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n; cin >> n;
    vector<coor> mine(n);
    for (int i = 0; i < n; i++)
        cin >> mine[i].x >> mine[i].y;

    coor a, b;
    cin >> a.x >> a.y >> b.x >> b.y;

    int minFuel = INT_MAX;

    for (int mask = 0; mask < (1 << n); mask++) {
        vector<coor> gA, gB;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) gA.push_back(mine[i]);
            else gB.push_back(mine[i]);
        }
        
        vector<int> idA(gA.size()), idB(gB.size());
        for (int i = 0; i < gA.size();i++){
            idA[i] = i;
        }
        for (int i = 0; i < gB.size();i++){
            idB[i] = i;
        }
        do {
            vector<coor> routeA;
            for (int i : idA) routeA.push_back(gA[i]);
            int costA = fuelcost(a, routeA);

            do {
                vector<coor> routeB;
                for (int j : idB) routeB.push_back(gB[j]);
                int costB = fuelcost(b, routeB);

                minFuel = min(minFuel, costA + costB);
            } while(next_permutation(idB.begin(), idB.end()));
        } while(next_permutation(idA.begin(), idA.end()));
    }

    cout << minFuel << "\n";
}
