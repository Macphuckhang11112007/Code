#include <iostream>

using namespace std;

int main() {
    int strawberry_chocolates, matcha_chocolates, Dubai_chocolates;
    int price_strawberry_chocolates, price_matcha_chocolates, price_Dubai_chocolates;
    // cin >> strawberry_chocolates >> matcha_chocolates >> Dubai_chocolates;
    // cin >> price_strawberry_chocolates >> price_matcha_chocolates >> price_Dubai_chocolates;
    strawberry_chocolates       = 5;
    matcha_chocolates           = 3;
    Dubai_chocolates            = 10;
    int total_chocolates        = strawberry_chocolates + matcha_chocolates + Dubai_chocolates;    // total_chocolates = 5 + 3 + 10 = 18
    price_strawberry_chocolates = 20'000;
    price_matcha_chocolates     = 30'000;
    price_Dubai_chocolates      = 50'000;
    int total_price             = price_strawberry_chocolates * strawberry_chocolates + price_matcha_chocolates * matcha_chocolates + price_Dubai_chocolates * Dubai_chocolates;    // total_price = 20,000 * 5 + 30,000 * 3 + 50,000 * 10 = 690,000
    cout << "Total: " << total_chocolates << " chocolates" << endl;                                                                                                                 // Total: 18 chocolates
    cout << "Total price: " << total_price << " VND" << endl;                                                                                                                       // Total price: 690,000 VND
    return 0;
}