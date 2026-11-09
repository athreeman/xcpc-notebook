#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using ill = __int128_t;

const ll inf = 1e9;
const ll INF = 1e18;
const ll NO = 1e15;
const ll mod = 998244353;
const int MAXN = 2e5;

ll x, y;

// 求 a∈[l1,r1], b∈[l2,r2], a&b==0 且区间非负时
// max/min(a+b), max/min(a-b)，并可回溯出一组 (a,b)
// 位宽自动取 max(r1,r2) 的最高位；值域需 < 2^62
// 复杂度 logV * 16 * 3

// https://codeforces.com/problemset/problem/2187/B

ll L1, R1, L2, R2;
ll dp[32][2][2][2][2];
int best[32][2][2][2][2];
vector<array<int, 2>>ch = { { 0,1 },{ 1,0 },{ 0,0 } };
// 0: 0,1
// 1: 1,0
// 2: 0,0
bool vis[32][2][2][2][2];

// 定义dp[i][l1][r1][l2][r2]
// 0~i-1位的状态已经确定
// 来到第i位, l1表示a目前取值是否等于下界, r1对应上界
// l2表示b取值是否等于下界, r2对应上界

// 规定res:
// 0: max(a+b)
// 1: min(a+b)
// 2: max(a-b)
// 3: min(a-b)
ll f(int i, int l1, int r1, int l2, int r2, int res) {
    if (i < 0)return 0;
    ll ans = dp[i][l1][r1][l2][r2];
    if (vis[i][l1][r1][l2][r2])return ans;
    vis[i][l1][r1][l2][r2] = true;
    ans = (res == 0 || res == 2) ? -INF : INF;
    int choice = -1;
    for (int j = 0;j < 3;j++) {
        auto& [pa, pb] = ch[j];
        // 在当前为的上下界
        int lo1 = l1 ? (((L1 >> i) & 1) ? 1 : 0) : 0;
        int hi1 = r1 ? (((R1 >> i) & 1) ? 1 : 0) : 1;
        if (pa<lo1 || pa>hi1 || lo1 > hi1)continue;
        int lo2 = l2 ? (((L2 >> i) & 1) ? 1 : 0) : 0;
        int hi2 = r2 ? (((R2 >> i) & 1) ? 1 : 0) : 1;
        if (pb<lo2 || pb>hi2 || lo2 > hi2)continue;

        int nl1 = l1 && pa == ((L1 >> i) & 1) ? 1 : 0;
        int nr1 = r1 && pa == ((R1 >> i) & 1) ? 1 : 0;
        int nl2 = l2 && pb == ((L2 >> i) & 1) ? 1 : 0;
        int nr2 = r2 && pb == ((R2 >> i) & 1) ? 1 : 0;

        ll low = f(i - 1, nl1, nr1, nl2, nr2, res);
        if (low <= -INF / 2 || low >= INF / 2)continue;
        ll add = (pa + pb) * (1LL << i) + low;
        ll sub = (pa - pb) * (1LL << i) + low;
        if (res == 0 && add > ans) {
            ans = max(ans, add);
            choice = j;
        }
        else if (res == 1 && add < ans) {
            ans = min(ans, add);
            choice = j;
        }
        else if (res == 2 && sub > ans) {
            ans = max(ans, sub);
            choice = j;
        }
        else if (res == 3 && sub < ans) {
            ans = min(ans, sub);
            choice = j;
        }
    }
    best[i][l1][r1][l2][r2] = choice;
    dp[i][l1][r1][l2][r2] = ans;
    return ans;
}

void init() {
    memset(vis, false, sizeof(vis));
}

// 回溯出a, b的值
array<ll, 2>GetVal() {
    ll a = 0, b = 0;
    int i = 31;
    int l1 = 1, r1 = 1, l2 = 1, r2 = 1;
    while (i >= 0) {
        auto& [pa, pb] = ch[best[i][l1][r1][l2][r2]];
        a += pa * (1LL << i);
        b += pb * (1LL << i);
        l1 = l1 && (pa == ((L1 >> i) & 1));
        r1 = r1 && (pa == ((R1 >> i) & 1));
        l2 = l2 && (pb == ((L2 >> i) & 1));
        r2 = r2 && (pb == ((R2 >> i) & 1));
        i--;
    }
    return { a,b };
}

void solve() {

    // 求|x - p| + |y - q|的最小值的一对(p, q)
    // x >= p, y >= q  x + y - (p + q)
    // x >= p, y  < q  x - y - (p - q)
    // x  < p, y >= q  y - x + (p - q)
    // x  < p, y  < q  (p + q) - x - y
    cin >> x >> y;
    ll UP = (1LL << 31) - 1LL;
    ll p, q, ans = INF;
    array<ll, 2>val;

    init();L1 = 0, R1 = x, L2 = 0, R2 = y;
    ll v = x + y - f(31, 1, 1, 1, 1, 0);
    if (llabs(v) <= INF / 2) {
        if (v < ans) {
            ans = v;
            val = GetVal();
            p = val[0], q = val[1];
        }
    }

    init();L1 = 0, R1 = x, L2 = y + 1, R2 = UP;
    v = x - y - f(31, 1, 1, 1, 1, 2);
    if (llabs(v) <= INF / 2) {
        if (v < ans) {
            ans = v;
            val = GetVal();
            p = val[0], q = val[1];
        }
    }

    init();L1 = x + 1, R1 = UP, L2 = 0, R2 = y;
    v = y - x + f(31, 1, 1, 1, 1, 3);
    if (llabs(v) <= INF / 2) {
        if (v < ans) {
            ans = v;
            val = GetVal();
            p = val[0], q = val[1];
        }
    }

    init();L1 = x + 1, R1 = UP, L2 = y + 1, R2 = UP;
    v = f(31, 1, 1, 1, 1, 1) - x - y;
    if (llabs(v) <= INF / 2) {
        if (v < ans) {
            ans = v;
            val = GetVal();
            p = val[0], q = val[1];
        }
    }

    cout << p << ' ' << q << '\n';

}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}