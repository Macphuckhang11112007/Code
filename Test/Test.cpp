#undef _GLIBCXX_DEBUG

// Th? vi?n <bits/stdc++.h> là m?t th? vi?n không chu?n c?a GNU C++.
// Nó ???c s? d?ng ph? bi?n trong l?p trình thi ??u (CP) ?? bao g?m t?t c?
// các th? vi?n chu?n m?t cách ti?n l?i, tránh ph?i include l? t?.
#include <bits/stdc++.h>

// Các ch? th? (pragma) dành riêng cho trình biên d?ch GCC/G++ ?? t?i ?u hóa.
// Chúng yêu c?u trình biên d?ch áp d?ng các k? thu?t t?i ?u hóa m?nh m? nh?t
// ?? t?ng t?c ?? th?c thi c?a ch??ng trình.
#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("arch=tigerlake")

// ??nh ngh?a m?t macro tên `ll` ?? thay th? cho `long long`.
// Vi?c này giúp vi?t code ng?n g?n h?n. `long long` c?n thi?t ??
// ch?a t?ng c?a các hàng/c?t vì giá tr? có th? r?t l?n.
#define ll long long

// Khai báo s? d?ng không gian tên `std` ?? không c?n ph?i gõ `std::`
// tr??c các thành ph?n c?a th? vi?n chu?n nh? `cout`, `cin`, `vector`.
using namespace std;

