#include<bits/stdc++.h>
using namespace std;
#define ll long long
vector<string> num = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven"
,"twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty"};

bool isDigitt(string s){
    for (char c : s){
        if(!isdigit(c)) return false;
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s; 
    cin >> s;
    if(isDigitt(s)){
        int x = stoi(s);
        cout << num[x];
    }
    else {
        for (int i = 1; i <= 20; i++){
            int err = 0;
            string so = num[i];
            if(s.size() == so.size()){
                for (int i = 0; i < s.size(); i++){
                    if(s[i] != so[i]){
                        err++;
                    }
                }
                if(err <=1){
                    cout << i;
                    return 0;
                }
            }
        }
    }
    return 0;
}
