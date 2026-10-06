#include<bits/stdc++.h>
namespace name {
    using std::queue;
    using std::sort;
    using std::max;
    using std::min;
    using std::fill;
    using std::cin, std::cout;
    using std::vector;
    using std::greater;
    using std::priority_queue;
};

using namespace name;

//拓扑排序可以用于环的查询
//拓扑排序操作完成后
//剩余的度数>0的点,就是环上点
//核心代码,bfs时只有入度=0时才入队

class Tmp {
private:
    //纯粹的拓朴排序模板代码
    static const int MAXN = 2e5;

    //链式前向星建图
    int idx = 0;
    int head[MAXN + 1];
    int next[2 * MAXN + 1];
    int to[2 * MAXN + 1];
    int indegree[MAXN + 1];//入度
    int rank[MAXN + 1];
    int cnt = 0;

    void add_edge(int u, int v) {
        next[++idx] = head[u];
        head[u] = idx;
        to[idx] = v;
        indegree[v]++;
    }

    bool TopoSort(int n) {
        //拓扑排序
        //入度删除法,入度为0入队,依次删除影响

        //如若需要输出字典序最小的排序
        //把队列更改为小根堆即可
        //priority_queue<int,vector<int>,greater<int>>q;
        queue<int>q;
        for (int i = 1;i <= n;i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }

        while (q.size()) {
            int cur = q.front();//q.top()
            q.pop();
            rank[++cnt] = cur;
            for (int edge = head[cur];edge > 0;edge = next[edge]) {
                int son = to[edge];
                //核心代码,当入度=0时入队
                if (--indegree[son] == 0) {
                    q.push(son);
                }
            }
        }

        return cnt == n;
    }

    void PrintTopo() {
        for (int i = 1;i <= cnt;i++) {
            cout << rank[i] << ' ';
        }
        cout << '\n';
    }

    void init(int n) {
        fill(head, head + n + 1, 0);
        cnt = idx = 0;
        fill(indegree, indegree + n + 1, 0);
    }

public:

    void solve() {
        int n, m;
        cin >> n >> m;
        for (int i = 1;i <= m;i++) {
            int u, v;
            cin >> u >> v;
            add_edge(u, v);
        }
        if (!TopoSort(n)) {//排序失败
            cout << -1 << '\n';
            return;
        }
        PrintTopo();
    }

};