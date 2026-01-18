#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    // TODO
    double hourlyRates[5] = {9.5, 6.4, 12.5, 5.5, 10.5};
    double workingHours[5], wages[5];
    for (int i = 0; i < 5; i++) {
        cin >> workingHours[i];
        wages[i] = hourlyRates[i] * workingHours[i];
    }
    cout << left << setw(30) << "Hourly Rate" << setw(30) << "Working Hour" << setw(30) << "Wage" << endl;
    for (int i = 0; i < 5; i++) {
        cout << fixed << setprecision(10) << left << setw(30) << hourlyRates[i] << setw(30) << workingHours[i] << setw(30) << wages[i] << endl;
    }
}