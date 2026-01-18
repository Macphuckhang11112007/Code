#include <string.h>
#include <iomanip>
#include <iostream>

using namespace std;

const int MAX = 100;

struct student {
    char     name[20];
    long int rollno;
    char     sex;
    float    height;
    float    weight;
};

int main() {
    student cls[MAX];
    int     i, n;
    cout << "How many names ? \n";
    cin >> n;

    for (i = 0; i <= n - 1; ++i) {
        cout << "record = " << i + 1 << endl;
        cout << "name : ";
        cin >> cls[i].name;
        cout << "rollno : ";
        cin >> cls[i].rollno;
        cout << "sex : ";
        cin >> cls[i].sex;
        cout << "height : ";
        cin >> cls[i].height;
        cout << "weight : ";
        cin >> cls[i].weight;
        cout << endl;
    }
    // TODO
    float avg_height = 0, avg_weight = 0;
    cout << left << setw(20) << "Name" << setw(20) << "Rollno" << setw(20) << "Sex" << setw(20) << "Height" << setw(20) << "Weight" << endl;
    for (i = 0; i <= n - 1; ++i) {
        avg_height += cls[i].height;
        avg_weight += cls[i].weight;
        cout << fixed << setprecision(2) << left << setw(20) << cls[i].name << setw(20) << cls[i].rollno << setw(20) << cls[i].sex << setw(20) << cls[i].height << setw(20) << cls[i].weight << endl;
    }
    cout << fixed << setprecision(5) << avg_height / n << " " << avg_weight / n;
}