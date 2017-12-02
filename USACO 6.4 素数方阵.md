# USACO 6.4 素数方阵(剪枝/枚举顺序)



## 素数方阵

[译题](http://www.nocow.cn/index.php/Translate:USACO/prime3)

构造5*5的方阵使得横着竖着左到右斜着看都是五位素数

给出`sum`和左上角元素,求所有方阵



- 明显剪枝,写出DFS枚举后我就开始思考,因为第一个元素确定,打表的时候就可以对外`10000n*10000(n+1)`之间的所有素数都存放下来,存在一个vector里面

- 然后枚举第一行和第一列,都很大,于是观察第一个元素不能为0,所以打表时候排除第一个元素为0,并且保证和是sum,这样第一组数据的`vector`大小不到20,用$O(n^2)$优化了$O(n^9)$的一个复杂度

- 但是这样挂在第三组数据,显然从左到右下的斜的也可以这样优化**但是这里没注意这时候可以取0了DEBUG一个小时+**,然后就开始$O(n^3)$的枚举这三列.因为有输出顺序要求,存在一个vector里面sort.

  - 卡在这里.一方面是莫名vector会加进一个最后一位是0的元素,怎么debug都失效

  - 另一方面,从print那里逃过这个问题后,爆空间.

  - 总结这么长时间的debug心得

    1. 浪费时间的方法

       改自己的代码风格

    2. 垃圾debug方法

       设置断点调试

    3. 更垃圾的debug方法

       找不到断点的时候,用if枚举看在什么时候找不到...

    4. 相对正确的方法

       给程序补充注释

    5. 卓越的方法

       重新写一遍,写到一半没准就会做了

    6. 很重要的经验

       - 保存每个版本的代码,,特别是对于这种不会帮保存代码的OJ
       - 太累的就不要debug了,,浪费时间.

    ```c++
    //一段不愿意继续debug的代码如下...
    const int maxP = 100000;
    int sum,first = 1;
    int maze[6][6];
    bool isprime[maxP];
    struct num{
        int n[6];
    };
    struct MAZE{
        MAZE(int b[6][6])
        {
            for(int i = 1;i <= 5;i++)
            {
                n[i] = 0;
                for(int j = 1;j <= 5;j++)
                    n[i] = n[i] * 10 + b[i][j];
            }
        }
        int n[6];
        bool operator < (const MAZE p)const
        {
            for(int i = 1;i <=5;i++)
                if(n[i] != p.n[i])return n[i] < p.n[i];
            return -1;
        }
        void print()
        {
            for (int i = 1; i <= 5; i++) {
                printf("%d\n",n[i]);
            }
        }
        
    };
    vector<MAZE>ans;

    inline bool check1()
    {
        int s = 0,p = 0;
        for (int i = 1; i <= 5; i++) {
            s += maze[5-i+1][i];
            p = p * 10 + maze[5-i+1][i];
        }
        return s == sum && isprime[p];
    }
    int cleft[6];
    void dfs(int r,int c,int rleft)  //r c 已经被确定
    {
        if (rleft < 0)
            return;
        if (r == c + 1 )//如果下一位被确定
        {
            dfs(r, c+1, rleft);
            return;
        }
        if (r == 4 && c == 2 && check1() == false)
            return;
        if (c == 5 && rleft)
            return;
        
        if(r == 5 && c == 5)//到了最后一位,确认横竖
        {
            int ss = 0,s=0;
            for (int i = 1; i <= 5; i++)
            {
                ss = ss * 10 + maze[i][c];
                s = s * 10 + maze[r][i];
            }
            if (isprime[ss] && isprime[s])
            {
                ans.push_back(MAZE(maze));
            }

        }
        else if (c == 5) //到了最后一列,往下扩展
        {
            if (r == 4 && cleft[5]) return;//如果最后一列有问题
            int ss = 0;
            for (int i = 1; i <= 5; i++)
                ss = ss * 10 + maze[r][i];//判断当期那行素数
            if (isprime[ss]) {
                dfs(r + 1, 1, sum - maze[r+1][1] - maze[r+1][r+1]);
                //减掉第一个数和已经安排好的数
            }
        }
        else if (r == 5)
        {
            maze[r][c+1] = cleft[c+1];   //(其实)下一位已经被确认
            if (maze[r][c+1] <= rleft) { //(下一位)满足条件
                int ss = 0;
                for (int i = 1; i <= 5; i++) {
                    ss = ss * 10 + maze[i][c+1];
                }
                if (isprime[ss]) {       //cleft是统计,这个判断素数
                    dfs(r, c+1, rleft - maze[r][c+1]);
                }
            }
        }
        else
        {
            int ms = min(rleft, cleft[c+1]);//不是最后一行
            for (int i = 0 ; i <= ms; i++) {
                maze[r][c+1] = i;
                cleft[c+1] -= i;
                dfs(r, c+1, rleft - maze[r][c+1]);
                cleft[c+1] += i;
            }
        }
    }//choose right for next floor
    vector<num> spfa,spfa1;
    inline void ensure(int n)
    {
        int temp = n,ss = 0;
        while (temp) {
            ss += temp % 10;
            temp /= 10;
        }
        if (ss != sum) return;
        temp = n;
        num b;
        for(int i = 5;i ;i--)
        {
            b.n[i] = n % 10;
            n /= 10;
        }
        spfa1.push_back(b);
        while (temp) {
            if (temp % 10 == 0)
                return;
            temp /= 10;
        }
        spfa.push_back(b);
    }
    int main()
    {
        freopen("prime3.in", "r", stdin);
        freopen("prime3.out", "w", stdout);
        scanf("%d %d",&sum,&maze[1][1]);
        fill(isprime, isprime + maxP, 1);
        int l = maze[1][1] * 10000,r = (maze[1][1]+1) * 10000;
        for (int i = 2; i < maxP; i++)
            if (isprime[i])
            {
                for (int j = i + i; j < maxP; j += i)
                    isprime[j] = 0;
                if (i > l && i < r) {
                    ensure(i);
                }
            }
        for (int fr = 0; fr < spfa.size(); fr++) {
            for (int fc = 0; fc < spfa.size(); fc++) {
                for (int dig = 0; dig < spfa1.size(); dig++) {
                    bool neg = false;
                    for (int i = 2; i <= 5; i++) {
                        maze[1][i] = spfa[fr].n[i];
                        maze[i][1] = spfa[fc].n[i];
                        maze[i][i] = spfa1[dig].n[i];
                        cleft[i] = sum - spfa[fr].n[i] - spfa1[dig].n[i];
                        if(cleft[i] < 0)
                        {neg = true;break;}
                    }
                    if (!neg)
                        dfs(2, 2, sum - maze[2][1] - maze[2][2]);
                }
            }
        }
        if (ans.size() == 0) {
            printf("NONE\n");
        }
        else
        {
            sort(ans.begin(), ans.end());
            ans[0].print();
            for(int i = 1;i < ans.size();i++)
            {
                printf("\n");
                ans[i].print();
            }
        }
        return 0;
    };
    ```

- 正确思路

  - 调整枚举顺序,因为一个展开式最后的大小,是由于一层一层check来的
  - 于是如果优先选择**能够缩小其它可行解范围**的顺序,比如优先搜**最右下角元素**因为他影响了八个格子


- [参考代码](http://www.nocow.cn/index.php/Code:USACO/prime3/C)对就直接翻下去..特别带感XDD