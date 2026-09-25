#include<bits/stdc++.h>

//前缀树(字典树),根据前缀字符建树
//根据前缀信息查询
//时间复杂度低,空间复杂度略高
//模板例题：https://www.nowcoder.com/practice/7f8a8553ddbf4eaab749ec988726702b

using std::string;
using std::fill;

const int MAXN = 1e6;
int tree[MAXN + 1][26];//'a'~'z',0~25
int pass[MAXN + 1];//多少个字符串经过该点
int end[MAXN + 1];//多少个字符串以该点结尾
int cnt = 1;//节点编号,申请和撤销空间

void build() {
    cnt = 1;//1作为头节点
}

void insert(string word) {
    int cur = 1;//cur初始化1,头节点
    pass[cur]++;
    for (int i = 0;i < word.size();i++) {
        int path = word[i] - 'a';
        if (tree[cur][path] == 0) {
            tree[cur][path] = ++cnt;
        }
        cur = tree[cur][path];
        pass[cur]++;
    }
    end[cur]++;
}

int search(string word) {
    int cur = 1;
    for (int i = 0;i < word.size();i++) {
        int path = word[i] - 'a';
        if (tree[cur][path] == 0) {
            return 0;
        }
        cur = tree[cur][path];
    }
    return end[cur];
}

int countPrefix(string word) {
    int cur = 1;
    for (int i = 0;i < word.size();i++) {
        int path = word[i] - 'a';
        if (tree[cur][path] == 0) {
            return 0;
        }
        cur = tree[cur][path];
    }
    return pass[cur];
}

void erase(string word) {
    if (search(word) < 1)return;
    int cur = 1;
    //沿途pass--
    //终点end--
    for (int i = 0;i < word.size();i++) {
        int path = word[i] - 'a';
        if (--pass[tree[cur][path]] == 0) {
            //全部删除完毕,清除节点编号
            tree[cur][path] = 0;
            return;
        }
        cur = tree[cur][path];
    }
    end[cur]--;
}

void clear() {
    for (int i = 1;i <= cnt;i++) {
        fill(tree[i], tree[i] + 26, 0);
        pass[i] = end[i] = 0;
    }
}
