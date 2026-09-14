#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 20;


// 核心：状态压缩
// f(status,i)表示从处于status状态且村庄i最后抵达的最短路
// 枚举下一个抵达的村庄j
// dp[s][i]=min(dp[s|(1<<j)][j]+dist[i][j])

ll dist[MAXN + 1][MAXN + 1];
ll dp[1 << MAXN][MAXN + 1];
ll solve() {
    //模板例题:https://www.luogu.com.cn/problem/P1171
    int n;
    cin >> n;
    memset(dp, 0x7f7f7f7f7f7f7f7f, sizeof(dp));
    for (int i = 1;i <= n;i++) {
        for (int j = 1;j <= n;j++) {
            cin >> dist[i][j];
        }
    }
    //n=4,status=111(位状态仅存储非起点)
    int status = (1 << (n - 1)) - 1;
    for (int j = 2;j <= n;j++) {
        dp[status][j] = dist[j][1];
    }
    ll ans = 1e18;
    //s的位状态表示2~n
    for (int s = status;s >= 0;s--) {
        for (int i = 2;i <= n;i++) {
            if ((s >> (i - 2)) & 1) {
                //位置i最后抵达
                for (int j = 2;j <= n;j++) {
                    if (!((s >> (j - 2)) & 1)) {
                        //下一个位置抵达j
                        dp[s][i] = min(dp[s][i], dp[s | (1 << (j - 2))][j] + dist[i][j]);
                    }
                }
            }
        }
    }
    //枚举第一个抵达的位置
    for (int i = 2;i <= n;i++) {
        ans = min(ans, dp[1 << (i - 2)][i] + dist[1][i]);
    }
    return ans;
}
