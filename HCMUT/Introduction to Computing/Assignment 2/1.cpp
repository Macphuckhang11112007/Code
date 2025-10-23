#include <iostream>

using namespace std;

void print(int &a, int &b, int &c, int &d) {
    cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << endl;
}

void prob1() {
    int a, b, c, d;
    // cin >> a >> b >> c >> d;
    a = 2;
    b = 0;
    c = 2;
    d = 5;
    a += 4;                                           // a = a + 4 = 2 + 4 = 6
    b = b - a;                                        // b = 0 - 6 = -6
    c += a + b;                                       // c = c + a + b = 2 + 6 + (-6) = 2
    print(a, b, c, d);                                // a = 6, b = -6, c = 0, d = 5
    cout << a << " " << ++a << " " << a++ << endl;    // 6 7 7
    //      6           ++a <=> a = a + 1 = 6 + 1 = 7, then print a = 7                print a = 7, then a++ <=> a = a + 1 = 7 + 1 = 8
    //    print a       inc a, then print a                                            print a, then inc a
    cout << "a = " << a << endl;    // a = 8
    return;
}

void prob2() {
    int a, b, c, d;
    // cin >> a >> b >> c >> d;
    a = 2;
    b = 0;
    c = 2;
    d = 5;
    c = a + b;            // c = 2 + 0 = 2
    print(a, b, c, d);    // a = 2, b = 0, c = 2, d = 5
    c = ++a + ++b;        // c = (++a <=> a = a + 1) + (++b <=> b = b + 1) = (a = 2 + 1 = 3) + (b = 0 + 1 = 1) = 3 + 1 = 4
    print(a, b, c, d);    // a = 3, b = 1, c = 4, d = 5
    c = a-- + b--;        // c = 3 + 1 = 4, then a-- <=> a = a - 1 = 3 - 1 = 2 and b-- <=> b = b - 1 = 1 - 1 = 0
    print(a, b, c, d);    // a = 2, b = 0, c = 4, d = 5
    c = --a + b++;        // c = (--a <=> a = a - 1 = 2 - 1 = 1) + b = 1 + 0 = 1, then b++ <=> b = b + 1 = 0 + 1 = 1
    print(a, b, c, d);    // a = 1, b = 1, c = 1, d = 5
    return;
}

void prob3() {
    int a, b, c, d;
    // cin >> a >> b >> c >> d;
    a = 2;
    b = 0;
    c = 2;
    d = 5;
    c = (++a) * (b--) + c++;    // c = (++a <=> a = a + 1 = 2 + 1 = 3) * b + c = 3 * 0 + 2 = 2, then b-- <=> b = b - 1 = 0 - 1 = -1, however c++ <=> return c and c = (old c) c + 1 = 2 + 1 = 3, but then c = 2 so it's no longer equal to 3, instead of that, it is equal to 2
    print(a, b, c, d);          // a = 3, b = -1, c = 2, d = 5
    // int d = (a++ * ++b) - (--c); // CE (Compilation Error) as 'int d' previously declared above
    d = (a++ * ++b) - (--c);    // d = a * (++b <=> b = b + 1 = -1 + 1 = 0) - (--c <=> c = c - 1 = 2 - 1 = 1) = 3 * 0 - 1 = -1, then a++ <=> a = a + 1 = 3 + 1 = 4
    print(a, b, c, d);          // a = 4, b = 0, c = 1, d = -1
    return;
}

void prob4() {
    // int a, b, c, d;
    // // cin >> a >> b >> c >> d;
    // a = 2;
    // b = 0;
    // c = 2;
    // d = 5;
    // // cout << (a++)-- << endl;    // CE (Compilation Error): error: lvalue required as decrement operand
    // // cout << ++(a++) << endl;    // CE (Compilation Error): error: lvalue required as decrement operand
    // // int d = (a++) + (a++);    // CE (Compilation Error) as 'int d' previously declared above
    // d = (a++) + (a++);             // d = (old a) a, then a++ <=> a = a + 1 = 2 + 1 = 3, + (new a) a = 2 + 3 = 5, then a++ <=> a = a + 1 = 3 + 1 = 4
    // cout << "d: " << d << endl;    // d: 5
    // print(a, b, c, d);             // a = 4, b = 0, c = 2, d = 5

    // !!! Remember to comment all above to run code below this !!!

    const int a = 3;
    // a++;    // CE (Compilation Error): error: increment of read-only variable 'a'
    cout << "a = " << a << endl;    // a = 3

    int c = 0;
    int d = (c += 3) * (c += 4);    // c += 3 <=> c = c + 3 = 3, c += 4 <=> c = c + 4 = 3 + 4 = 7, d = c * c = 7 * 7 = 49
    // Normally, we will think like this: d = (c = c + 3 = 0 + 3 = 3) * (c = c + 4 = 3 + 4 = 7) = 3 * 7 = 21, but the result is 49
    cout << "c = " << c << ", d = " << d << endl;    // c = 7, d = 49
    return;
}

int main() {
    cout << "Prob 1.1: " << endl;
    prob1();
    cout << endl;
    cout << "Prob 1.2: " << endl;
    prob2();
    cout << endl;
    cout << "Prob 1.3: " << endl;
    prob3();
    cout << endl;
    cout << "Prob 1.4: " << endl;
    prob4();
    cout << endl;
    return 0;
}