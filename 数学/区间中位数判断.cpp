#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 相当一部分题目, 事实上只需要用到简单定义
// c0: <= x的元素个数
// c1: > x的元素个数的相反数
// 用这两个变量进行简单的判断即可, 严格的判断和区分需要c0, c1, c2三个变量

// 对于区间[l, r]
// 定义: 
// n:  区间长度
// c0: 若元素<=x, c0++
// c1: 若元素>x,  c1--

// 1、奇数区间
// x是区间中位数: |c0 - c2| <= c1 - 1

// 2、偶数区间
// x是区间上中位数(第n/2+1小): c0 <= n/2      &&  c2 <= (n-2)/2
// x是区间下中位数(第n/2小):   c0 <= (n-2)/2  &&  c2 <= n/2
// x既是上中位数, 又是下中位数: 
//                  c0 <= (n-2)/2  &&  c2 <= (n-2)/2
//           等价于: max(c0, c2) <= (n-2)/2
//           等价于: |c0 - c2| <= c1 - 2



// 1、奇数区间 (n 为奇数)
// x 不是中位数:
//     c0 >= (n+1)/2   或   c2 >= (n+1)/2
// 等价于: max(c0, c2) >= (n+1)/2
// 等价于: |c0 - c2| >= c1

// 2、偶数区间 (n 为偶数)
// x 不是上中位数(第 n/2+1 小):
//     c0 >= n/2+1   或   c2 >= n/2

// x 不是下中位数(第 n/2 小):
//     c0 >= n/2     或   c2 >= n/2+1

// x 既不是上中位数, 也不是下中位数:
//     c0 >= n/2+1   或   c2 >= n/2+1   或   (c0 == n/2 && c2 == n/2)

// x 不满足"既上又下"
//     c0 >= n/2     或   c2 >= n/2
// 等价于: max(c0, c2) >= n/2
// 等价于: |c0 - c2| >= c1 - 1
const int MAXN = 1e5;
int a[MAXN + 1];
int pre[MAXN + 1];
void solve() {

    // https://codeforces.com/contest/2056/problem/D

    // 针对例题: 我们只需要判断是否是坏区间即可, 且只需要判断偶数区间
    // 偶数区间是坏区间: 上中位数!=下中位数
    // 为了使判断条件足够简单, 我们枚举x作为区间的下中位数
    // 证明这样枚举的正确性: 
    // 1、x只作为下中位数, 上中位数!=x, 满足是坏区间
    // 2、x至少存在一次才纳入统计(去重), x若不存在, 下中位数y!=x, 这种情况会被其他枚举所包含
    // 3、当x只作为上中位数的情况不纳入统计(去重), 此时下中位数y!=x, 这种情况同样会被其他枚举所包含
    // 判断条件: c0 + c1 = 0
    int n;
    cin >> n;
    ll ans = ll(n) * ll(n + 1) / 2LL;;
    for (int i = 1;i <= n;i++) {
        cin >> a[i];
    }
    for (int x = 1;x <= 10;x++) {
        // <=x: +1
        //  >x: -1
        // last: 左侧最近的x(包括自己)
        // 非法区间[l, r]: pre[r] - pre[l - 1] = 0
        // l <= last
        pre[0] = 0;
        map<int, int>cnt;// 计数
        queue<array<int, 2>>q;// 待激活
        for (int last = -1, r = 1;r <= n;r++) {
            pre[r] = pre[r - 1] + (a[r] <= x ? 1 : -1);
            if (a[r] == x)last = r;
            while (q.size() && q.front()[1] <= last) {
                cnt[q.front()[0]]++;
                q.pop();
            }
            ans -= cnt[pre[r]];
            q.push({ pre[r - 1],r });
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
