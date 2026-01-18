#include <iomanip>
#include <iostream>

using namespace std;

void prob1() {
    int n;
    // cin >> n;
    n       = 10;
    int num = 1, i = 1;
    while (num <= n) {
        for (int j = 1; j <= i; j++) {
            cout << num << " ";
            num++;
        }
        cout << endl;
        i++;
    }
    return;
}

void prob2() {
    string name, age;
    cout << "Your name is: ";
    getline(cin >> ws, name);
    cout << "Your age is: ";
    getline(cin >> ws, age);
    for (int i = 1; i <= 30; i++) {
        cout << "*";
    }
    cout << endl;
    cout << "* Name: " << name << setw(30 - 8 - name.size()) << "*" << endl;
    cout << "* Age: " << age << setw(30 - 7 - age.size()) << "*" << endl;
    for (int i = 1; i <= 30; i++) {
        cout << "*";
    }
    cout << endl;
    return;
}

void prob3() {
    double num;
    cout << "Input your number: ";
    cin >> num;
    cout << fixed << setprecision(1);
    const int max_rows = 4;
    for (int i = 1; i <= max_rows; ++i) {
        int leading_spaces = 2 * (max_rows - i);
        for (int j = 0; j < leading_spaces; ++j) {
            cout << " ";
        }
        for (int k = 1; k <= i; ++k) {
            cout << num;
            if (k < i) {
                cout << " ";
            }
        }
        cout << endl;
    }
    return;
}

int main() {
    cout << "Prob 2.1: " << endl;
    prob1();
    cout << endl;
    cout << "Prob 2.2: " << endl;
    prob2();
    cout << endl;
    cout << "Prob 2.3: " << endl;
    prob3();
    cout << endl;
    return 0;
}