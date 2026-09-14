#include<iostream>
using namespace std;
const int MAXN = 2e5;

//并查集
int father[MAXN + 1];
int siz[MAXN + 1];
int road[MAXN + 1];//辅助实现路径优化

//初始化
void initial(int m) {
    for (int i = 1;i <= m;i++) {
        father[i] = i;
        siz[i] = 1;
    }
}

//查找根节点
int find(int u) {
    //迭代+扁平化初始
    //即有递归的路径优化,又避免递归过深
    int cnt = -1;
    while (father[u] != u) {
        road[++cnt] = u;
        u = father[u];
    }
    //路径优化
    while (cnt >= 0)father[road[cnt--]] = u;
    return u;
}

//按秩合并,避免退化
void merge(int u, int v) {
    int ru = find(u);
    int rv = find(v);
    if (ru == rv)return;
    if (siz[ru] >= siz[rv]) {
        siz[ru] += siz[rv];
        father[rv] = ru;
    }
    else {
        siz[rv] += siz[ru];
        father[ru] = rv;
    }
}
