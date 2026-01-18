#include <math.h>
#include <iostream>
using namespace std;

double getDiameter(double radius) {
    //TODO
    return 2 * radius;
}

double getCircumference(double radius) {
    //TODO
    return 2 * 3.14 * radius;
}

double getArea(double radius) {
    //TODO
    return 3.14 * radius * radius;
}

int main() {
    double radius, diameter, circle, area;
    cin >> radius;
    diameter = getDiameter(radius);         // Calling getDiameter function
    circle   = getCircumference(radius);    // Calling getCircumference function
    area     = getArea(radius);             // Calling getArea function
    cout << "Diameter of the circle: " << diameter << " units" << endl;
    cout << "Circumference of the circle: " << circle << " units" << endl;
    cout << "Area of the circle:" << area << " sq. units" << endl;
    return 0;
}