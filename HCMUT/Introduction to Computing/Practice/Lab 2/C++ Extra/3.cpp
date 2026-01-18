#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    map<char, string> a = {{'0', "không"}, {'1', "một"}, {'2', "hai"}, {'3', "ba"}, {'4', "bốn"}, {'5', "năm"}, {'6', "sáu"}, {'7', "bảy"}, {'8', "tám"}, {'9', "chín"}};
    int               x;
    cin >> x;
    string s = to_string(x);
    if (x <= 9) {
        cout << a[s[0]];
    } else if (x <= 99) {
        if (s[0] == '1') {
            cout << "mười";
        } else {
            cout << a[s[0]] << " mươi";
        }
        if (s[1] != '0') {
            if (s[1] == '5') {
                cout << " lăm";
            } else if (s[0] != '1' && s[1] == '1') {
                cout << " mốt";
            } else {
                cout << " " << a[s[1]];
            }
        }
    } else if (x <= 999) {
        cout << a[s[0]] << " trăm";
        if (s[1] == '0') {
            cout << " lẻ";
        } else {
            if (s[1] == '1') {
                cout << " mười";
            } else {
                cout << " " << a[s[1]] << " mươi";
            }
        }
        if (s[2] != '0') {
            if (s[2] == '5') {
                cout << " lăm";
            } else if (s[1] != '0' && s[1] != '1' && s[2] == '1') {
                cout << " mốt";
            } else {
                cout << " " << a[s[2]];
            }
        }
    }
}