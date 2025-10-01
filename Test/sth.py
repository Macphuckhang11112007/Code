from math import *
q = int(input())
for _q in range(q):
    ans = 0
    a, b, n = map(int, input().split())
    x = lcm(a, b)
    l = min(a, b)
    r = n * x
    while l <= r:
        m = l + (r - l) // 2
        c = m // a + m // b - m // x
        if c < n:
            l = m + 1
        elif c > n:
            r = m - 1
        else:
            if m % a == 0 or m % b == 0:
                ans = m
            r = m - 1
    print(ans)
