# USACO5.5 降维扫描/离散_最小表示法_记忆化搜索

安利一个chrome的CSDN广告拦截[官网地址](https://adblockplus.org/)

先谈感(chui)想(shui)吧:

今天做最后一题有种上个学期做"别人口中的水题"的感觉,一拿到题没有思路,想了一会思路不对.

早上六点开题去晨跑,八点半开始看题解,晚上八点终于磕磕碰碰的完成了这一次的代码,过了OJ的测试

几度想要放下USACO,先去刷专题,先去强化自己的能力.去知乎搜索USACO也都是大家的嘲讽语气

> 不要浪费了这些好题

思考了一天人生+这道题后还是把它做了下来,

看题解不懂多看一遍

看懂题解不会写看代码

看一篇不会写多看几篇

看懂一段代码不会仿照多看几段

勤或能补拙吧.还是觉得应该在期末考试前努力往前把USACO刷掉(尽量往后刷),上个学期给自己最大的教训是,每每碰到困难,碰到无助的时候就去网上去找"更好的学习方法",然而这样简而言之就是在逃避.于是上学期看看题解磕磕碰碰过了百题关后还是在期末考试扑街.

然而紧接着而来的4+2选拔赛不允许自己再去逃避了.看往年4+2的题解也都是这种一拿到题就没有思路的,看完题解"收货满满的"这种难度高于自己水平的题目.如果现在放弃USACO,有三种可能选择

1. 去练专题,比如DP,贪心或者几何
   - 但是这些应该都是暑假的任务,USACO上面还有很多类型没有刷到,比如误闯入别人题解看到的概率"退火"算法等等.
2. 专心复习期末考试
   - 这个点无法辩驳,写完这篇题解赶紧去写高数
3. 水别的OJ
   - 上文已经辩驳

所以综上,还是要脚踏实地,努力向前,好好准备4+2~

## picture  降维扫描/离散_线段树

- 题意: 给出一堆矩形对角线的点(-10000~10000),求矩形覆盖后外围图形的周长



- 思路一(MLE) : 容斥..挂在第八个数据还是挺尴尬的..
  1. 两个矩形重复的话判断是这样重复的,然后减去重复的那块(也一定是矩形)
  2. 但是重复的那块可能减去了重复的,所以继续递归作加
  3. MLE在第八个之后发现需要几个G的内存...估计是炸了...然后消除了完全包含的之后TLE在512k


- 思路二: (计数): 

  1. 读入每个矩形后存下四条边的位置,并记左,下为始边,右上为终边

  2. 依据pos,始边,fr的优先级对于两个结构体排序(存有平行于x的结构体和存有平行于y的结构体)

  3. 开始一个mark数组,记录纵向

  4. 从左往右(从下往上扫),

     - 如果是始边`mark[i]++`,如果`mark[i] == 1`说明是边界始边,ans++
     - 如果是终边`mark[i]--`,如果`mark[i] == )`说明是边界终边,ans++

  5. 时间真的很漂亮...

     ```c++
     Test 7: TEST OK [0.011 secs, 4572 KB]
     Test 8: TEST OK [0.000 secs, 4572 KB]
     Test 9: TEST OK [0.011 secs, 4572 KB]
     Test 10: TEST OK [0.000 secs, 4572 KB]
     Test 11: TEST OK [0.065 secs, 4572 KB]
     ```

- 离散化图结构+线段树优化对于扫描过程的`mark++`,`mark--`

  - 这道题的离散就是用sort从左到右的意思

    ```c++
    int f[88888];
    bool g[88888]; // 表示该结点的子结点权值是否全为0
    for (i = 0; i < n; ++i) {
    		y1 = a[i].y1, y2 = a[i].y2;
    		a[i].in ? insert(1, -10000, 10000) : erase(1, -10000, 10000);
    }
    void insert(const long t, const int l, const int r) {
    	if (l >= y1 && r <= y2 && !g[t]) {
    		if (!f[t]++) ans += r - l;
    		return ;
    	}
     
    	long mid = (l + r) / 2, t1 = t << 1, t2 = t1 | 1;
    	f[t1] += f[t], f[t2] += f[t], f[t] = 0; // 子结点权值不全为0，要继续往下递归，当前结点的权值要传递到两子结点
    	if (mid >= y2) insert(t1, l, mid);
    	else if (mid <= y1) insert(t2, mid, r);
    	else insert(t1, l, mid), insert(t2, mid, r);
    	g[t] = f[t1] || f[t2] || g[t1] || g[t2];
    }
     
    void erase(const long t, const int l, const int r) {
    	if (l >= y1 && r <= y2 && !g[t]) {
    		if (!--f[t]) ans += r - l;
    		return ;
    	}
    	long mid = (l + r) / 2, t1 = t << 1, t2 = t1 | 1;
    	f[t1] += f[t], f[t2] += f[t], f[t] = 0;
    	if (mid >= y2) erase(t1, l, mid);
    	else if (mid <= y1) erase(t2, mid, r);
    	else erase(t1, l, mid), erase(t2, mid, r);
    	g[t] = f[t1] || f[t2] || g[t1] || g[t2];
    }
     
    ```

    ​

## hidden 最小表示法

模板题..红书P216

记得读入可能换行..

```c++
int pos = 0;
    while (pos < N ) {
        scanf("%s",str + pos);
        pos = (int)strlen(str);
    }
```



> 用i和j表示最优位置和待比较的起始位置
>
> k = 最小变化位置(s[i+k] != s[j+k]),k>=N说明已经最优
>
> s[j+k]>s[i+k] =>j 后移k+1位
>
> s[j+k]\<s[i+k] =>j = max(j , i + k)+1 并更新 i = j

复杂度是O(n)

周源的论文里面写的是两个串的同构比较..



## twofive

'A'~'Y'二十五个字母排列成5*5 的矩阵

求字典序第N大的排列

或者某个排列的字典序



- 一开始打了张表找规律..
- 然后模拟



- 恩然后看题解,是一个构造DP
- `cnt[a,b,c,d,e]`表示每一行分别为`a,b,c,d,e`个数的时候的值
- 使得`5>=a>=b>=c>=d>=e>=0`这样就可以从第一个字母一直往后填到最后一个字母
- 然后对于每个前缀记忆化搜索



代码

```c++
/*
 ID: leidar11
 PROG: twofive
 LANG: C++11
 */
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

int cnt[6][6][6][6][6];//记录每一行放了几个数的dp数组,注意这里用结构体会莫名MLE
int mx[6],my[6],line[6];//记录每一行列当前最大值,以及每一列放了多少个数,便于DP
int used[30];//记录哪些数被用掉了

int dp(int a,int b,int c,int d,int e,int val)
{
    int &now = cnt[a][b][c][d][e];
    if (now)
    {
        return now;
    }//记忆化搜索
  /*
  一开始看题解看到记忆化搜索,高高兴兴去打了张表
  但是没有意识到每次记忆化搜索是动态的
  也就是对于不同前缀尝试下的不同记忆化数组.
  (BTW 这是不是应该叫树形DP)
  */
    if (used[val]) {
        return now = dp(a,b,c,d,e,val+1);
    }//如果当前字母已经用掉,往下
    if (val == 25)
    {
        return 1;
    }//搜索到最后一个,返回1
    if (a < 5 && val > mx[0] && val > my[a])
    {
        now += dp(a+1,b,c,d,e,val+1);
    }//满足在第一行继续放字母的条件
    if (b < a && val > mx[1] && val > my[b])
    {
        now += dp(a,b+1,c,d,e,val+1);
    }//满足在第二行继续放字母的条件
    if (c < b && val > mx[2] && val > my[c])
    {
        now += dp(a,b,c+1,d,e,val+1);
    }//满足在第三行继续放字母的条件
    if (d < c && val > mx[3] && val > my[d])
    {
        now += dp(a,b,c,d+1,e,val+1);
    }//满足在第四行继续放字母的条件
    if (e < d && val > mx[4] && val > my[e])
    {
        now += dp(a,b,c,d,e+1,val+1);
    }//满足在第五行继续放字母的条件
    return now;
}


void laypos(int pos,int value)
{
    memset(cnt,0,sizeof(cnt));//初始化DP数组
    cnt[5][5][5][5][5] = 1;//结束条件
    int x = pos % 5;
    int y = pos / 5;
    mx[x] = my [y] = value;
    used[value] = 1;
}

int main()
{
    freopen("twofive.in","r",stdin);
    freopen("twofive.out","w",stdout);
    
    char type;
    string str;
    int id;
    
    cin >> type;
    if (type == 'W')//转换成数字
    {
        cin >> str;
        id = 0;
        memset(mx,-1,sizeof(mx));
        memset(my,-1,sizeof(my));
        memset(used,0,sizeof(used));
        memset(line,0,sizeof(line));
        
        for (int i = 0; i < 25; ++i)
        {
            line[i%5]++;//记录当前列数目
            int cur = 0,temp;
            for (temp = 0; temp < str[i] - 'A'; ++temp)
            {
                if (used[temp] == false && temp > mx[i%5] && temp > my[i/5])
                {
                    laypos(i,temp);
                    cur += dp(line[0],line[1],line[2],line[3],line[4],0);
                    used[temp] = false;
                }
            }//尝试每一个字母,从'A'到当前位置字母-1(后面计算当前字母的值)
            id += cur;
            used[temp] = 1;
            mx[i%5] = my[i/5] = temp;
        }
        cout << id + 1 << endl;//注意这里要+1,因为一开始不会计算最后一个字母的值
    }
    else
    {
        cin >> id;
        str = "";
        memset(mx,-1,sizeof(mx));
        memset(my,-1,sizeof(my));
        memset(used,0,sizeof(used));
        memset(line,0,sizeof(line));
        int cur = 0;//注意这个是放在外面,记录已经累计的数目
        for (int i = 0; i < 25; ++i)
        {
            line[i%5]++;
            int temp ;
            for(temp = 0;temp < 25;temp ++)
            {
                if (used[temp] == false && temp > mx[i%5] && temp > my[i/5])
                {
                    laypos(i,temp);
                    int ans = dp(line[0],line[1],line[2],line[3],line[4],0);
                    if (cur + ans >= id)
                    {
                        break;
                    }//如果累计数目超过或等于id,说明当前字母就是原字符串
                    cur += ans;//否则累计数目增加
                    used[temp] = false;
                }
            }
            str += char('A' + temp);
            mx[i%5] = my[i/5] = temp;
        }
        cout << str << endl;
    }
    return 0;
}
```

