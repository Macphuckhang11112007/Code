// MPK

#include <bits/stdc++.h>

using namespace std;

int t;

int Point(string card)
{
    if (card == "J" || card == "Q" || card == "K") { return 10; }
    else if (card == "A") { return 11; }
    else if ("1" <= card && card <= "10") { return stoi(card); }
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        string card_1, card_2;
        cin >> card_1 >> card_2;
        if (card_2 == "A") { swap(card_1, card_2); }  // Prioritize for A
        int point = Point(card_1) +
                    Point(card_2);  // Calculate current sum in "basic" theory
        if (point == 21)
        {
            cout << "Blackjack\n";
            continue;
        }
        else if (point > 21)
        {
            point = 11 + 1;  // [A, A] -> [11, 11] --> [11, 1]
            point += 9;      // [A, A, 9] -> [11, 1, 9] -> point = 21
        }
        else if (point < 21)
        {
            if (point + 11 <= 21) { point += 11; }
            else { point = 21; }
        }
        cout << point << '\n';
    }
    return 0;
}
