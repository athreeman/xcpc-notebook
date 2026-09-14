#include<bits/stdc++.h>
using namespace std;

//01bfs处理边权(耗费)全是0或1的图
//求原点到任意点的最短距离
//bfs+双端队列求解

//流程(核心)：入队,遍历儿子
//若dist[son]>dist[father]+w,更新
//dist[son]=dist[father]+w
//w==0,入队头
//w==1,入队尾

class Tmp {

private:

    static const int INF = 1e9;

    int mat[501][501];//二维矩阵
    int dist[501][501];//最短路径

    pair<int, int>dir[4] = { {1,0},{-1,0},{0,1},{0,-1} };

    void bfs(int rx, int ry, int n, int m) {
        dist[rx][ry] = 0;//起点路径=0
        deque<array<int, 2>>q;
        q.push_front({ rx,ry });
        while (q.size()) {
            //从队头弹出
            auto [x, y] = q.front();
            q.pop_front();

            for (auto [di, dj] : dir) {
                int nx = x + di, ny = y + dj;
                if (nx<1 || nx>n || ny<1 || ny>m) {
                    continue;
                }
                //根据题目定义获取边权
                int w = mat[x][y] == mat[nx][ny] ? 0 : 1;
                if (dist[nx][ny] > dist[x][y] + w) {
                    dist[nx][ny] = dist[x][y] + w;

                    //核心代码：0入队头，1入队尾
                    if (w == 0) {
                        q.push_front({ nx,ny });
                    }
                    else {
                        q.push_back({ nx,ny });
                    }
                }
            }


        }
    }

    void init(int n, int m) {
        //初始化....略......
    }

public:
    //模板例题：https://www.luogu.com.cn/problem/P4554
    void solve() {
        //输入,略......
    }

};