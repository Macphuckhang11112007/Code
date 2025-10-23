#include <math.h>
#include <iostream>

using namespace std;

void prob1() {
    double a, b, c;
    cout << "Enter three side lengths: ";
    cin >> a >> b >> c;
    if (a <= 0 || b <= 0 || c <= 0) {
        cout << "Error: Side lengths must be positive" << endl;
    } else if (a + b <= c || a + c <= b || b + c <= a) {
        cout << "Not a valid triangle" << endl;
    } else {
        if (a == b && b == c) {
            cout << "Triangle is Equilateral" << endl;
        } else if (a == b || b == c || a == c) {
            cout << "Triangle is Isosceles" << endl;
        } else {
            cout << "Triangle is Scalene" << endl;
        }
    }
    return;
}

void prob2() {
    int  age;
    char per;
    cout << "Please input your age: ";
    cin >> age;
    cout << "Do you have parent permission (Y/N): ";
    cin >> per;
    if (age >= 18) {
        cout << "You can join the event.";
    } else if (per == 'Y') {
        cout << "You can join with permission.";
    } else {
        cout << "You cannot join the event.";
    }
    cout << endl;
    return;
}

void prob3() {
    const double epsilon = 1e-6;
    const double s_quad  = 360.0;
    const double ang_90  = 90.0;
    double       a, b, c, d;
    cout << "Enter angle A (in degrees): ";
    cin >> a;
    cout << "Enter angle B (in degrees): ";
    cin >> b;
    cout << "Enter angle C (in degrees): ";
    cin >> c;
    d             = s_quad - (a + b + c);
    auto is_equal = [=](double v1, double v2) {
        return fabs(v1 - v2) < epsilon;
    };
    auto is_suppl = [=](double v1, double v2) {
        return is_equal(v1 + v2, 180.0);
    };
    if (a <= 0 || b <= 0 || c <= 0 || d <= 0) {
        cout << "Invalid quadrilateral." << endl;
        return;
    }
    bool is_r = is_equal(a, ang_90) && is_equal(b, ang_90) && is_equal(c, ang_90) && is_equal(d, ang_90);
    bool is_p = (is_equal(a, c) && is_equal(b, d)) || (is_equal(a, b) && is_equal(c, d)) || (is_equal(a, d) && is_equal(b, c));
    bool is_c = (is_suppl(a, c) && is_suppl(b, d)) || (is_suppl(a, b) && is_suppl(c, d)) || (is_suppl(a, d) && is_suppl(b, c));
    bool is_t = is_suppl(a, b) || is_suppl(b, c) || is_suppl(c, d) || is_suppl(d, a) || is_suppl(a, c) || is_suppl(b, d);
    if (is_r) {
        cout << "Rectangle" << endl;
    } else if (is_p) {
        cout << "Parallelogram" << endl;
    } else if (is_t) {
        if ((is_suppl(a, b) && is_equal(a, ang_90)) || (is_suppl(b, c) && is_equal(b, ang_90)) || (is_suppl(c, d) && is_equal(c, ang_90)) || (is_suppl(d, a) && is_equal(d, ang_90)) || (is_suppl(a, c) && is_equal(a, ang_90)) || (is_suppl(b, d) && is_equal(b, ang_90))) {
            cout << "Right trapezoid" << endl;
        }
        if ((is_suppl(a, b) && is_equal(a, c)) || (is_suppl(b, c) && is_equal(b, a)) || (is_suppl(c, d) && is_equal(c, a)) || (is_suppl(d, a) && is_equal(d, b)) || (is_suppl(a, c) && is_equal(a, b)) || (is_suppl(b, d) && is_equal(b, a)) || (is_suppl(a, b) && is_equal(a, d)) || (is_suppl(b, c) && is_equal(b, d)) || (is_suppl(c, d) && is_equal(c, b)) || (is_suppl(d, a) && is_equal(d, c)) || (is_suppl(a, c) && is_equal(a, d)) || (is_suppl(b, d) && is_equal(b, c))) {
            cout << "Isosceles trapezoid" << endl;
        }
        cout << "Normal trapezoid" << endl;
    }
    if (is_c) {
        cout << "Cyclic quadrilateral" << endl;
        return;
    }
    cout << "Irregular quadrilateral";
    return;
}

void prob4() {
    double a, b, c;
    cout << "Enter coefficients a, b, and c: ";
    cin >> a >> b >> c;
    if (a == 0) {
        cout << "Not a quadratic equation." << endl;
        if (b == 0) {
            if (c == 0) {
                cout << "Infinite solutions." << endl;
            } else {
                cout << "No solution." << endl;
            }
        } else {
            double x = -c / b;
            cout << "The solution is: " << x << endl;
        }
    } else {
        double delta = b * b - 4 * a * c;
        if (delta == 0) {
            double x = -b / (2 * a);
            cout << "The solution is: " << x << endl;
        } else if (delta > 0) {
            double x1 = (-b - sqrt(delta)) / (2 * a);
            cout << "The first solution is: " << x1 << endl;
            double x2 = (-b + sqrt(delta)) / (2 * a);
            cout << "The second solution is: " << x2 << endl;
        } else {
            cout << "No real solution." << endl;
        }
    }
    return;
}

void prob5() {
    int t, s;
    cin >> t >> s;
    const int B    = 6 * 10 + 6;
    int       ex   = (s == 6 ? 12 : 16);
    int       maxx = B + ex;
    if (t <= maxx) {
        cout << "The workload is doable for Dr. Tho ";
    } else {
        cout << "The workload is NOT doable for Dr. Tho ";
    }
    cout << "and he ";
    if (t > B) {
        cout << "WILL need to work extra hours";
    } else {
        cout << "will NOT need to work extra hours";
    }
    cout << endl;
    return;
}

int main() {
    cout << "Prob 4.1:" << endl;
    prob1();
    cout << endl;
    cout << "Prob 4.2:" << endl;
    prob2();
    cout << endl;
    cout << "Prob 4.3:" << endl;
    prob3();
    cout << endl;
    cout << "Prob 4.4:" << endl;
    prob4();
    cout << endl;
    cout << "Prob 4.5:" << endl;
    prob5();
    cout << endl;
    return 0;
}