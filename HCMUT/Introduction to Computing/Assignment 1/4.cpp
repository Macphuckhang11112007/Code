#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    float mid, lab, final;
    cin >> mid >> lab >> final;
    mid   = 7.0f;
    lab   = 10.0f;
    final = 9.0f;
    float wMid, wLab, wFinal;
    cin >> wMid >> wLab >> wFinal;
    wMid      = 0.30f;
    wLab      = 0.20f;
    wFinal    = 0.50f;
    float avg = mid * wMid + lab * wLab + final * wFinal;                    // avg = 7.0 * 0.30 + 10.0 * 0.20 + 9.0 * 0.50 = 8.6
    cout << fixed << setprecision(2) << "Average score: " << avg << endl;    // Average score: 8.60
    return 0;
}