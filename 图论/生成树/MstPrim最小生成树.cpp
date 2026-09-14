#include<bits/stdc++.h>
using namespace std;

//最小生成树,Prim算法
//模板例题：https://www.luogu.com.cn/problem/P3366


//Prim算法，反向索引堆优化
//时间复杂度,点n,边m
//优化实现需要手写堆Heap
//O(n+m)+O((n+m)*logn)
class PrimBetter {
private:
    using ll = long long;

    static const int MAXN = 5e3;
    static const int MAXM = 2e5;

    int head[MAXN + 1];
    int nxt[2 * MAXM + 1];
    int to[2 * MAXM + 1];
    ll w[2 * MAXM + 1];
    int idx = 0;

    bool vis[MAXN + 1];

    void addEdge(int u, int v, ll z) {
        nxt[++idx] = head[u];
        head[u] = idx;
        to[idx] = v;
        w[idx] = z;
    }

    void init(int n) {
        fill(head, head + n + 1, 0);
        fill(vis, vis + n + 1, false);
        fill(f, f + n + 1, -1);
        siz = 0;
    }

    int f[MAXN + 1];//堆的反向索引表,点i在堆中的位置
    pair<ll, int> heap[MAXN + 1];//堆:{值,点}
    int siz = 0;//堆的大小

    int heapIdx;
    ll heapVal;

    //上浮
    void siftUp(int i) {
        while (i > 0 && heap[i].first < heap[(i - 1) >> 1].first) {
            swap(f[heap[i].second], f[heap[(i - 1) >> 1].second]);
            swap(heap[i], heap[(i - 1) >> 1]);
            i = (i - 1) >> 1;
        }
    }

    //下沉
    void siftDown(int i) {
        int l = 2 * i + 1, r = l + 1;
        while (l < siz) {
            int best = r < siz && heap[r].first < heap[l].first ? r : l;
            if (heap[best].first >= heap[i].first) {
                return;
            }
            swap(f[heap[best].second], f[heap[i].second]);
            swap(heap[i], heap[best]);
            i = best;
            l = 2 * i + 1;
            r = l + 1;
        }
    }

    //插入新值
    void HeapInsert(ll val, int idx) {
        if (f[idx] != -1) {
            //已在堆内,执行更新操作
            update(idx, val);
            return;
        }
        int cur = ++siz - 1;
        heap[cur].first = val;
        heap[cur].second = idx;
        f[idx] = cur;
        siftUp(cur);
    }

    //更新值
    void update(int idx, ll val) {
        int i = f[idx];//获取该点在堆内的位置
        if (val < heap[i].first) {
            heap[i].first = val;
            siftUp(i);
        }
    }

    //弹出堆顶
    void pop() {
        if (siz == 0)heapIdx = -1;
        swap(f[heap[0].second], f[heap[--siz].second]);
        swap(heap[0], heap[siz]);
        heapIdx = heap[siz].second;
        heapVal = heap[siz].first;
        f[heap[siz].second] = -1;//删除
        siftDown(0);
    }

    ll Prim(int n) {
        int cnt = 1;
        vis[1] = true;
        ll ans = 0;
        for (int edge = head[1];edge > 0;edge = nxt[edge]) {
            int son = to[edge];
            ll val = w[edge];
            HeapInsert(val, son);
        }
        while (siz > 0) {
            pop();
            if (vis[heapIdx]) {
                continue;
            }
            ans += heapVal;
            vis[heapIdx] = true;//标记弹出过,即在树上
            cnt++;
            for (int edge = head[heapIdx];edge > 0;edge = nxt[edge]) {
                int son = to[edge];
                ll val = w[edge];
                if (vis[son]) {
                    //弹出过,即在树上
                    continue;
                }
                HeapInsert(val, son);
            }
        }

        if (cnt != n)return -1;
        return ans;
    }

public:

    void solve() {
        //略......
    }
};

class PrimNormal {
    //未优化,一般时间复杂度
    //点n,边m,O(n+m)+O(mlogm)
private:

    using ll = long long;

    static const int MAXN = 5e3;
    static const int MAXM = 2e5;

    int head[MAXN + 1];
    int nxt[2 * MAXM + 1];
    int to[2 * MAXM + 1];
    ll w[2 * MAXM + 1];
    int idx = 0;

    bool vis[MAXN + 1];

    void addEdge(int u, int v, ll z) {
        nxt[++idx] = head[u];
        head[u] = idx;
        to[idx] = v;
        w[idx] = z;
    }

    void init(int n) {
        fill(head, head + n + 1, 0);
        fill(vis, vis + n + 1, false);
    }

    ll Prim(int n) {
        int cnt = 1;
        ll ans = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>>q;
        vis[1] = true;
        for (int edge = head[1];edge > 0;edge = nxt[edge]) {
            int son = to[edge];
            ll val = w[edge];
            q.push({ val,son });
        }
        while (q.size()) {
            auto [val, cur] = q.top();
            q.pop();
            if (!vis[cur]) {
                ans += val;
                cnt++;
                vis[cur] = true;
                for (int edge = head[cur];edge > 0;edge = nxt[edge]) {
                    int son = to[edge];
                    ll val = w[edge];
                    q.push({ val,son });
                }
            }
        }
        if (cnt < n)ans = -1;
        return ans;
    }

public:
    void solve() {
        //略......
    }
};
