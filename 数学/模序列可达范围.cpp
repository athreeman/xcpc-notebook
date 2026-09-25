#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 给定整数a, b, c
// b > 0, a >= 0, c >= 0
// q[k] = (a + k * c) % b, k >= 0

// 存在解q[k] = t
// 需要满足: (t - a) % gcd(b, c) = 0

// 可达集合S
// g = gcd(b, c), T = b / g, r = a % g

// S: 0 <= s < b, (满足s % g = a % g)
// S: r + j * g, (满足0 <= j <= T - 1)
// Smax = b - 1 - ((b - 1 - a) % g), (条件: a < b)
// Smax = b - g + (a % g), (条件: a >= 0)
// 特例:
// 1.当gcd(b, c) = 1, 取值[0, b - 1]
// 2. c % b = 0 且 a < b, 取值{ a }
// 3. 一般地, 可达点只有T个

ll a, b, c;
ll g, T, r;

ll Max() {
    return b - g + r;
}

void printRange() {
    for (ll j = 0;j <= T - 1;j++) {
        cout << r + j * g << ' ';
    }
    cout << '\n';
}

void solve() {
    cin >> a >> b >> c;
    // 前提: b >= 0, a, c > 0
    g = gcd(b, c);
    T = b / g;
    r = a % g;
    cout << Max() << '\n';
    printRange();
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}
