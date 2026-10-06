#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 2e5;
const int MAXM = 2e5;

// 强连通分量(SCC): 任何有向图都可以
// 一个图中, 若干点构成的点集S, 如果点集S内的点可以两两抵达
// 且S是满足该性质的极大点集, 这些点就构成一个SCC(单点也符合)
// 把这些点抽象为一个超级点放入图, 就称为缩点
// 缩点后的超级点构成的图是一个有向无环图(DAG)
// 在此基础上, 执行一些图上算法, 或者是dp等算法, 会变得很容易实现
// 求解SCC, 算竞掌握Tarjan求解即可

// 误区: SCC并不一定是一个环, 可能是多个互相可达的环的并集

// 核心维护数据如下:

// 树边: 遍历时, 通过一条边, 抵达一个未分配dfn序的点
// 非树边: 遍历时, 通过一条边, 抵达一个已经分配dfn序的点
// 回边: 遍历时, 通过一条非树边, 抵达一个未归属(任意SCC)的点
// 弃边: 遍历时, 通过一条非树边, 抵达一个已经归属的点

int dfn[MAXN + 1];// dfn序
int low[MAXN + 1];// low[u]: 点u及其子树上, 经过至多一条回边能抵达的dfn最小的点(未必是祖先)
int belong[MAXN + 1];// belong[u]: 节点u属于哪个强连通分量
int dfnIdx = 0;
int sccIdx = 0;

// 流程:
// 1、初次抵达u, 设置dfn[u], low[u] = dfn[u], 入栈
// 2、遍历儿子, 回溯时low[u] = min(low[u], low[v])
// 3、遇到弃边, 不做处理
// 4、遇到回边, low[u] = min(low[u], dfn[v])
// 5、回溯时, 遇到dfn[u] = low[u]的点, 弹出直到u也被弹出, 弹出的这些点归为一个belong
// 6、考察图中每个点, 一旦发现dfn[i] == 0, 就跑一遍, 因为可能出现森林

// 时间复杂度: O(n + m), n:点, m:边

// 扩展(实际应用, 不推荐该写法, 且第一种写法含义更为精确清晰)：
// Tarjan当遇到回边时, 存在另一种写法: low[u] = min(low[u], low[v])
// 此时, low[u]的定义为: 点u及其子树上, 允许走若干条回边能抵达的dfn最小的点(未必是祖先), 不保证穷尽所有可能性
// 这种写法和定义, 依然可以保证Tarjan的正确性

struct Stack {// 静态栈
    int data[MAXN + 1], si = 0;

    void push(int val) { data[++si] = val; }
    int top() { return data[si]; }
    int pop() { return data[si--]; }
    void clear() { si = 0; }
    bool empty() { return si == 0; }
    int size() { return si; }
}sta;

int n, m;

int head[MAXN + 1];
int to[MAXM + 1];
int nxt[MAXM + 1];
int ei = 0;

void addEdge(int u, int v) {
    to[++ei] = v;
    nxt[ei] = head[u];
    head[u] = ei;
}

// 核心代码如下:
void Tarjan(int u) {
    sta.push(u);
    dfn[u] = ++dfnIdx;
    low[u] = dfn[u];
    for (int edge = head[u];edge > 0;edge = nxt[edge]) {
        int v = to[edge];
        if (dfn[v] == 0) {// 树边
            Tarjan(v);
            low[u] = min(low[u], low[v]);
        }
        else {// 非树边
            if (belong[v] == 0) {// 回边
                low[u] = min(low[u], dfn[v]);
            }
            // 否则, 弃边
        }
    }
    if (dfn[u] == low[u]) {
        sccIdx++;
        while (sta.top() != u) {
            belong[sta.pop()] = sccIdx;
        }
        belong[sta.pop()] = sccIdx;
    }
}

void init() {
    ei = dfnIdx = sccIdx = 0;
    for (int i = 0;i <= n;i++) {
        dfn[i] = 0;
        low[i] = 0;
        head[i] = 0;
        belong[i] = 0;
    }
}

void build() {
    for (int i = 1;i <= n;i++) {
        if (dfn[i] == 0) {
            Tarjan(i);
        }
    }
}
