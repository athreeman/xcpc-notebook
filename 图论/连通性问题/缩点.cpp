#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 2e5;
const int MAXM = 2e5;

// 缩点: 基于SCC的强连通分量概念, 把这个极大的点集抽象为一个超级点P, 就是缩点
// 缩点后的图是一个有向无环图(DAG)
// 缩点往往和SCC结合, 此处只展示缩点的核心代码
// 根据缩点建图时, 复用同样的数组, 但是点的编号 + n(注意内存要二倍空间)

int n, m;
int belong[MAXN + 1];// belong[u]: 节点u属于哪个强连通分量
ll edgeArr[MAXM + 1];// 读入数据时存入的边
int Ei = 0;

int head[2 * MAXN + 1];
int to[2 * MAXM + 1];
int nxt[2 * MAXM + 1];
void addEdge(int u, int v) {}

// 对于缩点后的图, 节点编号是SCC的编号
// 事实上, 大多数情况下, 出度或入度才是需要维护的核心信息, 并不需要真实的建一个图出来

// 以下只展示核心代码
void condense() {

    Ei = 0;
    edgeArr[0] = -1;
    for (int u = 1;u <= n;u++) {
        for (int edge = head[u];edge > 0;edge = nxt[edge]) {
            int v = to[edge];
            // 分别使用高位和低位去存储, 题目给定的点的个数不会很大, 30位足够了
            if (belong[u] != belong[v]) {
                edgeArr[++Ei] = (ll(belong[u]) << 30) + ll(belong[v]);
            }
        }
    }
    sort(edgeArr, edgeArr + Ei + 1);
    for (int i = 1;i <= Ei;i++) {
        if (edgeArr[i] != edgeArr[i - 1]) {
            int u = edgeArr[i] >> 30;
            int v = edgeArr[i] - (ll(u) << 30);
            addEdge(u + n, v + n);
        }
    }

}