#include <bits/stdc++.h>

using namespace std;

// Chọn B khoảng sqrt(N)
const int B = 450;

int main()
{
    fstream fin("Test.inp", ios::in);
    fstream fout("Test.out", ios::out);
    cin.rdbuf(fin.rdbuf());
    cout.rdbuf(fout.rdbuf());
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;

    vector<int> a(n);
    vector<int> unique_vals;
    unique_vals.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> a[i];
        unique_vals.push_back(a[i]);
    }

    // Rời rạc hóa giá trị
    sort(unique_vals.begin(), unique_vals.end());
    unique_vals.erase(unique(unique_vals.begin(), unique_vals.end()),
                      unique_vals.end());

    vector<int> compressed_a(n);
    int         num_unique = unique_vals.size();
    for (int i = 0; i < n; ++i)
    {
        compressed_a[i] =
            lower_bound(unique_vals.begin(), unique_vals.end(), a[i]) -
            unique_vals.begin();
    }

    vector<int> freq(num_unique, 0);
    for (int x : compressed_a) { freq[x]++; }

    vector<bool> is_heavy(num_unique, false);
    for (int i = 0; i < num_unique; ++i)
    {
        if (freq[i] > B) { is_heavy[i] = true; }
    }

    long long ans = 0;

    // --- Phần 1: Xử lý phần tử Nặng ---
    vector<int> p(n + 1, 0);
    vector<int> bit(2 * n + 2, 0);

    auto update = [&](int idx, int val)
    {
        for (; idx < bit.size(); idx += idx & -idx) bit[idx] += val;
    };
    auto query = [&](int idx)
    {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
        return sum;
    };

    for (int v = 0; v < num_unique; ++v)
    {
        if (!is_heavy[v]) continue;

        for (int i = 0; i < n; ++i)
        {
            p[i + 1] = p[i] + (compressed_a[i] == v ? 1 : -1);
        }

        fill(bit.begin(), bit.end(), 0);

        // Offset n+1 để các chỉ số không âm
        update(p[0] + n + 1, 1);
        for (int j = 1; j <= n; ++j)
        {
            ans += query(p[j] + n);
            update(p[j] + n + 1, 1);
        }
    }

    // --- Phần 2: Xử lý phần tử Nhẹ ---
    vector<int> current_freq(num_unique, 0);
    vector<int> window_vals;
    window_vals.reserve(2 * B);

    for (int L = 0; L < n; ++L)
    {
        // Biến này không thực sự cần, chỉ để minh họa logic break
        int max_local_freq = 0;

        for (int R = L; R < n; ++R)
        {
            int val = compressed_a[R];
            current_freq[val]++;

            // Điều kiện break then chốt
            if (current_freq[val] > B)
            {
                // Dọn dẹp tần suất cho L tiếp theo
                for (int k = L; k <= R; ++k)
                {
                    current_freq[compressed_a[k]]--;
                }
                break;
            }

            // Chỉ xét các đoạn mà mọi phần tử có freq cục bộ <= B
            // Tìm mode của đoạn [L, R]
            int mode  = -1;
            int max_f = 0;
            // Việc tìm lại mode mỗi bước hơi chậm, nhưng vì cửa sổ nhỏ (dài <=
            // N*B trong tổng số) nên vẫn chấp nhận được. Tối ưu hơn có thể duy
            // trì mode. Để an toàn, ta tìm lại. Đoạn này có thể tối ưu hơn,
            // nhưng logic này là an toàn nhất.
            int current_mode     = compressed_a[L];
            int current_max_freq = 0;
            for (int k = L; k <= R; ++k)
            {
                if (current_freq[compressed_a[k]] > current_max_freq)
                {
                    current_max_freq = current_freq[compressed_a[k]];
                    current_mode     = compressed_a[k];
                }
            }

            // Chỉ đếm nếu mode là phần tử nhẹ
            if (!is_heavy[current_mode])
            {
                if (2LL * current_max_freq > (R - L + 1)) { ans++; }
            }

            // Dọn dẹp ở cuối vòng L nếu R chạm đến cuối mảng
            if (R == n - 1)
            {
                for (int k = L; k <= R; ++k)
                {
                    current_freq[compressed_a[k]]--;
                }
            }
        }
    }

    cout << ans << endl;

    return 0;
}