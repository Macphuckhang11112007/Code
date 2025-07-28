#undef _GLIBCXX_DEBUG

// The <bits/stdc++.h> header is a non-standard GNU C++ library.
// It is widely used in competitive programming (CP) to conveniently include all
// standard libraries, avoiding the need for individual includes.
#include <bits/stdc++.h>

// Pragmas specific to the GCC/G++ compiler for optimization.
// They instruct the compiler to apply the most aggressive optimization
// techniques to increase the program's execution speed.
#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("arch=tigerlake")

// Define a macro named `ll` as a substitute for `long long`.
// This helps in writing shorter code. `long long` is necessary to
// store the sum of rows/columns as the values can be very large.
#define ll long long

// Declare the use of the `std` namespace to avoid typing `std::`
// before standard library components like `cout`, `cin`, `vector`.
using namespace std;

int main()
{
    // --- SETUP AND DATA INPUT SECTION ---

    // These two lines significantly speed up input/output (I/O).
    // `sync_with_stdio(0)` disables synchronization between C++ and C I/O
    // streams. `cin.tie(0)` unties `cin` from `cout`.
    cin.tie(0)->sync_with_stdio(0);

    // Declare and read 3 integer inputs:
    // n: number of rows in the matrix
    // m: number of columns in the matrix
    // q: number of queries to process
    int n, m, q;
    cin >> n >> m >> q;

    // --- DATA STRUCTURE DECLARATION SECTION ---
    // This is the core part that determines the program's performance.

    // Declare a 2D matrix using `vector<vector<ll>>`.
    // The size is (n+1) x (m+1) to use 1-based indexing.
    // `vector` allocates memory on the HEAP, avoiding STACK overflow errors
    // which are common when working with large arrays.
    vector<vector<ll>> matrix(n + 1, vector<ll>(m + 1));

    // Vector `sum_row` to store the sum of each ORIGINAL row.
    // `sum_row[i]` will contain the sum of all elements in the original i-th
    // row.
    vector<ll> sum_row(n + 1, 0);

    // Vector `sum_col` to store the sum of each column.
    // `sum_col[j]` will contain the sum of all elements in the j-th column.
    vector<ll> sum_col(m + 1, 0);

    // The `org_row` vector is the key to handling row swaps in O(1).
    // It acts as a mapping layer: `org_row[i]` stores the index of the ORIGINAL
    // row that is currently displayed at the i-th row position.
    vector<int> org_row(n + 1);

    // The `iota` function (from the <numeric> library) is used to quickly
    // initialize the `org_row` vector. It fills the vector with values 0, 1, 2,
    // ..., n. Initially, the displayed row `i` is the original row `i`.
    iota(org_row.begin(), org_row.end(), 0);

    // --- PRE-COMPUTATION SECTION ---

    // This loop reads data for the matrix and simultaneously calculates the sum
    // of rows and columns. This is an O(N*M) time investment so that subsequent
    // queries can be performed in O(1) time.
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            // Read the value of cell (i, j)
            cin >> matrix[i][j];
            // Add the read value to the sum of the original row i
            sum_row[i] += matrix[i][j];
            // Add the read value to the sum of column j
            sum_col[j] += matrix[i][j];
        }
    }

    // --- QUERY PROCESSING SECTION ---

    // Start the loop to process `q` queries.
    while (q--)
    {
        // Read the first character of the line to determine the query type.
        char type;
        cin >> type;

        // Case 1: Swap two rows (O(1) complexity)
        if (type == '1')
        {
            int r1, r2;
            cin >> r1 >> r2;
            // Instead of swapping the data of both rows (which costs O(m)), we
            // just need to swap two indices in the `org_row` mapping vector.
            // This is an extremely fast operation.
            swap(org_row[r1], org_row[r2]);
        }
        // Case 2: Change the value of a cell (O(1) complexity)
        else if (type == '2')
        {
            int r, c, val;
            cin >> r >> c >> val;

            // From the current row position `r`, we find the actual original
            // row index through the `org_row` mapping vector.
            int actual_row = org_row[r];

            // Get the old value of the cell that is about to be changed.
            ll old_val = matrix[actual_row][c];

            // Update the pre-calculated sums.
            // We subtract the old value and add the new value.
            sum_row[actual_row] = sum_row[actual_row] - old_val + val;
            sum_col[c]          = sum_col[c] - old_val + val;

            // Finally, update the new value in the original data matrix.
            matrix[actual_row][c] = val;
        }
        // Case 3: Print the sum of a row (O(1) complexity)
        else if (type == 'r')
        {
            int r;
            cin >> r;
            // Get the original row index from `org_row[r]` and directly access
            // the pre-calculated sum in `sum_row`.
            cout << sum_row[org_row[r]] << "\n";
        }
        // Case 4: Print the sum of a column (O(1) complexity)
        else if (type == 'c')
        {
            int c;
            cin >> c;
            // Swapping rows does not affect the column sums, so we just need
            // to access `sum_col` directly to get the result.
            cout << sum_col[c] << "\n";
        }
    }

    // --- FINAL RESULT PRINTING SECTION ---

    // Loop to print the final state of the matrix after all queries.
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            // To print the cell at the current row `i` and column `j`, we must
            // get the data from the original row `org_row[i]` and column `j` in
            // the data matrix.
            cout << matrix[org_row[i]][j] << (j == m ? "" : " ");
        }
        // Newline after printing a row.
        cout << "\n";
    }

    // End the program, return 0 to indicate successful execution.
    return 0;
}
