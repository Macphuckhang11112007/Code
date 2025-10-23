#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;

void prob1() {
    char  grade = 'B';
    int   p = 8, q = 15, r = 20, s = 25;
    float t = 19.5;
    cout << (p > q);
    cout << (p * 2 <= r);
    cout << ((grade + 1) != 'C');
    cout << ('A' + 1 == grade);
    cout << (t + p / 2.0 <= 24.0);
    cout << ((r - q) * 2 == (s - p));
    // 010110
    cout << endl;
    return;
}

void prob2() {
    int   wind        = 130;     // km/h
    int   pressure    = 985;     // hPa
    int   rain        = 220;     // mm
    float river       = 4.6;     // meters
    bool  tide        = true;    // tidal surge warning
    bool  strongStorm = (wind >= 120 && pressure <= 980);
    cout << strongStorm << endl;
    bool floodRisk = (rain > 200 || (river > 4.5 && tide));    // (i)  parentheses needed around the river && tide part for not going simply from left to right, it is two separated conditions, not three simple ones
                                                               // (ii) if not, c++ will evaluate rain > 200 or river > 4.5 and tide, which do not have the original meaning
    cout << floodRisk << endl;

    // rain = 180, river = 4.8, tide = true
    // bool floodRisk = (rain > 200 || river > 4.5 && tide); // rain > 200 = 180 > 200 -> 0, river > 4.5 = 4.8 > 4.5 -> 1, tide = 1 -> 1  --> floodRisk = 0 || 1 && 1 = 1 && 1 = 1
    bool safe = (!strongStorm && !floodRisk);
    cout << safe << endl;
    return;
}

void prob3() {
    int a = 4, b = 6, c = 8;
    cout << (!(++a > b) && (c-- >= b)) << endl;    // !(++a <=> a = a + 1 = 4 + 1 = 5 > 6) && (c >= b) = !(5 > 6) && (8 >= 6) = !0 && 1 = 1 && 1 = 1, then c-- <=> c = c - 1 = 8 - 1 = 7
    int x = 3, y = 5, z = 7;
    cout << (x + y * 2 < z * 2 - 3 && y++ <= z) << endl;
    // Evaluation order for (x + y * 2 < z * 2 - 3 && y++ <= z):
    // 1. y * 2 = 10
    // 2. x + (y * 2) = 3 + 10 = 13
    // 3. z * 2 = 14
    // 4. z * 2 - 3 = 11
    // 5. Compare: 13 < 11 → false (0)
    // 6. Left side of && is false → short-circuit, so (y++ <= z) is NOT evaluated
    // 7. y remains 5 (no increment happens)
    // Final result: expression = 0, so cout prints "0"
    return;
}

int main() {
    cout << "Prob 1.1:" << endl;
    prob1();
    cout << endl;
    cout << "Prob 1.2:" << endl;
    prob2();
    cout << endl;
    cout << "Prob 1.3:" << endl;
    prob3();
    cout << endl;
    return 0;
}