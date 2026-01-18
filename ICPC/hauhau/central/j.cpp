#include<bits/stdc++.h>
using namespace std;
#define ll long long
vector<string> a;
int r, c;
int dx[] ={-1, 1, 0 , 0};
int dy[] = {0, 0, 1, -1};
int dist[105][105][4];
struct state{
    int x, y, hp;
};

bool isValid(int x, int y){
    if(x >= 0 && x < r && y >= 0 && y < c) return true;
    else return false;
}

void bfs(state start){
    memset(dist, -1, sizeof(dist));
    queue<state> q;
    q.push({start.x, start.y, 3});
    dist[start.x][start.y][3] = 0;
    while(q.size()){
        auto cur = q.front(); q.pop();
        for (int i = 0; i < 4; i++){
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];
            if(!isValid(nx, ny)) continue;
            int nhp = cur.hp;
            if(a[nx][ny] == '+') nhp--;
            if(nhp <= 0) continue;
            if(dist[nx][ny][nhp] == -1){
                dist[nx][ny][nhp] = dist[cur.x][cur.y][cur.hp] + 1;
                q.push({nx , ny, nhp});
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> r >> c;
    for (int i = 1; i <= r; i++){
        string s; cin >> s;
        a.emplace_back(s);
    }
    state start;
    state end;
    for (int i = 0; i < r; i++){
        for (int j = 0; j < c; j++){
            if(a[i][j] == 'S'){
                start = {i, j, 3};
            }
            if(a[i][j] == 'D'){
                end ={i ,j, 3};
            }
        }
    }
    bfs(start);
    int ans = INT_MAX;
    for (int h = 1; h <= 3; h++){
        if(dist[end.x][end.y][h] != -1)
            ans = min(ans, dist[end.x][end.y][h]);
    }
    cout << (ans == INT_MAX ? -1 : ans);

    return 0;
}