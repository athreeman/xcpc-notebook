#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 给定n个石子
// 规定p是质数, 每次操作可以拿p的自然数次幂个石子
// 拿走最后一个石子的人获胜

// 结论: 对于6的整数倍, 6 = 2*3
// 因此, 6的整数倍绝对无法倍任何p的自然数次幂直接拿完
// 因此, 谁最开始面对6的整数倍, 谁输

string BashGame(int n) {
    return n % 6 != 0 ? "先手" : "后手";
}