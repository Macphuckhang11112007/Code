#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;

void prob1() {
    int score;
    cin >> score;
    if (score >= 90) {
        cout << "The student is awarded a SCHOLARSHIP";
    } else {
        cout << "The student is not awarded a SCHOLARSHIP";
    }
    cout << endl;
    return;
}

void prob2() {
    float a, b, c;
    cin >> a >> b >> c;
    if (a > 0 && b > 0 && c > 0) {
        if (a + b > c && a + c > b && b + c > a) {
            cout << "VALID TRIANGLE";
        } else {
            cout << "NOT A TRIANGLE";
        }
    } else {
        cout << "INVALID LENGTHS";
    }
    cout << endl;
    return;
}

void prob3() {
    int age, fare = 20'000;
    cin >> age;
    string card;
    getline(cin >> ws, card);
    string type;
    type = card.substr(6);
    if (type == "DISABLED" || (type == "CHILD" && age < 7) || (type == "ELDERLY" && age >= 60)) {
        fare -= 0.75 * fare;
    }
    cout << "Fare: " << fare << " VND" << endl;
    return;
}

int main() {
    cout << "Prob 2.1:" << endl;
    prob1();
    cout << endl;
    cout << "Prob 2.2:" << endl;
    prob2();
    cout << endl;
    cout << "Prob 2.3:" << endl;
    prob3();
    cout << endl;
    return 0;
}