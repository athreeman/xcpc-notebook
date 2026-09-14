#include<bits/stdc++.h>
using namespace std;

//分层图最短路(状态空间图)
//用于图中点的状态不明确,因路径选择不同存在不同的实际状态
//把点抽象为{坐标，状态}的形式
//具体因题而异
//例如：{坐标，位信息}视为一个点
//基于此进行Djk算法

class Tmp {

private:

    static const int MAXN = 1e4;
    static const int MAXM = 5e4;
    static const int INF = 1e9;

    //建图
    int head[MAXN + 1] = { 0 };
    int nxt[2 * MAXM + 1] = { 0 };
    int to[2 * MAXM + 1] = { 0 };
    int idx = 0;
    int weight[2 * MAXM + 1];

    void addEdge(int u, int v, int w) {
        nxt[++idx] = head[u];
        head[u] = idx;
        to[idx] = v;
        weight[idx] = w;
    }

    //手动堆,反向索引堆优化
    int heapCnt;
    int heapPoint;
    int heapVal;
    int siz = 0;
    int f[MAXN + 1][11];//{点,状态}->堆中位置
    struct {
        int val;//耗费
        int cnt;//免费次数使用状态
        int point;//点
    }heap[MAXN + 1];

    void HeapSwap(int i, int j) {
        //交换映射->交换数据
        swap(f[heap[i].point][heap[i].cnt], f[heap[j].point][heap[j].cnt]);
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

    void update(int val, int point, int cnt) {
        int idx = f[point][cnt];
        //更优,更新
        if (heap[idx].val > val) {
            heap[idx].val = val;
            siftUp(idx);
        }
    }

    void HeapInsert(int val, int point, int cnt) {
        if (f[point][cnt] != -1) {
            //已插入,更新
            update(val, point, cnt);
            return;
        }
        int cur = ++siz - 1;
        heap[cur].point = point;
        heap[cur].val = val;
        heap[cur].cnt = cnt;
        f[point][cnt] = cur;
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
        heapCnt = heap[siz].cnt;
        f[heapPoint][heapCnt] = -1;//删除映射
        siftDown(0);
    }

    bool vis[MAXN + 1][11] = { false };

    int Djk(int st, int ed, int k) {
        vis[st][0] = true;
        HeapInsert(0, st, 0);
        while (siz > 0) {
            pop();
            vis[heapPoint][heapCnt] = true;
            if (heapPoint == ed) {
                return heapVal;
            }
            for (int edge = head[heapPoint];edge > 0;edge = nxt[edge]) {
                int son = to[edge];
                int cost = weight[edge];
                if (vis[son][heapCnt])continue;
                //不使用免费次数
                //HeapInsert函数自带增添、更新、不做修改的检查
                HeapInsert(cost + heapVal, son, heapCnt);
                if (heapCnt < k) {
                    if (vis[son][heapCnt + 1])continue;
                    HeapInsert(heapVal, son, heapCnt + 1);
                }
            }
        }
    }

public:

    //模板例题：https://www.luogu.com.cn/problem/P4568
    void solve() {
    //略...... 
    }
};