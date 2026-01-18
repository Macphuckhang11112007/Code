#include <iostream>

using namespace std;

int computeStamina(string moves, int hp, double breathingMastery) {
    // TODO
    double res = hp;
    for (char c : moves) {
        res -= (int(c) - 'A' + 1) - breathingMastery;
    }
    return int(max(0.0, res));
}

int main() {
    cout << computeStamina("ABCAAC", 100, 0.5) << endl;
    cout << computeStamina("ABCABCABC", 99, 0.5) << endl;
    cout << computeStamina("B", 1000, 0.0) << endl;
}