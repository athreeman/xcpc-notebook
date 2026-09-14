#include<bits/stdc++.h>
using namespace std;

//Djt算法解决边权非负图的单源最短路径问题
//有向/无向图,有/无环,边权非负
//以上类型的图都可以解决

//反向索引堆优化
//时间复杂度：点n,边m
//O((n+m)logn)
//未优化时间复杂度:O((n+m)logm)

//Djk和Bfs的差别
//Djk在弹出时标记
//普通bfs在入队时标记
//原因在于bfs的所有边权相同,逐层遍历更早抵达的一定更优
//而复杂的图需要用Djk在弹出时标记,获取最优更新

class Tmp {

private:

    // 核心代码如下：

    // 此处使用了反向索引堆优化
    // 普通Djk,维护小根堆
    // 每次需要判断未出过队且dist[v]大于dist[u]+weight
    // 若满足,则更新dist[v]并入队
    // 出队时标记
    void Djk(int s) {
        HeapInsert(0, s);
        dist[s] = 0;
        vis[s] = true;
        while (siz > 0) {
            pop();
            //弹出,意味着找到该点的最优解
            dist[heapPoint] = heapVal;
            if(vis[heapPoint])continue;
            vis[heapPoint] = true;
            for (int edge = head[heapPoint];edge > 0;edge = nxt[edge]) {
                int son = to[edge];
                ll val = weight[edge];
                //已弹出不受理
                if (vis[son]) {
                    continue;
                }
                //HeapInsert函数自带增添、更新选择
                HeapInsert(heapVal + val, son);
            }
        }
    }

    using ll = long long;
    static const int MAXN = 1e5;
    static const int MAXM = 2e5;
    static const ll INF = 1e18;

    int head[MAXN + 1];
    int nxt[MAXM + 1];
    int to[MAXM + 1];
    int idx = 0;
    ll weight[MAXM + 1];

    void addEdge(int u, int v, ll w) {
        nxt[++idx] = head[u];
        head[u] = idx;
        to[idx] = v;
        weight[idx] = w;
    }

    int heapPoint;
    ll heapVal;
    int siz = 0;
    int f[MAXN + 1];//点->堆中位置
    struct {
        ll val;
        int point;
    }heap[MAXN + 1];

    void HeapSwap(int i, int j) {
        swap(f[heap[i].point], f[heap[j].point]);
        swap(heap[i], heap[j]);
    }

    void siftUp(int i) {
        while (i > 0 && heap[i].val < heap[(i - 1) >> 1].val) {
            HeapSwap(i, (i - 1) >> 1);
            i = (i - 1) >> 1;
        }
    }

    void siftDown(int i) {
        int l = 2 * i + 1, r = l + 1;
        while (l < siz) {
            int best = r < siz && heap[r].val < heap[l].val ? r : l;
            if (heap[best].val >= heap[i].val) {
                return;
            }
            HeapSwap(i, best);
            i = best;
            l = 2 * i + 1, r = l + 1;
        }
    }

    void update(ll val, int point) {
        int idx = f[point];
        //更优,更新
        if (heap[idx].val > val) {
            heap[idx].val = val;
            siftUp(idx);
        }
    }

    void HeapInsert(ll val, int point) {
        if (f[point] != -1) {
            //已插入,更新
            update(val, point);
            return;
        }
        int cur = ++siz - 1;
        heap[cur].point = point;
        heap[cur].val = val;
        f[point] = cur;
        siftUp(cur);
    }

    void pop() {
        if (siz < 1) {
            heapPoint = -1;
            return;
        }
        HeapSwap(0, --siz);
        heapVal = heap[siz].val;
        heapPoint = heap[siz].point;
        f[heapPoint] = -1;//删除映射
        siftDown(0);
    }

    ll dist[MAXN + 1];
    bool vis[MAXN + 1];

    void init(int n) {
        idx = siz = 0;
        for (int i = 1;i <= n;i++) {
            dist[i] = INF;
            head[i] = 0;
            f[i] = -1;
            vis[i] = false;
        }
    }


public:
    //模板例题：https://www.luogu.com.cn/problem/P4779
    void solve() {
        //输入,略......
    }
};
