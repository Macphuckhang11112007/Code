#include <cctype>
#include <iostream>
#include <string>
using namespace std;

string a[] = {"ai", "ae", "ao", "au", "ei", "eu", "iu", "oi", "ou", "ui", "p", "k", "h", "l", "m", "n", "w", "a", "e", "i", "o", "u"};

bool isVocab(string c) {
    bool ok = 1;
    for (char ch : c) {
        ok &= ('a' <= ch && ch <= 'z');
    }
    return ok;
}

bool isConsonant(string c) {
    for (int id = 10; id <= 16; id++) {
        string ch = a[id];
        if (c == ch) {
            return 1;
        }
    }
    return 0;
}

bool isVowel(string c) {
    for (int id = 17; id <= 21; id++) {
        string ch = a[id];
        if (c == ch) {
            return 1;
        }
    }
    return 0;
}

bool isVowelGroup(string c) {
    for (int id = 0; id <= 9; id++) {
        string ch = a[id];
        if (c == ch) {
            return 1;
        }
    }
    return 0;
}

int main() {
    string s;
    getline(cin, s);

    // TODO
    int ans = 0;
    for (size_t i = 0; i < s.size(); i++) {
        bool   ok = 0;
        string c{s[i] = tolower(s[i])};
        for (string &ch : a) {
            ok |= (c == ch || c == " " || c == "\'");
        }
        if (ok == 0) {
            ans = -1;
            break;
        }
    }
    if (ans != -1) {
        for (size_t i = 0; i < s.size(); i++) {
            string c{s[i]};
            if (isVocab(c)) {
                ans++;
                if (isConsonant(c)) {
                    if (isVowelGroup(s.substr(i + 1, 2))) {
                        i += 2;
                    } else if (isVowel(s.substr(i + 1, 1))) {
                        i++;
                    }
                } else if (isVowelGroup(s.substr(i, 2))) {
                    i++;
                }
            }
        }
    }
    cout << ans;
    return 0;
}