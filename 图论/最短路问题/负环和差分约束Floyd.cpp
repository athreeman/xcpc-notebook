#include<bits/stdc++.h>
using namespace std;

// 模板：https://www.luogu.com.cn/problem/P2474
// Floyd算法核心在于u, v关系通过bridge桥接
// 该题只要求判断a,b和u,v这两队的关系
// 且给定了任意两个的某种关系, 因此可以使用Floyd算法把该关系严格化

int dmin[51][51];// i和j的最小可能差值
int dmax[51][51];// i和j的最大可能差值

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    for (int i = 1;i <= n;i++) {
        for (int j = 1;j <= n;j++) {
            char r;
            cin >> r;
            if (r == '=') {
                dmin[i][j] = dmax[i][j] = 0;
            }
            else if (r == '-') {
                // i < j
                dmin[i][j] = -2;
                dmax[i][j] = -1;
            }
            else if (r == '+') {
                // i > j
                dmin[i][j] = 1;
                dmax[i][j] = 2;
            }
            else {
                dmin[i][j] = -2;
                dmax[i][j] = 2;
            }
        }
    }
    for (int i = 1;i <= n;i++) {
        dmin[i][i] = dmax[i][i] = 0;
    }
    // 桥接, 把两两的关系更严格化
    for (int bridge = 1;bridge <= n;bridge++) {
        for (int i = 1;i <= n;i++) {
            for (int j = 1;j <= n;j++) {
                dmin[i][j] = max(dmin[i][j], dmin[i][bridge] + dmin[bridge][j]);
                dmax[i][j] = min(dmax[i][j], dmax[i][bridge] + dmax[bridge][j]);
            }
        }
    }
    int ans1 = 0, ans2 = 0, ans3 = 0;
    for (int i = 1;i <= n;i++) {
        for (int j = i + 1;j <= n;j++) {
            if (i != a && j != a && i != b && j != b) {
                if (dmin[a][i] > dmax[j][b] || dmin[a][j] > dmax[i][b]) {
                    ans1++;
                }
                if (dmax[a][i] < dmin[j][b] || dmax[a][j] < dmin[i][b]) {
                    ans3++;
                }
                if (dmin[a][i] == dmax[a][i] && dmin[j][b] == dmax[j][b] && dmin[a][i] == dmin[j][b]) {
                    ans2++;
                }
                else if (dmin[b][i] == dmax[b][i] && dmin[j][a] == dmax[j][a] && dmin[j][a] == dmin[b][i]) {
                    ans2++;
                }
            }
        }
    }
    cout << ans1 << ' ' << ans2 << ' ' << ans3 << '\n';
}
int main() {
    solve();
    return 0;
}