int main()
{
    // --- PH?N THI?T L?P VÀ NH?P D? LI?U ---

    // Hai dòng này giúp t?ng t?c ?? nh?p xu?t (I/O) m?t cách ?áng k?.
    // `sync_with_stdio(0)` ng?t ??ng b? gi?a lu?ng I/O c?a C++ và C.
    // `cin.tie(0)` b? vi?c bu?c `cin` ph?i ??i `cout` x? lý xong.
    cin.tie(0)->sync_with_stdio(0);

    // Khai báo và ??c 3 s? nguyên ??u vào:
    // n: s? hàng c?a ma tr?n
    // m: s? c?t c?a ma tr?n
    // q: s? l??ng truy v?n c?n x? lý
    int n, m, q;
    cin >> n >> m >> q;

    // --- PH?N KHAI BÁO C?U TRÚC D? LI?U ---
    // ?ây là ph?n c?t lõi quy?t ??nh hi?u n?ng c?a ch??ng trình.

    // Khai báo ma tr?n 2 chi?u b?ng `vector<vector<ll>>`.
    // Kích th??c là (n+1) x (m+1) ?? s? d?ng ch? s? t? 1 (1-based indexing).
    // `vector` c?p phát b? nh? trên HEAP, tránh ???c l?i tràn b? nh? STACK
    // v?n r?t ph? bi?n khi làm vi?c v?i m?ng l?n.
    vector<vector<ll>> matrix(n + 1, vector<ll>(m + 1));

    // Vector `sum_row` ?? l?u tr? t?ng c?a m?i hàng G?C.
    // `sum_row[i]` s? ch?a t?ng c?a t?t c? các ph?n t? trên hàng g?c th? i.
    vector<ll> sum_row(n + 1, 0);

    // Vector `sum_col` ?? l?u tr? t?ng c?a m?i c?t.
    // `sum_col[j]` s? ch?a t?ng c?a t?t c? các ph?n t? trên c?t th? j.
    vector<ll> sum_col(n + 1, 0);

    // Vector `org_row` là chìa khóa ?? x? lý vi?c ??i hàng trong O(1).
    // Nó ho?t ??ng nh? m?t l?p ánh x?: `org_row[i]` l?u ch? s? c?a hàng G?C
    // mà hi?n t?i ?ang ???c hi?n th? ? v? trí hàng `i`.
    vector<int> org_row(n + 1);

    // Hàm `iota` (t? th? vi?n <numeric>) dùng ?? kh?i t?o nhanh vector
    // `org_row`. Nó s? ?i?n vào vector các giá tr? 0, 1, 2, ..., n. Ban ??u,
    // hàng hi?n th? `i` chính là hàng g?c `i`.
    iota(org_row.begin(), org_row.end(), 0);

    // --- PH?N TÍNH TOÁN TR??C (PRE-COMPUTATION) ---

    // Vòng l?p này ??c d? li?u cho ma tr?n và ??ng th?i tính t?ng các hàng, các
    // c?t. ?ây là m?t b??c ??u t? th?i gian O(N*M) ?? các truy v?n sau này có
    // th? ???c th?c hi?n trong th?i gian O(1).
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            // ??c giá tr? c?a ô (i, j)
            cin >> matrix[i][j];
            // C?ng d?n giá tr? v?a ??c vào t?ng c?a hàng g?c i
            sum_row[i] += matrix[i][j];
            // C?ng d?n giá tr? v?a ??c vào t?ng c?a c?t j
            sum_col[j] += matrix[i][j];
        }
    }

    // --- PH?N X? LÝ TRUY V?N ---

    // B?t ??u vòng l?p ?? x? lý `q` truy v?n.
    while (q--)
    {
        // ??c ký t? ??u tiên c?a dòng ?? xác ??nh lo?i truy v?n.
        char type;
        cin >> type;

        // Tr??ng h?p 1: ??i ch? hai hàng (?? ph?c t?p O(1))
        if (type == '1')
        {
            int r1, r2;
            cin >> r1 >> r2;
            // Thay vì ??i d? li?u c?a c? hai hàng (t?n O(m)), ta ch? c?n
            // hoán ??i hai ch? s? trong vector ánh x? `org_row`.
            // ?ây là m?t thao tác c?c k? nhanh.
            swap(org_row[r1], org_row[r2]);
        }
        // Tr??ng h?p 2: Thay ??i giá tr? m?t ô (?? ph?c t?p O(1))
        else if (type == '2')
        {
            int r, c, val;
            cin >> r >> c >> val;

            // T? v? trí hàng hi?n th? `r`, ta tìm ra ch? s? c?a hàng g?c th?c
            // s? thông qua vector ánh x? `org_row`.
            int actual_row = org_row[r];

            // L?y giá tr? c? c?a ô s?p ???c thay ??i.
            ll old_val = matrix[actual_row][c];

            // C?p nh?t l?i các t?ng ?ã tính tr??c.
            // Ta tr? ?i giá tr? c? và c?ng vào giá tr? m?i.
            sum_row[actual_row] = sum_row[actual_row] - old_val + val;
            sum_col[c]          = sum_col[c] - old_val + val;

            // Cu?i cùng, c?p nh?t giá tr? m?i vào ma tr?n d? li?u g?c.
            matrix[actual_row][c] = val;
        }
        // Tr??ng h?p 3: In ra t?ng c?a m?t hàng (?? ph?c t?p O(1))
        else if (type == 'r')
        {
            int r;
            cin >> r;
            // L?y ch? s? hàng g?c t? `org_row[r]` và truy c?p tr?c ti?p
            // vào t?ng ?ã ???c tính s?n trong `sum_row`.
            cout << sum_row[org_row[r]] << "\n";
        }
        // Tr??ng h?p 4: In ra t?ng c?a m?t c?t (?? ph?c t?p O(1))
        else if (type == 'c')
        {
            int c;
            cin >> c;
            // Vi?c ??i hàng không ?nh h??ng ??n t?ng c?t, nên ta ch? c?n
            // truy c?p tr?c ti?p vào `sum_col` ?? l?y k?t qu?.
            cout << sum_col[c] << "\n";
        }
    }

    // --- PH?N IN K?T QU? CU?I CÙNG ---

    // Vòng l?p ?? in ra tr?ng thái cu?i cùng c?a ma tr?n sau t?t c? các truy
    // v?n.
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            // ?? in ô ? hàng hi?n th? `i` và c?t `j`, ta ph?i l?y d? li?u t?
            // hàng g?c `org_row[i]` và c?t `j` trong ma tr?n d? li?u.
            cout << matrix[org_row[i]][j] << (j == m ? "" : " ");
        }
        // Xu?ng dòng sau khi in xong m?t hàng.
        cout << "\n";
    }

    // K?t thúc ch??ng trình, tr? v? 0 ?? báo hi?u ch??ng trình ?ã ch?y thành
    // công.
    return 0;
}
