#include<bits/stdc++.h>
using namespace std;

//BellmanFord和SPFA
//单源最短路算法
//队列优化
//出队->松弛操作->入队(不在队内)->出队
//要求不能有负环

//松弛操作(核心)：
//通过边和点去检查是否更新更小的dist
//dist[son]=min(dist[father]+wight)

const int MAXN = 2e3;
const int MAXM = 3e3;
const int INF = 1e9;

int idx = 0;
int head[MAXN + 1];
int nxt[2 * MAXM + 1];
int to[2 * MAXM + 1];
int weight[2 * MAXM + 1];

void addEdge(int u, int v, int w) {
    nxt[++idx] = head[u];
    head[u] = idx;
    to[idx] = v;
    weight[idx] = w;
}

int roundCnt[MAXN + 1];
int dist[MAXN + 1];
bool vis[MAXN + 1];
//start是源头
int SPFA(int start, int n) {
    queue<int>q;
    q.push(start);
    vis[start] = true;
    roundCnt[start] = 1;//轮次
    dist[start] = 0;
    while (q.size()) {
        int cur = q.front();
        vis[cur] = false;
        q.pop();

        for (int edge = head[cur];edge > 0;edge = nxt[edge]) {
            int son = to[edge];
            int w = weight[edge];
            if (dist[cur] + w < dist[son]) {
                dist[son] = dist[cur] + w;
                if (!vis[son]) {

                    //只有当不在队列里时,视为一轮松弛操作
                    if (++roundCnt[son] >= n) {
                        //判断负环,陷入循环
                        return -1;
                    }
                    vis[son] = true;
                    q.push(son);
                }
            }
        }
    }
    return 1;//成功得到结果
}
