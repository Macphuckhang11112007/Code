// MPK

#ifndef THEMIS
#ifndef ONLINE_JUDFE
#define _GLIBCXX_DEBUG
#endif
#pragma GCC optimize("Ofast,inline,unroll-loops,omit-frame-pointer")
#if __GNUC__ == 14 && __cplusplus > 202002L
#pragma GCC target("arch=x86-64")
#else
#pragma GCC target("arch=corei7-avx")
#endif
#endif

#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

typedef unsigned int       uint;
typedef unsigned long long ull;
typedef long long          ll;
typedef __int128_t         lll;
typedef __uint128_t        ulll;
typedef double             dbl;
typedef long double        ldbl;

template <typename T, typename... Args>
void PRINTLN(T x, Args... args) {
    cout << x << "\n";
    if constexpr (sizeof...(args))
        PRINTLN(args...);
}

int tc = 1;

void SetIO(int mtc = 0, string file = "", string inp = ".inp", string out = ".out") {
    if (file != "") {
        fstream fin(file + inp, ios::in);
        fstream fout(file + out, ios::out);
        cin.rdbuf(fin.rdbuf());
        cout.rdbuf(fout.rdbuf());
    }
    cin.tie(0)->sync_with_stdio(0);
    __builtin_ia32_ldmxcsr(40896);
    if (mtc == 1) {
        cin >> tc;
    } else if (mtc == -1) {
        tc = -1;
    }
    return;
}

// -------------------- [Entry Point] --------------------
ll n;

void Solve() {
    cin >> n;
    PRINTLN(1LL << (bit_width(ull(n)) - popcount(ull(n))));
    return;
}

signed Main() {
    SetIO();
    if (tc != -1) {
        for (int i = 1; i <= tc; i++) {
            Solve();
        }
    } else {
        while (1) {
            Solve();
        }
    }
    return 0;
}

signed main() {
#ifndef CPH
    auto time_start = high_resolution_clock::now();
    cerr << "\n\n";
#endif
    signed ret = Main();
#ifndef CPH
    auto   time_end = high_resolution_clock::now();
    double elapsed  = (time_end - time_start).count();
    cerr << "\n\n-------------------- [Execution Summary] --------------------\n";
    cerr << "| Execution Time" << ": ";
    cerr << fixed << setprecision(4) << elapsed * 1e3 << " ms" << setw(36 - to_string(int(elapsed * 1e3)).length()) << "|\n";
    cerr << "| Exit Code     : " << ret << setw(25 - to_string(ret).size() - 2 * !ret) << "(0x" << setfill('0') << setw(8) << hex << uppercase << ret << (ret == 0 ? ", Success) |" : ", Error) |") << "\n";
    cerr << "-------------------------------------------------------------\n\n";
#endif
    return ret;
}