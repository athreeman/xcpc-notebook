#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int inf = 1e9;
const ll INF = 1e18;

const int MAXN = 12;
const int MAXV = 5e5;
int a[MAXN + 1];
ll dist[MAXV + 1];

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

void Cycle(int n) {
    int x = a[1];
    fill(dist, dist + x, INF);
    dist[0] = 0;
    for (int i = 2;i <= n;i++) {
        int d = gcd(a[i], x);
        for (int st = 0;st < d;st++) {
            for (int cur = st, next, round = 0;round < 2;round += (cur == st ? 1 : 0)) {
                next = (cur + a[i]) % x;
                if (dist[cur] != INF) {
                    dist[next] = min(dist[next], dist[cur] + a[i]);
                }
                cur = next;
            }
        }
    }
}

ll Cal(ll r, int x) {
    if (r < 0)return 0;
    ll ans = 0;
    for (int i = 0;i < x;i++) {
        if (r >= dist[i]) {
            ans += max(0LL, (r - dist[i]) / x + 1);
        }
    }
    return ans;
}

void solve() {
    ll n, l, r;
    cin >> n >> l >> r;
    for (int i = 1;i <= n;i++) {
        cin >> a[i];
    }
    int siz = 0;
    sort(a + 1, a + n + 1);
    for (int i = 1;i <= n;i++) {
        if (a[i] != 0) {
            a[++siz] = a[i];
        }
    }
    if (siz == 0) {
        cout << 0 << '\n';
    }
    else {
        Cycle(siz);
        cout << Cal(r, a[1]) - Cal(l - 1, a[1]) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
