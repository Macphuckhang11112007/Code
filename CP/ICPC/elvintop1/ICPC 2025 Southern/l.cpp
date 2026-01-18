#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to compute sum of digits of a number
long long sum_of_digits(long long n) {
    long long sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

// Function to check if a number is beautiful
bool is_beautiful(long long n) {
    // Check for the first condition: n = a + b and S(a) = S(b)
    for (long long a = 1; a < n; ++a) {
        long long b = n - a;
        if (sum_of_digits(a) == sum_of_digits(b)) {
            // Check for the second condition: n = c + d + e and S(c) = S(d) = S(e)
            for (long long c = 1; c < n; ++c) {
                for (long long d = c; d < n; ++d) {
                    long long e = n - c - d;
                    if (e >= d && sum_of_digits(c) == sum_of_digits(d) && sum_of_digits(d) == sum_of_digits(e)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Precompute beautiful numbers in a reasonable range (let's assume 10^6 for now)
vector<long long> precompute_beautiful_numbers(long long limit) {
    vector<long long> beautiful_numbers;
    for (long long i = 1; i <= limit; ++i) {
        if (is_beautiful(i)) {
            beautiful_numbers.push_back(i);
        }
    }
    return beautiful_numbers;
}

// Binary search to count beautiful numbers in the range [L, R]
int count_beautiful_numbers_in_range(const vector<long long>& beautiful_numbers, long long L, long long R) {
    auto lower = lower_bound(beautiful_numbers.begin(), beautiful_numbers.end(), L);
    auto upper = upper_bound(beautiful_numbers.begin(), beautiful_numbers.end(), R);
    return upper - lower;
}

int main() {
    // Step 1: Precompute beautiful numbers up to a reasonable limit (for demonstration purposes, let's use 10^6)
    long long limit = 1000000;
    vector<long long> beautiful_numbers = precompute_beautiful_numbers(limit);
    
    // Step 2: Read the number of queries
    int q;
    cin >> q;
    
    // Step 3: Process each query
    for (int i = 0; i < q; ++i) {
        long long L, R;
        cin >> L >> R;
        
        // Step 4: Count beautiful numbers in the range [L, R]
        cout << count_beautiful_numbers_in_range(beautiful_numbers, L, R) << endl;
    }

    return 0;
}
