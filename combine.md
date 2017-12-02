# USACO 4.1 更棒的算法





# nuggets

```c++
//这居然过了......
sort(arr, arr+n);
if (n == 1 || arr[0] == 1 || alleven)
{
    fout << "0" << endl;
}//枚举了这三种情况后生成(背包)200000以前的所有,如果mp = 200000-1那么输出0,否则mp...
```

- 结论(之前数论白学了系列QAQ)

  - 有两个数p,q,且gcd(q,p)=1,则最大无法表示成px+qy（x>=0,y>=0)的数是pq-q-p
  - 而且数越多，这个值只会越小。
  - 取上界为256\*256-256*2 = 65024//这也就证明了..上面的ac
  - 判断无限解可以按上面的方法，另外也可以算所有的数的最大公约数。如果不是1，也就是说这些数不互质，那么不被这个最大公约数整除的数一定构造不出来。当且仅当这种情况会有无限解。
  - 优化算法 这道题其实是可以做到O(N^2)。 令A1表示所有鸡块中最小的那种， 如果有连续A1个数都可以打到，那么之后所有的数都是可以达到的（每次加上A1就行了）。

- 标准解法用了尝试过的循环数组

  ```c++
  while (pos < 2000000000)
     { /* bound as stated */

      /* if last 256 were all possible, we are done */
      if (pos - last > 256) break; 

      if (!cando[lv]) 
        last = pos; /* this isn't possible, update last impossible */
      else 
       { /* this is possible */
        cando[lv] = 0; /* mark pos+256 as impossible */

        /* mark pos + size as possible for each package size */
        for (lv2 = 0; lv2 < nsize; lv2++)
          cando[(lv+sizes[lv2])%256] = 1;
       }

      /* update lv & pos */
      lv = (++pos) % 256; 
     }
  ```

  ​

> 另外感觉USACO的机器升级了XD

# fence6

给出n条边长度,每条边两个端点相交边的序号,求最小环

这道题难点有二:

- 序号很麻烦..坑了一晚上

  主要是不是逆时针或者顺时针给的,需要判断交点

  O(n2)扫两遍,用一个变量标志id

  ```c++
  //扫一个数组,
  for (int i = 1; i <= totF; ++i)
  {
          if (F[i].rr == 0)//这样id一定最小
          {
              F[i].rr = ++d;//新的id
            
              for (int p = 1; p <= F[i].r[0]; p++) {
                  int k = F[i].r[p];//更新rr位置
                //确认对应位置,给id
                  int inRR = false;
                  for (int s = 1; s <= F[k].r[0]; s++) {
                      if (F[k].r[s] == i) {
                          inRR = true;
                          break;
                      }
                  }
                  if (inRR) {
                      F[k].rr = d;
                      continue;
                  }
                  F[k].ll = d;
              }
          }
          
          if (F[i].ll == 0)
          {
              //和上面相同
          }
  }
  ```

- 最小环

  有了id之后就可以最小环了

  思路是枚举每一条边`oo`最短路

  FLOYD改进版本可以过

  ```c++
  for (int eg = 1; eg <= totF; eg++) //枚举边
  {
          int fr = F[eg].ll,to = F[eg].rr;
          //初始化距离矩阵,该边为oo
          for (int i = 1; i <= d; i++) {
              for (int j = 1; j <= d; j++) {
                  dis[i][j] = inf;
              }
              //dis[i][i] = 0;
          }
          for (int i = 1; i <= totF; i++) {
              if (i != eg) {
                  int fr = F[i].ll,to = F[i].rr;
                  dis[fr][to] =dis[to][fr] = F[i].len;
              }
          }
    		//floyd
          for (int by = 1;  by <= d; by ++) {
              for (int fr = 1; fr <= d; fr ++) {
                  for (int to = 1; to <= d; to ++) {
                      dis[fr][to] = min(dis[fr][to],dis[fr][by]+dis[by][to]);
                  }
              }
          }
    		//当有另一条路过去的时候,那么就是最小环的一半
          for (int by = 1; by <= d; by++) {
              if(by != fr && by != to){
                  ans = min(ans, F[eg].len+ dis[fr][by] + dis[by][to]);          
              }
          }
      }
  ```

  - 当然,这是单源最短路径,所以可以用SPFA,dijkstra来优化速度
  - 据说搜索+剪枝也能过

  ```c++
  //nocow上没有id化的代码
  int dijkstra(int z){
  	int i,j,k;
  	for(i=1;i<=n;i++){dis[i]=MAX;isi[i]=0;}//初始化
  	for(i=1;i<=map[0][z][0];i++)
      {
        bef[map[0][z][i]]=z;
        dis[map[0][z][i]]=len[z];
      }
  	while(1){
  		j=0;
        	k=MAX;
  		for(i=1;i<=n;i++)
  			if(dis[i]<k && isi[i]==0){
  				j=i;
  				k=dis[i];
  			}//最近d
  		if(j==0)break;//找不到
  		if(j==z)return dis[z];//当前
  		isi[j]=1;//最优标记
  		for(i=1;i<=map[0][j][0];i++)
  			if(map[0][j][i]==bef[j])break;
  		if(i==map[0][j][0]+1)
            k=0;//0方向
  		else 
            k=1;//1方向
  		for(i=1;i<=map[k][j][0];i++){
  			if(dis[map[k][j][i]]>dis[j]+len[j]){
  				dis[map[k][j][i]]=dis[j]+len[j];
  				bef[map[k][j][i]]=j;
  			}
  		}
  	}
  	return MAX;
  }
  ```

  - 其实这道题时间大多数花在纠结是用vector还是set
  - 搜索也写烂了几次,所以其实很多看起来很炫酷的东西其实并不重要,重要的是即使是枚举也先要实现代码.

- 今(昨)天有一个很开心的事是发现xcode怎么输入输出不是绝对路径的文件了23333

# Polya





### POJ2409

主要没有理清翻转的意义,把翻转的次数直接除了去

通过移位形成的置换群的置换数是gcd(N, M)，其中N是置换群的元素个数，M是移位的位数。

翻转也算置换,与论文例题不同,一共有2n种置换

> 先看n为奇数的情况，一共有n种翻转，此时只有一种形式，即经过某个顶点i与中心的连线为轴的翻转：
>
> i->i,i+1->i-1,i+2->i-2,.........(i+(n-1))%n->(i-(n-1)+n)%n
>
> 再看n为偶数的情况，在这里又分为了两种情况。
>
> （1）这种是经过某个顶点i与中心的连线为轴的翻转，由于n为偶数，有对称性，所以此种共n/2种翻转：
>
> i->i,i+1->i-1,i+2->i-2,.........(i+(n-1))%n->(i-(n-1)+n)%n
>
> （2）这种是以顶点i和i+1的连线的中点与中心的连线为轴的翻转，同样，根据对称性，也有n/2种翻转：
>
> i->i+1,i-1->i+2,i-2->i+3,..................(i-(n-1)+n)%n->(i+(n-1)+1)%n

# USACO 4.2 网络流

## ditch

裸的网络流...练习抄模板(红书dinic)

因为bool写成int调试了好久QAQ

- 变量声明

  ```c++
  const int inf = 0x7ffffff;
  const int maxn = 300,maxm = 300;//一般图不会这么小
  struct edge{
      int v,f,nxt;//到 v ,容量为f,下一个点事nxt
  };
  int n,src,sink;//记得初始化源点和结束
  int g[maxn + 10];//邻接表存边 下一条边
  int nume;//边序号
  edge e[maxm * 2 + 10];//边,邻接表存
  ```

- 加边

  ```c++
  void addedge(int u,int v,int c)
  {
      e[++nume].v = v;
      e[nume].f   = c;
      e[nume].nxt = g[u];
      g[u] = nume;
      //反向边
      e[++nume].v = u;
      e[nume].f = 0;
      e[nume].nxt = g[v];
      g[v] = nume;
  }
  ```

- 初始化(加边)

  ```c++
  void init()
  {
      int a;
      int fr, to, c;
      
      nume = 1;
      src = 1;
      
      memset(g, 0, sizeof(g));
      fin >> a >> sink;
      while (a--) {
          fin >> fr >> to >> c;
          addedge(fr, to, c);
      }
  }
  ```

- bfs找增广路径是否存在

  ```c++
  queue<int> que;
  bool vis[maxn + 10];
  int dist[maxn + 10];
  void bfs(){
      memset(vis, 0, sizeof(vis));
      memset(dist, 0, sizeof(dist));
      while (!que.empty())
          que.pop();
      
      vis[src] = true;
      que.push(src);
      while (!que.empty()) {
          int u = que.front();
          que.pop();
          for (int i = g[u]; i; i = e[i].nxt) {
              if (e[i].f && !vis[e[i].v]) {
                  que.push(e[i].v);
                  dist[e[i].v] = dist[u] +1;//通过序号标记确认是下一条边
                  vis[e[i].v] = true;
              }
          }
      }
    //如果vis[sink]为0那么不存在
  }
  ```

- dfs增广路径

  ```c++
  int dfs(int u,int delta)
  {
      if (u == sink) {
          return delta;
      }else{
          int ret = 0;
          for (int i = g[u]; delta && i; i = e[i].nxt) {
              if (e[i].f && dist[e[i].v] == dist[u] +1) {
                  int dd = dfs(e[i].v, min(e[i].f,delta));//增广递归
                 	e[i].f -= dd;
                  e[i^1].f += dd;//反向边
                  delta -= dd;
                  ret += dd;
              }
          }
          return ret;
      }
  }
  ```

- nocow上没用邻接表的实现sap(可以充分利用以前的标号)

  ```c++
  //这里h[n] 充当了距离标号
  int sap(int v,int flow)
  {
  	int rec=0;
  	if (v==n) 
        return flow;			//汇聚点
  	for (int i=0;i++<n;)	//从v到i尝试
      {
  		if (map[v][i] 		//存在增广空间
              && h[v]==h[i]+1)//d[i]=d[j]+1的弧(i,j)叫做允许弧 ，且增广时只走允许弧，那么就可以达到“怎么走都是最短路”的效果
          {
  			int ret=sap(i,
                          min(flow-rec,map[v][i]));//尝试下一层增广
  			map[v][i]-=ret;
  			map[i][v]+=ret;		
  			rec+=ret;		//统计增广量
  			if (rec==flow) 
                return flow;	//满足上一层完全增广,返回
  		}
  	}
    //不满足上一层增广
    /*
   距离函数（distance function）有效的当且仅当满足有效条件（valid function）
   就是某个点到汇点的最少的弧的数量（即边权值为1时某个点到汇点的最短路径长度）
  	
  （1）d（t）= 0；
  （2）d（i）<= d(j)+1(如果弧rij在残余网络G（x）中)；
    
    gap[i]数组表示距离标号为i的点有多少个，
    如果到某一点没有符合距离标号的允许弧，
    那么需要修改距离标号来找到增广路；
    如果重标号使得gap数组中原标号数目变为0，则算法结束
    
    初始化gap[0] = n;
    每次增广gap[当前节点距离]--;
    gap[当前节点+1距离]++;
    
    */
  	if (!(--gap[h[v]]))
        h[1]=n;//结束外层循环标志
  	gap[++h[v]]++;
  	return rec;//返回可以增长量
  }
  ```

  ​

# 匹配

继续抄模板(匈牙利算法)

```c++
vector<int> g[maxn];//与i相关联
int from [maxn],tot;
bool use[MAXN];

bool match(int x)
{
    for (int i =0; i < g[x].size(); i++) {
        if (!use[g[x][i]]) {
            use[g[x][i]] = true;
            if (from[g[x][i]] == -1 
                || match(from[g[x][i]]))//递归就是可以改变匹配而增广
            {
                from[g[x][i]] = x;//可以增广
                return true;
            }
        }
    }
    return false;
}

int hungary()
{
    tot = 0;
    memset(from, 255, sizeof(from));
    for (int i = 1; i <= n; i++) {
        memset(use, 0, sizeof(use));
        if (match(i)) {
            ++tot;
        }//尝试每一个点能否增广.
    }
    return tot;
}
```

当然这么裸的二分图,网络流模型也很好构造.只是代码复杂多了



# 双重机器

贪心贪心贪心.....

最后一次缩小了二分范围,取消了cout输出0s过....cout真是慢慢慢

就是给出N<1000个物品,经过两种机器的加工,两种机器分别有a,b台,它们加工的速度不尽相同,给出每个机器加工时间,求最后完成所有A加工的时间和完成所有B加工的时间





因为觉得A每次选最快加工的,B也同理就好了嘛

一开始写贪心是这样写的(还修正了几次...)

```c++
优先队列,按照FinishTime + cost从小到大排列.
for (int i = 1; i <= N; i++) {
        int time = AM.top().FinishTime; //最优仪器上一次完成时间
        int add  = AM.top().OnceTime;   //这个要求时间(取出来的是和最小的)
        //cout << time << " "<<  add << "\t" << flush;
        AM.pop();
        time += add;                    //完成当前工作
        ansA = max(ansA, time);
        AM.push(machine(time ,add));
        arr[i] = time;
        /*
        while (BM.top().FinishTime < time)
        {
            BM.push(machine(time,BM.top().OnceTime));
            BM.pop();
        }//等待下一个A
         
        int Time = BM.top().FinishTime; //最优仪器上一次完成时间
        int Add  = BM.top().OnceTime;   //这个要求时间(取出来的是和最小的)
        cout << Time << " " << Add << endl;
        Time += Add;                    //完成当前工作
        ansB = max(ansB, Time);
        BM.pop();
        BM.push(machine(Time ,Add));    //进入排序
         这样贪心是不对的,因为5 3 5 8这样的结果会是14QAQ
         */
    }
```

然后看到最后一行的那组数据..彻底崩盘(觉得最优子结构就这样挂了QAQ)

然后看大牛题解..果然贪心从后往前扫是很重要的技巧...

> 看了各位大牛的解释之后，我是坐在马桶上冥思苦想，想到屁股出血，终于差不多想通了： 注意到A产生的工件不一定是需要马上送给B来处理的。只要B可以在最后时限前处理好这些工件就行。 那么我们反演时间，想象B的工作不是组装成品，而是逆时间轴运转，分解成品成为工件，那么cost[A]其实给出了B做拆解工作的时间约束。也就是说，假设B在T时间开始拆，那么**最早拆完的那个工件在时间轴上要比A最晚交付的晚**。依次，就可以得到上面的算法。 = = 太敬仰你们了。。。。 交了代码之后发现usaco题解里面说的和我这里说的一样。。。匿了。。。

就这样二分B的最后时间,贪心扫一遍就可以了

因为cout和inf太大T了一次...



这样思路的好处在于不必担心是否有更优的解,

只需要每次选最后拆完的就可以啦..

数据有一点弱,第一次没用用最晚拆完的居然过了...

```c++
//每次选完成时间能够最大的那个人.
bool check(int END)
{
    for (int i = 1; i <= totB; i++)
    {
        finish[i] = END - brr[i];//能拆完一个包裹的时间
    }
    for (int i = N; i ; --i) {
        int choose = -1,mintime = -inf;
        for (int k = 1; k <= totB; k++) {
            if (finish[k] >= arr[i])//拆完后的时间比当前时间要晚
            {
                if (finish[k] > mintime)
                {
                    mintime = finish[k];
                    choose = k;
                }//选最晚的那一个拆
            }
        }
        if (choose == -1) {
            return false;
        }
        finish[choose] -= brr[choose];
    }
    return true;
}
```

当然神牛的代码简明精要多了

```c++
	for (int k=0;k<2;k++)
    {
    	//由于对称性,反过来也是一样的
        memset(delay,0,sizeof(delay));
        for (int i=0;i<n;i++)
        {
            int min=20001,minj;
            for (int j=0;j<m[k];j++)
            	if (delay[j]+t[k][j]<min)
            	{
                	min=delay[j]+t[k][j];
                	minj=j;
            	}//搜索可行,选最先
            cost[k][i]=min;
            delay[minj]+=t[k][minj];
        }
    }
    int ans=0;
	for (int i=0;i<n;i++)
    	if (cost[0][i] + cost[1][n-i-1] > ans)
    		//对称位置最后的选择
    		ans=cost[0][i]+cost[1][n-i-1];
    fout<<cost[0][n-1]<<" "<<ans<<endl;
```



# USACO 4.3 高精度



## buylow

给出一个数组,求最长下降子序列以及个数



主要是个数..如果用通常的LIS是做不出来的.所以用`vector<pair<long, string> >cnt[maxn];`这样的一个vector来存每一个数所应该贡献的位置.并用M记录最大值(这里忘记用string了...)然后从后往前扫

> /*  cnt[i]是当前为i的长度的所有序偶
> 第一个是它的值    第二个是它的数量
> 每次扫到小于M[i-1]的时候更新cnt[i] 以及M[i] *

更新cnt[i]的时候判断cnt[i]中是否有相同的元素,有的话直接更新存在的元素(所以这里应该用map?)

后面几组数据需要使用高精度.

[nocow](http://www.nocow.cn/index.php/USACO/buylow)上的题解

> 对于第二问求最长下降序列的数量，可以通过求第一问的过程解决。设MaxCnt[i]为第i项为末尾中最长下降序列的个数。
>
> 对于所有的j(1≤j≤i-1)如果有(s[j]>s[i] 并且 MaxLength[j]+1>MaxLength[i])则MaxCnt[i]=MaxCnt[j]，否则如果(MaxLength[j]+1= =MaxLength[i])可利用加法原理，MaxCnt[i]=MaxCnt[i]+MaxCnt[j]。
>
> 考虑到题目中说的不能又重复的序列，我们可以增加一个域Next[i]表示大于i且离i最近的Next[i]使得第Next[i]个数与第i个数相同。如果不存在这样的数则Next[i]=0。这样我们在DP的时候如果出现Next[j]不为0且Next[j]<i可直接跳过。

### 优化技巧

1. 可以在给出的序列的末尾增加一个0，这样直接统计以最后一个0结尾的最长下降子序列即可。

2. 显然题目的规模要求我们用高精度。可以使用longint，8位8位的加以节省时间。

3. ```c++
   //nocow上奇妙的高精度
   /* our bignum structure will store 6 digits per struct */
   /* the numbers will also be stored in INCREASING significance */
   /* thus 86,376,576 is represented as 376576 -> 86 */
   typedef struct BIGNUM *bignum_t;
   struct BIGNUM
    {
     int val;
     bignum_t next;
    };

   int num[5000]; /* the numbers (prices) */
   int len[5000]; /* the length of the maximum sequence which ends here */
   int nlen; /* number of numbers */

   bignum_t cnt[5000]; /* the number of maximal sequences which end here */

   /* allocate a bignum_t */
   bignum_t get_big(void)
    { /* because the bignum structure is so small, we'll allocate them
         in blocks */
     static bignum_t block;
     static int size = 0;
     if (size == 0)
      { /* no more structures left, allocate a new block */
       block = (bignum_t)malloc(sizeof(*block)*128);
       size = 128;
      }
     size--;
     return block++;
    }

   /* initialize a bignum to this value */
   void init_big(bignum_t *num, int val)
    {
     *num = get_big(); /* allocate space */

     /* initialize */
     (*num)->val = val;
     (*num)->next = NULL;
    }

   /* add b to a (a <- a+b) */
   void add(bignum_t a, bignum_t b)
    {
     int c; /* carry */
     
     c = 0;
     while (b || c) /* while either there's more b, or there's a carry */
      {
       a->val += c; /* add the carry */
       if (b) a->val += b->val; /* add b, if there's more b */

       /* if a->val is too large, we need to carry */
       c = (a->val / 1000000);
       a->val = (a->val % 1000000);

       if (b) b = b->next;
       if (!a->next && (b || c))
        { /* allocate if we need to */
         a->next = get_big();
         a = a->next;

         /* initialize the allocated space to 0 */
         a->val = 0;
         a->next = NULL;
        } else a = a->next;
      }
    }

   /* output the number v to the FILE f. no leading spaces or trailing newline */
   void out_num(FILE *f, bignum_t v)
    {
     if (v->next)  /* not at first digits */
      {
       out_num(f, v->next); /* output the more significant digits */
       fprintf (f, "%06i", v->val); /* output these digits, 0 padding is necc */
      }
     else 
       fprintf (f, "%i", v->val); /* most significant digits, no 0-padding */
    }
   ```

## lgame

每个字符有个值

给一个字符串

给一个字典

求最大重新排列的值的和,可以多个相加



发现两个长度都是3->7..直接枚举就过了......

为了sort标记词组

```c++
		if (v > maxValue){
                ans.clear();
                maxValue = v;
                ans.push_back(three[i] + three[j] + '3');}
```



## race3

给一个有向好图(v<50,e<100),有起点和终点,找出所有有下列条件的点

1. 必须经过

2. 可以把图分成两块好图

   - 好图的定义:

     - 从任意一个路口都可以到达终点。

     - 终点不通往任何路口

     - 每一个路口都可以由起点到达。

       ​

想了好久没想出算法..看了眼数据大小(v<50,e<100)直接暴力啊....

一开始对于好图的理解不对,所以就错了很多次,就是分割点不能引出来往左边的点

第一个枚举+SPFA

第二个枚举+并查集

话说..第二个居然不能枚举第一个的结果...



## 总结

先不要把问题想复杂..先思考暴力能不能做出来…没准就....

# USACO4.4 三道题

这次三道题

Shuttle Puzzle 是普通题,可以找规律,可以搜索+剪枝.

Pollutant Control 是网络流,有几个难点需要仔细处理,不能直接套模板

Frame Up 是简单题...递归扫一遍,注意输出



## shuttle

```c++
N = 3 
初始状态: WWW_BBB 
目标状态: BBB_WWW
```

N <= 12

求字典序最小的最小步数...

(找规律可以发现最小步数是N * (N+2))

深搜+剪枝..0s过(可以用规律优化)

- (nocow)构造法

  ![官方图片](http://train.usaco.org/usaco/probs/prob27a.gif)

  根据答案寻找规律（可以证明正确性）:

  ```c++
  3
  3 5 6 4 2 1 3 5 7 6 4 2 3 5 4
  2 1 -2 -2 -1 2 2 2 -1 -2 -2 1 2 -1

  4
  4 6 7 5 3 2 4 6 8 9 7 5 3 1 2 4 6 8 7 5 3 4 6 5
  2 1 -2 -2 -1 2 2 2 1 -2 -2 -2 -2 1 2 2 2 -1 -2 -2 1 2 -1

  5
  5 7 8 6 4 3 5 7 9 10 8 6 4 2 1 3 5 7 9 11 10 8 6 4 2 3 5 7 9 8 6 4 5 7 6
  2 1 -2 -2 -1 2 2 2 1 -2 -2 -2 -2 -1 2 2 2 2 2 -1 -2 -2 -2 -2 1 2 2 2 -1 -2 -2 1 2 -1

  6
  6 8 9 7 5 4 6 8 10 11 9 7 5 3 2 4 6 8 10 12 13 11 9 7 5 3 1 2 4 6 8 10 12 11 9 7 5 3 4 6 8 10 9 7 5 6 8 7
  2 1 -2 -2 -1 2 2 2 1 -2 -2 -2 -2 -1 2 2 2 2 2 1 -2 -2 -2 -2 -2 -2 1 2 2 2 2 2 -1 -2 -2 -2 -2 1 2 2 2 -1 -2 -2 1 2 -1

    
  先观察样例数据，如果把还没移动的那一步也算上，那么空格的位置为
  4 3 5 6 4 2 1 3 5 7 6 4 2 3 5 4 (n=3,样例)
  5 4 6 7 5 3 2 4 6 8 9 7 5 3 1 2 4 6 8 7 5 3 4 6 5 (n=4)
  我们凭借极其敏锐的眼光发现这组序列为
  ４35 642 1357 642 35 4 (n=3,样例)
  5 46 753 2468 97531 2468 753 46 5 (n=4)
  即长度为1,2,3,4,...,n,n+1,n,...,4,3,2,1这样的2n+1组等差序列
  我们讨论第1~n+1组序列，这些序列满足
  　　*公差的绝对值为2
  　　*奇数组为降序列，偶数组为升序列
  　　*对于第i组(1<=i<=n+1),若为奇数组则首项为n+i，偶数组则首项为n-i+2
  对于第n+2~2n+1组，可以由对称性求出。
  输出时从第二组开始即可。
  ```

  ## *mike6 不宜用最小割优化算法的最小割...

  V<=32,E<=1000的有向图,给出每个割的价值,求最小数目的最小价值割,有多组输出字典序最小的

  最大流=最小割中,最大流的意义就不大了...

  但是还是要套用最大流算法

- 这道题尴尬的是输出,因此不能完全套用模板

  想了很久去看题解,输出字典序最小的话,那么最后就应该枚举边集

  (话说看题解释nocow吵的很开心..都说别人错了并给出范例数据= =)

  然后是最小数目的割,观察到E<=1000所以编码优化,存储流信息的时候,`cost = c * 1001 + 1;`

  然后用一个edge表存边,用邻接矩阵存图,由于V很小,没必要用邻接表优化,直接枚举就行

  (话说我也觉得一开始学这个算法就应该用枚举)

  输出最小价值就是cost/1001,最小割数就是cost%1001.(其实这个没有先后关系吧.)

- 一堆WA给第一个网络流的bug警示

  - Dinic算法

    1. bfs时候起始点距离应该标记为1,不然可能有回边,这样会很麻烦
    2. dfs时候深搜下去是` temp = dfs(p,min(ca,cap[fr][p]));`然后当前位置矩阵和容量-temp,对称位置矩阵+temp.ca不能写成temp
    3. 输出字典序最小,那么应该设一个变量标记输出了多少个了,完成即退出

  - 算法简要描述

    1. bfs确定每一点到源点最近距离,源点为1.

    2. 如果终点距离存在,深搜扩展(源点,inf),否则退出算法

       - 深搜: 

         1. 如果是终点返回扩展量

         2. 否则枚举可行边,进行扩展.基本模板如下

            ```c++
            	long long temp,cnt = 0;
                for (int p = 1; p <= totNode; p++) {
                    if (dis[p] == dis[fr] + 1 && cap[fr][p]) {
                        //cout << temp << " " << ca << endl;
                        temp = dfs(p,min(ca,cap[fr][p]));
                        ca   -= temp;
                        cap[fr][p] -= temp;
                        cap[p][fr] += temp;
                        cnt += temp;
                    }
                }
            ```

  - min_cut算法

    ```c++
    void min_cut(){
        //广搜/floodfill包含源点联通区域(联通及f>0)
        int cnt = 0;
        for (int i = 1; i <= n; i+=2) 
            if (v[i]) //枚举联通区域每一点,确认割点
                for (int j = g[i]; j; j = e[j].nxt) 
                    if (!v[e[j].v] && e[j].f == 0) 
                        ans.push_back(e[j].v);
                        
    }
    ```

    ​

## Frame Up

给出一个形如下方的图层覆盖,求覆盖顺序

```c++
/*
矩形的边的宽度为 1 ，每条边的长度都不小于 3 。
矩形的每条边中，至少有一部分是可见的。注意，一个角同时属于两条边。
矩形用大写字母表示，并且每个矩形的表示符号都不相同。

30 30
..............................
.OOOOOOOOOYYYYYOO.............
.O....PPPPYPPPYPOPP...........
.O....P...Y...Y.O.P...........
.O....P...Y...Y.O.P...........
.O....P...Y...Y.O.P...........
.O....P...Y...Y.O.P...........
.O....P...Y...Y.O.P...........
.O....P...Y...Y.O.P...........
.O....P...Y...Y.O.P...........
.OOOOOOOOOYOOOYOEEEEEEEEE.....
......PPPPYPPPYPEPP.....E.....
..........Y...Y.E.......E.....
..WWWWWWWWWWWWWWWWWTT...E.....
..W...T...Y...Y.E.W.T...E.....
..W...T...Y...Y.E.W.T...E.....
..W...T...Y...Y.E.W.T...E.....
..W...T...YYYYY.EEWEEEEEE.....
UUWWWWWWWWWWWWWWWWW.T.........
U.....TTTTTTTTTTTTQQQQQQQQQQQ.
U....RRRRRRRRRRRRRQRRRRR....Q.
U....R............Q....R....Q.
UIIIIRRRRRRRRRRRRRQRRRRRIIIIQ.
UI................QQQQQQQQQQQ.
UUUUUUUUUUUUUUUUUUU........I..
.IIIIIIIIIIIIIIIIIIIIIIIIIII..
..............................
..............................
..............................
..............................

*/
```

本来想找标志的..看到规则这道题就很简单了

就是要注意输出顺序

读入的时候标记每个字符的最上最左最右最下....check是否为当前字符或者已经用了的字符

有一点栈的思想

# fence

- 欧拉回路

  主要思路就是一个dfs,一开始写了一个while卡在倒数第二个case..前面坑了一次重边和一个非零起始点.

  下面代码(模板的思路)是随便一个点开始,(因为大小顺序所以优先队列来水...)然后最后记得倒序输出.这样也解释了为什么一路搜下去万一先碰到奇数点的问题,因为倒序的话,奇数点是最先入队的(每次dfs最后一步入队嘛)

```c++
void euler(int p){
    while (net[p].size()) {
        int nextV=net[p].top();
        net[p].pop();
        if (road[p][nextV]==0) {
            continue;
        }else{
            road[p][nextV]--;
            road[nextV][p]--;
            deg[p]--;deg[nextV]--;
            euler(nextV);
            if (road[p][nextV]) {
                net[p].push(nextV);
            }
        }
    }
    vis.push_back(p);
}
```



# shopping(多维背包)

一开始暴力..挂在编译器上面

```c++
//不能愉快玩耍的输入:
//fin>>products[i]>>needNum[products[i]]>>oriPrice[products[i]];
    for (int i=0; i<b; i++) {
        fin>>products[i];
		fin>>needNum[products[i]]>>oriPrice[products[i]];
    }
```

然后瘫痪在case10.

背包一直纠结在怎么找点上

dp的关键是这个数组逻辑的变换

```c++
//b数组存下反过来的编号,然后k2进行编号转换,五重循环从k2[i]开始
for (long j=0;j<5;++j)
     k2[b[so[i].component[j]]]=so[i].amount[j];
```

或者..给每一个special一个1k大的数组,直接拿空间换编程复杂度,从当前开始即可.

这道题毒性太重,第一次有想贴完整代码冲动

```c++
/*
 ID: leidar11
 PROG: shopping
 LANG: C++11
 */
#include <iostream>
#include <fstream>
using namespace std;
const int maxn=1001;
struct offer{
    int num[maxn];
    int price;
}special[100];
int id[6],oriprice[maxn],need[maxn];
int k[6],dp[6][6][6][6][6];//dp&loop
ifstream fin("shopping.in");
ofstream fout("shopping.out");
#define res0(i,n) for(i=0;i<=n;i++)
#define res(i,n) for(i=1;i<=n;i++)
#define CUR dp[k[1]][k[2]][k[3]][k[4]][k[5]]
#define min(a,b) ((a<b)?a:b)
int main(){
    int totoffer,totneed,temp,tempnum,ord,code;
    fin>>totoffer;
    res(temp, totoffer)
    {
        fin>>tempnum;
        res(ord, tempnum)
        {
            fin>>code;
            fin>>special[temp].num[code];
        }
        fin>>special[temp].price;
    }
    fin>>totneed;
    res(temp, totneed)
    {
        fin>>id[temp];
        fin>>need[id[temp]]>>oriprice[id[temp]];
    }
    res0(k[1], need[id[1]])res0(k[2], need[id[2]])res0(k[3], need[id[3]])
    res0(k[4], need[id[4]])res0(k[5], need[id[5]])res(temp, totneed)
    CUR+=k[temp]*oriprice[id[temp]];
    
    res(temp, totoffer)
    for (k[1]=special[temp].num[id[1]]; k[1]<=need[id[1]]; k[1]++) {
        for (k[2]=special[temp].num[id[2]]; k[2]<=need[id[2]]; k[2]++) {
            for (k[3]=special[temp].num[id[3]]; k[3]<=need[id[3]];k[3]++ ) {
                for (k[4]=special[temp].num[id[4]]; k[4]<=need[id[4]]; k[4]++) {
                    for (k[5]=special[temp].num[id[5]]; k[5]<=need[id[5]]; k[5]++) {
                        CUR=min(CUR,dp[k[1]-special[temp].num[id[1]]][k[2]-special[temp].num[id[2]]][k[3]-special[temp].num[id[3]]][k[4]-special[temp].num[id[4]]][k[5]-special[temp].num[id[5]]]+special[temp].price);
                    }
                }
            }
        }
    }
    fout<<dp[need[id[1]]][need[id[2]]][need[id[3]]][need[id[4]]][need[id[5]]]<<endl;
    return 0;
}
```



# NOCOW 5.1 凸包

> 类似把顶点放入多边形的题目通常是求凸包。如果题目要求一个面积最小的凸多边形，或者周长最小的凸多边形，那么我们几乎可以确定是要求凸包了。
>
> 不幸的是，这个算法不能简单地推广到三维的情形。幸运的是，三维凸包算法全都超级复杂（四维以上的更恶心），所以题目不太可能要你去求。
>
> 如果你给多边形加上任何限制条件时，这个算法就玩完了（例如，多边形的顶点不多于 n 个，或者必须是矩形）。



[凸包算法](http://www.nocow.cn/index.php/Translate:USACO/ConvexHulls)



## 凸包算法流程(nocow)

- 找出一个必定会在凸包内的中点
- 计算每个点和中点的连线与x轴的夹角（在 0——360 度的范围内）
- 根据这些夹角对顶点排序
- 加入最初的两个顶点
- 对于除最后一个顶点以外的其余顶点
- 让其成为凸包上的下一个顶点
- 检查它和前面两个顶点组成的角是否大于 180 度
  - 如果它和前面两个顶点组成的角大于 180 度，那么把它前面那个顶点删掉
- 加入最后一个顶点
  - 完成上述的删除任务
  - 检查最后一个顶点和它的前一个顶点和第一个顶点所组成的角是否大于 180 度，或者最后一个顶点和第一、第二个顶点组成的角是否大于 180 度。
  - 如果第一种情况为真，删除最后一个顶点，并且检查倒数第二个顶点。
  - 如果第二种情况为真，删除第一个顶点，继续检查。
  - 当两种情况都不为真时，停止。
- 由于角度的计算方式，增加顶点的时间复杂度是线性的（就是我们所说的 O(n) ）。因此，运行的时间复杂度决定于排序的时间复杂度，所以“卷包裹法”的时间复杂度是 O(nlog n )，这是最优的。

## Fencing the Cows 凸包模板题

求一个凸包的周长

```c++
红书模板
  const double eps = 1e-8;
inline int cmp(double x){
    if (fabs(x) < eps) {
        return 0;
    }if (x > 0) {
        return 1;
    }
    return -1;
}
double det(const point &a,const point&b){
    return a.x * b.y - a.y * b.x;
}
point operator-(const point &a,const point&b){
    return make_pair(a.x-b.x, a.y-b.y);
}

inline bool comp_less(const point&a,const point&b){
    return cmp(a.x - b.x) < 0 || (cmp(a.x - b.x)==0 && cmp(a.y - b.y)<0);
}


  sort(a.begin(), a.end(), comp_less);
    a.erase(unique(a.begin(), a.end()), a.end());
    vector<point>res;
    res.resize(a.size() * 2 + 5);
    int m = 0;
    for(int i = 0;i < a.size();++i){
        while (m > 1 && cmp(det(res[m-1] - res[m-2], a[i]-res[m-2]))<=0) --m;
        res[m++] = a[i];
    }
    int k = m;
    for (int i = int(a.size()) - 2; i >= 0; --i) {
        while (m > k && cmp(det(res[m-1] - res[m-2], a[i]-res[m-2]))<=0) --m;
        res[m++] = a[i];
    }
    res.resize(m);
    if (a.size() > 1) {
        res.resize(m-1);
    }
    
```

一开始看成面积错了一次.



## starry

美哭的一道题...

对于代码量的掌控能力还是有待提升..从下午打到晚上差不多三百行的代码量

交了9次的原因是三个地方的数组越界…看错题意QAQ否则过了样例基本过了

最后用谜一样的TLE函数来测试..

```c++
void getTLE(){
    for (; ; ) {
        cout << 1 << endl;
    }
}
```



题意是有一堆星系(八个方向可以到达的点),编号,给相似星系相同编号,求最小编号

相似星系如下:

![相似](http://www.nocow.cn/index.php/%E6%96%87%E4%BB%B6:Starry-1.gif)

测试样例如下:

![样例](http://www.nocow.cn/index.php/%E6%96%87%E4%BB%B6:Starry-3.gif)



算法: 

- 从左到右从上到下floodfill扫一遍,
  - 给每个区域编号
  - 记录区域四个方向的界限
  - 统计数目丢到一个同数向量里面
- 对每个向量枚举,测试相似星系,因为先丢进来的肯定小..
- 然后dfs给答案数组编号,输出(至于最小字符序用map存下来对应就好啦)


- 枚举测试:

- 一开始没思路...突然发现两个相似可以通过相邻点作差比较得到

- 枚举八个相似形状,发现刚好把左右上下八个方向都用了一遍

- (中途因为修正答案,打了一堆注释)

- 发现还是有一些没有相似标记成功,分析不成功的输出数据发现

- **相似不能只用一个点判别,而应该用一个起始点+两个方向向量**

  ```c++
  typedef pair<int, int> LU;
  vector<LU> TN,XN;//TN是模板,XN是待测(其实可以互换...)
  inline LU operator-(const LU &a,const LU &b){
      return make_pair(a.first - b.first, a.second - b.second);
  }
  inline LU operator+(const LU &a,const LU &b){
      return make_pair(a.first + b.first, a.second + b.second);
  }

  //枚举太多了,留一个到blog里面
  		case 1:
              for (int c = node[val].r; c >= node[val].l; c--) {
                  for (int r = node[val].u; r <= node[val].d; r++) {
                      if (vis[r][c] == val) {
                          XN.push_back(make_pair(-c, r));
                      }
                  }
              }//r->l u->d
  inline bool check(int i,int size,int x)
  {
      int h = points[size][x].first;
      int w = points[size][x].second;
      int val = vis[h][w];
      fillxn(i, val);//枚举...
      for (int i = 1; i < XN.size(); i++) {
          if (XN[i] - XN[i-1] != TN[i] - TN[i-1]) {
              return false;
          }
      }
      return true;
  }
  ```

- 所以相似就是每个的差相等..

```c++
//官方
/* is shape id the same as the current shape */
/* specify the orientation with dy/dx and by/bx */
/* dy/dx is the difference value to associate with y and x changes */
/* by/bx is the upper right corner of the bounding box with respect
   to the current orientation */
/* NOTE: assumes that the number of stars are the same */
int
check_shape(int id, int dy, int dx, int by, int bx)
 {
  int lv;
  int pval;
 
  for (lv = 0; lv < ncurrent; lv++)
   {
    pval = (current[lv][0]-by)*dy + (current[lv][1]-bx)*dx;
    if (!bsearch(&pval, stars[id], count[id], sizeof(stars[id][0]), int_comp))
      return 0; /* found a star that didn't match */
   }
  return 1;
 }
```



## theme

与前一个位置作差

二分长度+暴力枚举(因为模板里面的最长重复子串是string...)过的

流真的很慢...一开始忘记屏蔽cerr挂了几次超时..

> dp
>
> 令theme(i,j)表示第一个主题开头为i，第二个主题开头为j所能构成的重复主题的长度
>
> 那么有转移方程：
>
> 如果note(i+1)-note(i)=note(j+1)-note(j)，则theme(i,j)=them(i+1,j+1)+1,否则theme(i,j)=1(深刻啊)
>
> 这个方程需满足 theme(i+1,j+1)+1 <= j-i 否则两主题将会重叠--------dementrock注

## [启发式搜索](http://www.nocow.cn/index.php/Translate:USACO/HeuristicSearch)

启发式搜索的主要思想是通过评价一个状态有"多好"来改进对于解的搜索.



## 方法#1:启发式剪枝

估价函数最简单最普通的用法是进行剪枝.假设有一个求最小代价的一个搜索,使用一个可行的估价函数.如果搜到当前状态时代价为A,这个状态的估价函数是B,那么从这个状态开始搜所能得到的最小代价是A+B.如果当前最优解是C满足C<A+B,那么就不需要从这个状态开始搜索.

## 方法#2:最佳优先搜索

最佳搜索可以看成贪心的深度优先搜索.

与一般搜索随意扩展后继节点不同,最优优先搜索按照估价函数所给的他们的"好坏"的顺序扩展节点.与只扩展最优节点的贪心不同,最佳优先搜索先扩展较"优"的后继,然后再扩展较"差"的后继.与上面的启发式剪枝组合,它可以很好地提升效率.

## 方法#3:A*搜索

A*搜索和"贪心"的广度优先搜索相似.

广度优先搜索总是扩展已达到的代价最小节点.但是,A*搜索,扩展最"有可能"的节点(就是说,到达1节点的代价和这个节点的估价函数之和最小)

状态被保存在优先队列中,优先级是状态的代价与估价函数之和.每一步,算法移除优先级最小的节点,将它的后继加入优先队列中.

如果估价函数可行,A*搜索到的第一个结束状态可以保证是最优的.



## Milk Measuring

求一元不定方程是否有正整数解



- 一开始思路是求一元不定方程是否有整数解,WA一次后发现是正整数解

- 然后在枚举到最后完全背包过了,时间没有前面的同学快.但是思路特别简单...

  第一个判断方法就是**裴蜀定理**被选数的gcd能被Q整除,TEST7和10用了0.1s.....-leidar

- 好像写出了DFSID(迭代加深搜索)的感觉...

```c++
//别人0s过的代码
void dfs(int depth, int rv, int bc)
{
	if (depth == i)
	{
		if (rv == 0)
		{
			memcpy(ans, cur, i << 2);
			flag = true;
		}
		return;
	}
	if (bc > n || ans[depth] < v[bc]) return; //最优性剪枝
	cur[depth] = v[bc];
	for (int m = 1; m * v[bc] <= rv; ++m)
		dfs(depth + 1, rv - m * v[bc], bc + 1);
	if (bc < n) dfs(depth, rv, bc + 1);
}
```

## window

模拟一堆矩形(窗口)的上下波动,求可见面积

有删除创造,放到顶端和低端四种操作以及对于面积的动态询问



第八次提交终于过了0.0

- 一开始觉得这是在模拟一个栈

- 但是可以放到底端(一开始以为最小化..)所以不能用STL中stack模拟

  - 所以这里用一个两倍长最大值的高度数组,以及在结构体内反对应高度

- 然后前面四个操作就很简单了,模拟位置变化

- 第五个操作

  - 一开始尝试随机数撒..真是sa...精度从来没对过还调了好久

  - 再尝试暴力TLE(想必也是..不过想炸出数据...)

    - 暴力的时候发现矩形给的位置不同很麻烦

      ```c++
      //在结构体内加上sx,sy,mx,my来存四个极端位置
      inline bool INRECT(DD p,int i){
          int x = p.first,y = p.second;
          if (x < n[i].sx || x > n[i].mx) {//x >=
              return false;
          }
          if (y < n[i].sy || y > n[i].my) {//y >=
              return false;
          }
          return true;
      }
      //其实这个判断函数应该一边有等于的,但是由于后面测试函数的修改就懒得加了
      ```

- 看nocow上大家都说矩形切割...(WHAT'S THIS…?)最后一个小哥说了句离散化简直给予了希望

- 然后在创造操作里面加上了这句话来离散坐标

  ```c++
  set<int>xpos,ypos;//set对于离散化真是神器(自动排序寻找hhh)
  //插入
  xpos.insert(n[id].x);
  xpos.insert(n[id].X);
  ypos.insert(n[id].y);
  ypos.insert(n[id].Y);
  //找到对应位置的指针:
  set<int>::iterator x=xpos.find(n[id].sx);
  /*
  cout << *test.lower_bound(5) << endl;
  cout << *test.lower_bound(6) << endl;
  找到
  */   
  //删除
  不需要删除!!!万一有重复的呢..
  ```

- 再用离散后的坐标做

  - 但是面积很麻烦...用bfx记录上一个x,bfy记录上一个y

  - 距离判断真的很麻烦...一开始设了一个flag变量随着y变化是否变

  - WA两次后调整算法

    ```c++
    //不过加了一点常数嘛..
    //把测试函数对于矩形四条边都表示包含进去..不管它
    			int suc = 0;
                if (tt(mp(*x, *y),id)
                    || tt(mp(((*x + bfx)>>1),*y), id)
                    || tt(mp(*x,((*y+bfy)>>1)), id)
                    || tt(mp(((*x + bfx)>>1), ((*y+bfy)>>1)), id))
                    suc = ((*y - bfy)*len);
    //恩 然后测试内点...(写的时候突然发现只需要测一个...//其实并没有,只是更换一下顺序能优化时间)
    ```

- ```c++
  //test学习~
  #include <iostream>
  #include <set>
  using namespace std;
  set<int>test;
  void print(){
      for (set<int>::iterator it = test.begin(); it != test.end(); it++) {
          cout << *it << " ";
      }
      cout << endl;
  }
  int main(){
      for (int i =0; i < 10; i+=2) {
          test.insert(i);
      }//0 2 4 6 8 
    	//<
      cout << *test.upper_bound(5) << endl;//6
      cout << *test.upper_bound(6) << endl;//8
      //>=
      cout << *test.lower_bound(5) << endl;//6
      cout << *test.lower_bound(6) << endl;//8
    	
    	print();//0 2 4 6 8 
      test.erase(1);
      print();//0 2 4 6 8 
      test.erase(2);
      print();//0 4 6 8 
      test.erase(test.find(0));//这里还用了find函数,返回指针
      print();//4 6 8 
    	cout << *test.find(0) <<" " << *test.find(8) << endl;//注意第一个是test.end()
      return 0;
  }
  ```

  - 别人代码的方法:

    ```c++
    //求切割
    int calc_area(int x1,int x2,int y1,int y2,int floor){
        int ans=0;
        while ((floor<=sum_window) && ((x1>=win[floor].x2) || (x2<=win[floor].x1) || (y1>=win[floor].y2) || (y2<=win[floor].y1))) ++floor;
        if (floor>sum_window) {ans+=(x2-x1)*(y2-y1);return ans;}
        if (x1<win[floor].x1) {ans+=calc_area(x1,win[floor].x1,y1,y2,floor+1);x1=win[floor].x1;}
        if (x2>win[floor].x2) {ans+=calc_area(win[floor].x2,x2,y1,y2,floor+1);x2=win[floor].x2;}
        if (y1<win[floor].y1) {ans+=calc_area(x1,x2,y1,win[floor].y1,floor+1);y1=win[floor].y1;}
        if (y2>win[floor].y2) {ans+=calc_area(x1,x2,win[floor].y2,y2,floor+1);y2=win[floor].y2;}
        return ans;
    }
    ```

  ​

## **Network of Schools**

- 题意: 给出一个有向图图,求两个东西

  1. 要给多少个节点资源才能"流"到所有节点
  2. 要补上多少条(有向)边才能让整个图强连通

- 思路(3A): 

  1. 一开始思考第一个问题觉得就是并查有多少个集合

  2. 但是发现有向图后发现**可能有多个入度为0的点**转变成思考有多少个入度为0的点,统计输出.然后对于**没有入度为0的弱连通区域**记作1

  3. 第十个样例画了一个这样的图.当时就懵逼了一会

      ![屏幕快照 2016-05-30 12.49.32](屏幕快照 2016-05-30 12.49.32.png)

     这样算红色点(入度为0)的话紫色点是遍历不到的

      ![屏幕快照 2016-05-30 12.49.38](屏幕快照 2016-05-30 12.49.38.png)

  4. 于是在去高数课的路上苦思冥想(因为没赶上小白XD).想起了IDDFS.

  5. 也就是说在每次测0indeg的之后,dfs一次统计每个点的DFS入度.再扫一遍看是否有入度小于实际入度,如果有的话就是上图的紫色部分(可能不止一块区域),然后再迭代加深搜索(需要多少/哪些点),算出来需要剩下的数目.

  6. 所以这里的方法也就是贪心(因为入度为0的必须要)+迭代加深

  7. 第二问还是一道图论题,一开始发现两种情况

     1. 有一个单独的强连通区域,那么就不需要
     2. 区域不止一个,那么强联通区域就可以穿插在弱联通区域的补边上

  8. 弱连通区域可以每一个"叶子"给每一个"根"相连,第三个数据挂了后发现应该是`max(inZero[i],outZero(i))`然后因为不同的区域之间可以通过叶子的循环相连*也就是说可以我的叶子连你的根,你的连回我的*这样就不需要多补边.

  9. 总结一下用了并查集来优化搜索范围,IDDFS来优化搜索深度.0S过~不过中间写到一半发现有个变量应该作为一个参数传递下(被引用的函数)去就用了一个乱七八糟名字的全局变量传了过去..毁了代码风格


- 正确思路(no cow)

  - 可以把该题中所有强连通分量收缩成分别一个顶点,则入度为0的顶点就是最少要接受新软件副本的学校。

  - 第二问就是，问至少添加多少条边，才能使原图强连通。也就问在收缩后的图至少添加多少条边，才能使之强连通。

  - [强连通分类计算方法](http://www.cnblogs.com/luweiseu/archive/2012/07/14/2591370.html)

    - 用floyed算出两点间是否可以互相到达。然后枚举任意两个顶点Vi还有Vj，如果两个点互相可以到达，那么他们就是属于同一个强连通分量了。O(n3)

    - Kosaraju算法、Tarjan算法和Gabow算法，它们都可以在线性时间内找到图的强连通分支。

    - Kosaraju算法

      - 逆后序遍历

      - 转置得到RG

      - 对RG依照第一步得到顺序DFS,每一颗搜索树就是一个强连通分类

        ```c++
        void dfs(long u){
        	hash[u]=1;
        	for(long i=1;i<=n;i++)
        		if(!hash[i]&&a[u][i])
        			dfs(i);
        	f[++tim]=u;//存下访问顺序
        }//第一次遍历
        void dfs2(long u){
        	hash[u]=1;
        	big[u]=cnt;
        	for(long i=n;i>=1;i--)
        		if(!hash[f[i]]&&a[f[i]][u])
        			dfs2(f[i]);
        }//第二次遍历
        	for(long i=1;i<=n;i++)
        		if(!hash[i])//标记数组
                  dfs(i);
        	memset(hash,0,sizeof hash);
        	for(long j=n;j>=1;j--)
        		if(!hash[f[j]]){//用访问顺序访问
        			cnt++;
        			dfs2(f[j]);//第cnt个联通区域
        		}
        	for(long i=1;i<=n;i++)
        		for(long j=1;j<=n;j++)
        			if(a[i][j]&&big[i]!=big[j])
        				c[big[i]][big[j]]=true;
        	for(long i=1;i<=cnt;i++)
        		for(long j=1;j<=cnt;j++)
        			if(c[i][j]){
        				inp[j]++;oup[i]++;
        			}
        	
        ```

        ​

    - Tarjan算法

      > Tarjan基于递归实现的深度优先搜索，在搜索过程中将顶点不断压入堆栈中，并在回溯时判断堆栈中顶点是否在同一联通分支。函数借助两个辅助数组pre和low，其中pre[u]为顶点u搜索的次序编号，low[u]为顶点u能回溯到的最早的顶点的次序编号。当pre[u]=low[u]时，则弹出栈中顶点并构成一个连通分支。

      1. 数组的初始化：当首次搜索到点p时，Dfn与Low数组的值都为到该点的时间。
      2. 堆栈：每搜索到一个点，将它压入栈顶。
      3. 当点p有与点p’相连时，如果此时（时间为dfn[p]时）p’不在栈中，p的low值为两点的low值中较小的一个。
      4. 当点p有与点p’相连时，如果此时（时间为dfn[p]时）p’在栈中，p的low值为p的low值和p’的dfn值中较小的一个。
      5. 每当搜索到一个点经过以上操作后（也就是子树已经全部遍历）的low值等于dfn值，则将它以及在它之上的元素弹出栈。这些出栈的元素组成一个强连通分量。
      6. 继续搜索（或许会更换搜索的起点，因为整个有向图可能分为两个不连通的部分），直到所有点被遍历。

```
  ```c++
  void tarjan(int u){
      LOW[u] = DFN[u] = ++step;
      visit[u] = true;
      slack[++top] = u;
      for (int i = 1 ; i <= links[u][0] ; i++)
          {
              int v = links[u][i];
              if (!DFN[v])
                  {
                      tarjan(v);
                      LOW[u] = min(LOW[u] , LOW[v]);
                      }
              else
                  {
                      if (visit[v])
                          LOW[u] = min(LOW[u] , DFN[v]);
                      }
          }
      if (LOW[u] == DFN[u])
          {
              Cnt++;
              int v;
              do
              {
                  v = slack[top--];
                  Belong[v] = Cnt;
                  visit[v] = false;
              }
              while (u!=v);
          }
      }

  ```

  ​
```

## bigbarn

给个正方形撒一些点,求最大没有点的正方形..

好像以前做过..`dp[LINE][ROW] = (maze[LINE][ROW] == 1)?0:MIN(dp[LINE-1][ROW-1],dp[LINE-1][ROW],dp[LINE][ROW-1])+1;`

- 滚动数组优化

  ```c++
  k=min(f[i-1][j],f[i][j-1]);
  f[i][j]=hash[i-k][j-k]?k:k+1;
  ans=max(ans,f[i][j]);
  ```

- 二维树状数组+二分

  ```c++
  二维树状数组，预先保存每个正方形的面积，然后二分边长是否符合即面积等于0
  /*ID: zyz91361
  LANG: C++
  TASK: bigbrn
  */
   
  #include <cstdio>
  #include <cstring>
   
  const int N = 1001;
  int g[N][N],s[N][N];
  int row,col,n;
   
  inline int lowbit(const int &x){
      return x &(-x);
  }
   
  int sum(int i,int j){
      int tempj,sum = 0;
      while(i > 0){
          tempj = j;
          while(tempj > 0){
              sum += g[i][tempj];
              tempj -= lowbit(tempj);
          }
          i -= lowbit(i);
      }
      return sum;
  }
   
  void update(int i,int j,int num){
      int tempj;
      while(i <= row){
          tempj = j;
          while(tempj <= col){
              g[i][tempj] += num;
              tempj += lowbit(tempj);
          }
          i+=lowbit(i);
      }
  }
   
  int is_ok(int k){
      for(int i = k; i <= n; i++)
          for(int j = k; j <= n; j++){
              if(!g[i][j]&&(s[i][j]-s[i-k][j]-s[i][j-k]+s[i-k][j-k]==0)) return 1;
          }
      return 0;
  }
   
  int main(){
   
      freopen("bigbrn.in","r",stdin);
      freopen("bigbrn.out","w",stdout);
     // memset(g,0,sizeof(g));
      int T;
      scanf("%d%d",&n,&T);
      row = col = n;
      for(int i = 0; i < T; i++){
          int u,v;
          scanf("%d%d",&u,&v);
          update(u,v,1);
      }
      for(int i = 1; i <= n; i++)
          for(int j = 1; j <= n; j++)
              s[i][j] = sum(i,j);
      int l = 0,r = n;
      int maxn = 1;
      while(l <= r){
          int mid = (l+r)/2;
          if(is_ok(mid)){
              l = mid+1;
              if(maxn < mid) maxn = mid;
              else break;
          }else r = mid-1;
      }
      printf("%d\n",maxn);
  }
  ```

- 据说枚举也能过..

# USACO5.4 TSP_背包_网络流拆点



## tour 双调TSP&&传纸条模型

题意,给出一个图,从最东边到最西边再返回,求最多经过的城市,除了起始只能经过一次



- 一开始想二分从左到右最多的城市,T
- 而后从左边开始两条路同时搜索,T
- DP,状态定义为同时走到`i,j` 城市的最大距离
  - `[1,1] = 1,f[n,n] = 0(1<n<V),f[j,i] = f[i,j] = max(f[n,j] + 1)`其中j到i有路,最后扫一遍f[v,j]就可以过了.
  - 因为`f[n,n] = 0`所以不会经过相同的城市


- 最大费用流

  - > 把第i个城市拆分成两个顶点<i.a>,<i.b>。 1、对于每个城市i，连接(<i.a>,<i.b>)一条容量为1，费用为1的有向边，特殊地(<1.a>,<1.b>)和(<N.a>,<N.b>)容量设为2。 2、如果城市i,j(j>i)之间有航线，从<i.b>到<j.a>连接一条容量为1，费用为0的有向边。 求源<1.a>到汇<N.b>的最大费用最大流。 如果(<1.a>,<1.b>)不是满流，那么无解。否则存在解，即为最大费用最大流量 - 2。

  - > 每条航线都是自西向东，本题可以转化为求航线图中从1到N两条不相交的路径，使得路径长度之和最大。转化为网络流模型，就是找两条最长的增广路。由于每个城市只能访问一次，要把城市拆成两个点，之间连接一条容量为1的边，费用设为1。因为要找两条路径，所以起始点和终点内部的边容量要设为2。那么费用流值-2就是两条路径长度之和，为什么减2，因为有两条容量为2的边多算了1的费用。求最大费用最大流后，如果(<1.a>,<1.b>)不是满流，那么我们找到的路径不够2条（可能是1条，也可能0条），所以无解。

  - 代码(nocow)

    ```c++
    #include <iostream>
    #include <cstdio>
    #include <cstdlib>
    #include <cmath>
    #include <cstring>
    using namespace std;
    const int MAXN=1001,MAXM=MAXN*4*2,INF=~0U>>1;
    struct Queue
    {
    	int Q[MAXN],head,tail,size;
    	bool inq[MAXN];
    	void init()
    	{
    		memset(inq,0,sizeof(inq));
    		head = size =0; tail = -1;
    	}
    	void ins(int p)
    	{
    		size++;
    		if (++tail == MAXN) tail = 0;
    		Q[tail] = p;
    		inq[p]=true;
    	}
    	int pop()
    	{
    		size--;
    		int p=Q[head];
    		if (++head == MAXN) head = 0;
    		inq[p]=false;
    		return p;
    	}
    }Q;
    struct edge
    {
    	edge *next,*op;
    	int t,c,v;
    }*V[MAXN],ES[MAXM],*fe[MAXN];
    char City[MAXN][16];
    int N,M,S,T,EC,Ans,Costflow;
    int dist[MAXN],ft[MAXN];
    inline void addedge(int a,int b,int c,int v)
    {
    	ES[++EC].next = V[a]; V[a]=ES+EC; V[a]->t=b; V[a]->c=c; V[a]->v=v;
    	ES[++EC].next = V[b]; V[b]=ES+EC; V[b]->t=a; V[b]->c=0; V[b]->v=-v;//反向边
    	V[a]->op = V[b]; V[b]->op = V[a];
    }
    int getcity(char *s)
    {
    	for (int i=1;i<=N;i++)
    		if (strcmp(s,City[i])==0)
    			return i;
    	return -1;
    }//邻接表初始化图
    void init()
    {
    	int i,a,b;
    	char name[16];
    	freopen("tour.in","r",stdin);
    	freopen("tour.out","w",stdout);
    	scanf("%d%d",&N,&M);
    	for (i=1;i<=N;i++)
    	{
    		scanf("%s",City[i]);
    		if (i==1 || i==N)
    			addedge(i,i+N,2,-1);
    		else
    			addedge(i,i+N,1,-1);
    	}
    	for (i=1;i<=M;i++)
    	{
    		scanf("%s",name);a=getcity(name);
    		scanf("%s",name);b=getcity(name);
    		if (a<b)
    			addedge(a+N,b,1,0);
    		else
    			addedge(b+N,a,1,0);
    	}
    	S=1; T=N+N;
    }
    //找到增广路
    bool SPFA()
    {
    	int i,j;
    	for (i=S;i<=T;i++)
    		dist[i]=INF;
    	dist[S]=0;
    	Q.ins(S);
    	while (Q.size)
    	{
    		i=Q.pop();
    		for (edge *e=V[i];e;e=e->next)
    		{
    			j=e->t;
    			if (e->c && dist[i] + e->v < dist[j])
    			{
    				dist[j] = dist[i] + e->v;
    				ft[j] = i;
    				fe[j] = e;
    				if (!Q.inq[j])
    					Q.ins(j);
    			}
    		}
    	}
    	return dist[T]!=INF;
    }
    //增广
    void Augment()
    {
    	int i,delta=INF;
    	for (i=T;i!=S;i=ft[i])
    		if (fe[i]->c < delta)
    			delta = fe[i]->c;
    	for (i=T;i!=S;i=ft[i])
    	{
    		fe[i]->c -= delta;
    		fe[i]->op->c += delta;
    		Costflow += fe[i]->v * delta;//加上增广费用
    	}
    }
    void SPFAFlow()
    {
    	Q.init();
    	while (SPFA())
    		Augment();
    }
    int main()
    {
    	init();
    	SPFAFlow();
    	Costflow =-Costflow;
    	Costflow -= 2;
    	if (ES[1].c!=0)
    		Costflow = 1;
    	printf("%d\n",Costflow);
    	return 0;
    }
    ```

# charrec

题意: 给出a~z的图像,然后给出另外一条图像求出对应字符串

输入文件包含一个或多个可能损坏的字符图案。一个字符图案可能以这些方式被损坏。

- 最多有一行可能被复制了（就接在原来那一行的下面）
- 最多有一行可能丢失了
- 有些“0”可能被改成“1”
- 有些“1”可能被改成“0”

不会有任何一个字符图案既多余了一行**并且**又丢失了一行。在测试数据的任何一个字符图案中，“0”和“1”的被改变率不超过 30%。

被复制的那一行中，原来的行和多余的行可能都损坏了，而且损坏的部分可能并不相同。



分析: 

1. dp,先预处理记录第c个字符的第i行与字符串第j行的dis值,再背包就好
2. 因为要输出,可以用结构体记录前一个的位置.
3. 对于三种不同的扩展每次找三个前卫就好了.

总结:

1. 代码打烂了就重打,不要一昧调试凑数据**没准还能找到错误地方(没错这题就是这么过的)**
2. WA很多次的原因是,,对应位置不是从0位开始dp的..
3. 算法比赛中可以通过短字母+注释的方法缩短代码长度,减少畏惧心理

## 网络流拆点

题意:

给出n割点,m条无向边

求分割点a到点b最少+最小割点集合



一开始IDDFS和二分都T在第八个数据



- 拆点:
  - 把每个点拆为`i*2`和`i*2+1`,连一条边,权值1
  - 原始边添加从`a*2+1`到`b*2`和从`b*2+1`到`a*2`的两条权值为`inf`的边
  - 然后最大流,,枚举删除点(两个)(因为这里有删除,所以dinic的bfs中条件加一个删除)


- 因为没有重新初始化错了(删点后的最大流需要初始化)

  ```c++
  for (int i = 2; i <= nume ; i++) {
          if (i % 2 == 0) {
              e[i].f += e[i^1].f;
          }else
              e[i].f = 0;
      }
  ```

- 因为空间分配小了错了两次

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

# USACO6.1 动态规划_剪枝_位01二叉树

从模板题到论文题...

## *vans

- 花了一个上午理解了解法,明天或者晚点再敲代码(会不会晚了一点= =)

- 参考[这篇博客](http://blog.csdn.net/jiangshibiao/article/details/21446033?utm_source=tuicool&utm_medium=referral)

- 基本意思就是把

  - 要求的f[i]定义为从第i行到第i-1行的哈密尔顿路

  - 把g[i]定义为从第i行到第i-4行的哈密尔顿路

  - 用一堆递推式子简化:)然后就得到一个互相递推的关系

    `f[i] = f[i-1] + g[i-1],g[i] = 2*f[i-1] + g[i-1] + g[i-2] - g[i-3]`

  - BTW看这个递推的证明看了一个小时..三四遍才看懂(真的想去补组合数学了...)

  - 然后因为直接去看题解去了,,,不会掉到高精的坑

- 另外这是一篇**雅礼师姐**的论文题...(插头DP......)

(权当练习了高精,不贴代码了)



## Rectangular

太久没有不看翻译/题解A题了..

兴奋的让人心疼= =

- 题意:

  ​给出一个最大为3000 * 3000 的矩形,中间有瑕点,求不包含瑕点的最大子矩形


- 思路

  1. 数据量不小,暴力枚举肯定过不了,需要找后效性的东西
  2. 设两个滚动数组`h[maxn] , w[maxn]`分别记录到这一个点最大的行数和列数
     - 状态转移: 如果有瑕点` h[r] = w[r] = 0;`否则`h[r]++,w[r] = w[r-1]+1`
  3. 然后往前逐步递减`h`更新ans
  4. 剪枝
     - 用vector存每一行的瑕点,每次处理一行之前sort一遍
     - 用`htry`存储测试的`h`值,当且仅当`htry*w[i]>ans`时候继续往下搜索
     - 更新`htry`**使用上一步的位置**
       - 因为上一步在某个位置停下是因为那个位置发生了转折
       - 所以`htry = h[pos-1]`,`pos`不变.能想到这里可能是因为我搞了一上午的回文串...

- 别人的思路(nocow)

  - 这是一道论文题= =`IOI2003王知昆`

  - 不用滚动数组会快很多,左右分别扫,而不是只往左扫

  - 把瑕点设为-inf后最大子矩阵...$O(n^3)$

  - 另一种最大子矩阵,两个min函数用的特别巧妙

    ```c++
    	for (int i=1;i<=n;i++)
        {
            int k=0; //记录连续几个空格
            for (int j=1;j<=m;j++)
                if (map[i][j]==false)
                {
                    h[j]=h[j]+1;
                    l[j]=min(l[j],++k);//保证上一层左边也有那么多
                } else 
                {
                    h[j]=0;
                    l[j]=0;
                    k=0;
                }
     
            k=0;
            for (int j=m;j>=1;j--)
                if (map[i][j]==false)
                {
                    r[j]=min(r[j],++k);//保证上一层右边也有那么多
                    ans=max(ans,h[j]*(l[j]+r[j]-1));
                } 
          	else
                {
                    r[j]=0;
                    k=0;
                }
        }
    ```

    ​


- 总结

  - 滚动数组的想法是发现**一层一层处理数据**之后想出来的

  - 思路不是特别清晰的时候可以试着敲一部分(暴力)代码

  - `now^=1,last^=1`是二维滚动数组的零一变换

  - ```c++
    //明显是卡时间过的代码
    Test 1: TEST OK [0.000 secs, 4240 KB]
       Test 2: TEST OK [0.000 secs, 4240 KB]
       Test 3: TEST OK [0.000 secs, 4240 KB]
       Test 4: TEST OK [0.000 secs, 4240 KB]
       Test 5: TEST OK [0.000 secs, 4240 KB]
       Test 6: TEST OK [0.022 secs, 4240 KB]
       Test 7: TEST OK [0.065 secs, 4240 KB]
       Test 8: TEST OK [0.594 secs, 4240 KB]
       Test 9: TEST OK [1.048 secs, 4300 KB]
       Test 10: TEST OK [0.929 secs, 4240 KB]
    /*
     ID: leidar11
     PROG: rectbarn
     LANG: C++11
     */
    #include <iostream>
    #include <cstring>
    #include <algorithm>
    #include <iostream>
    #include <vector>
    using namespace std;
    const int maxn = 3000 + 5;
    int R,C,P,a,b;
    vector<int> line[maxn];
    int h[maxn],w[maxn],mul[maxn][maxn];

    int main()
    {
        freopen("rectbarn.in","r",stdin);
        freopen("rectbarn.out","w",stdout);
        scanf("%d %d %d",&R,&C,&P);
        for (int i = 0; i < P; i++) {
            scanf("%d %d",&a,&b);
            line[a].push_back(b);
        }
        for (int i = 1; i < maxn; i++) {
            for (int j = i; j < maxn; j++) {
                mul[i][j] = mul[j][i] = mul[i][j-1] + i;
            }
        }
        int ans = 0;
        for (int l = 1; l <= R ; l++)
        {
            sort(line[l].begin(), line[l].end());//存储每一列的瑕点
            for (int r = 1,pos = 0; r <= C; r++) {
                if (pos == line[l].size() || r < line[l][pos]) {
                    h[r] = h[r]+1;
                    w[r] = w[r-1] + 1;//到这一个点最大的行数和列数
                    if (h[r] * w[r] > ans)
                    {
                        int htry = h[r],pos = r;
                        while (mul[htry][w[r]] > ans) {
                            while (pos > 0 && h[pos-1] >= htry) {
                                pos--;
                            }
                            ans = max(ans, mul[(r - pos + 1)][htry]);
                            htry = h[pos - 1];
                        }
                    }
                }//状态转移if
                else
                {
                    pos++;
                    h[r] = w[r] = 0;
                }
            }
        }
        printf("%d\n",ans);
        return 0;
    }
    ```


- 论文笔记

  - 在一个有障碍点的矩形中的最大子矩形一定是一个极大子矩形。

    > 算法1 $O(N^2M^2)$
    >
    > 的思路是通过枚举所有的极大子矩形找出最大子矩形。根据这个思路可以发现，如果算法中有一次枚举的子矩形不是有效子矩形、或者不是极大子矩形，那么可以肯定这个算法做了“无用功”，这也就是需要优化的地方。怎样保证每次枚举的都是极大子矩形呢，我们先从极大子矩形的特征入手。

  - 一个极大子矩形的四条边一定都不能向外扩展。更进一步地说，一个有效子矩形是极大子矩形的充要条件是这个子矩形的每条边要么覆盖了一个障碍点，要么与整个矩形的边界重合。

    > 算法二 $O(S^2)$ 
    >
    > 从左往右扫四个点确定一个矩形
    >
    > 再考虑边界情况

  - **有效竖线**：除了两个端点外，不覆盖任何障碍点的竖直线段。

    **悬线**：上端点覆盖了一个障碍点或达到整个矩形上端的有效竖线。

    如果将一个悬线向左右两个方向**尽可能**移动所得到的有效子矩形称为这个悬线所对应的子矩形，那么所有悬线所对应的有效子矩形的集合一定包含了所有极大子矩形的集合。

    > 算法三 $O(N*M)$
    >
    > 对于每个确定了底部的悬线，我们需要知道有关于它的三个量：顶部、左右最多能移动到的位置。
    >
    > 对于底部为(i,j)的悬线，
    >
    > 设它的高为hight[i,j],
    >
    > 左右最多能移动到的位置为left[i,j],right[i,j]。
    >
    > 为了充分利用以前得到的信息，我们将这三个函数用递推的形式给出。
    >
    > 如果点(i－1,j)为障碍点 : height[i,j] = 1,left[i,j] = 0,right[i,j] = m
    >
    > 否则: left[i,j] = max(left[i-1,j],左边第一个障碍点位置),height[i,j]=height[i-1,j]+1

  - 当NM很大,S很小,考虑对数据离散化处理

  - 最大权值子矩形

## *cowxor

给一段最长为100000的数组,

求最大一段连续子序列,使得xor和最大(序号,个数尽可能小)

限时0.5s,要求输出最大和,起始位置,结束位置



- 题外话

1. (话说这一题是旧版本的final test0.0)
2. 论文题: 武森

- 想了很久都是最坏复杂度为$O(n^2)$的算法

- 思路$O(nlogn)$

  - 异或的性质 : `a ^ b = c`那么这三个中任意两个异或都能得到剩下
  - 用`a[i]`记录0~i的数字的异或值,转化为求`max(a[r] - a[l-1]`
  - `a[i] = a[i-1] ^ nums[i]`
  - 再构造一个二叉搜索树,
    - 查询时,从第20位开始向下搜索尽量选择与当前位不同的子树
      - 比如左子树象征1的时候,那一位a[i]也恰好为1,尽可能搜右边
    - 每次查询完后更新树的结构,更新(覆盖子节点的值)

- 思路O(n)

  > 再维护一个s
  >
  > s[i]=k 表示离当前位最近的k-1，使得tot[k-1]的某个前缀为i。
  >
  > 对于tot[i]，从大到小枚举每一位j。
  >
  > 令q等于tot[i,j]的异或，则若q曾经出现过，那么解的第j位可以为1，否则只能为0。
  >
  > 重复上述过程就可以得到当前最优解t。 
  >
  > 然后用tot[i]的所有前缀更新s。 最后的最优解就是答案。

  ​大抵就是记录上一个那一位为0和1的位置

- 官方题解

```c++
runs a loop over all possible exponents e of 2 (starting from 21 downwards), updating arrays pop[] and best[]. It is assumed that after finishing the loop with exponent e (and before entering the loop, with e assumed to be 22), that for every 0 ≤ q ≤ N:
//这一段的意思大概是pop[q][0]存的是xr[q]在某一位的最大值的前驱
xr[pop[q][0]]'s and xr[q]'s binary representations are the same on positions e, e+1, etc., 
pop[q][0] is biggest possible with 0 ≤ pop[q][0] < q. 
If there's no such pop[q][0] possible, then pop[q][0] = -1. 
//后驱
xr[pop[q][1]]'s and xr[q]'s binary representations are the same on positions e+2, e+2, etc., different on position e, and pop[q][1] is biggest possible with 0 ≤ pop[q][1] < q. If there's no such pop[q][1] possible, then pop[q][1] = -1.
//找到最优
if X would be equal biggest possible xor, then xr[best[q]] xor xr[q]'s in equal X's binary representation on positions e, e+1, etc., and best[q] is biggest possible with 0 ≤ best[q] < q. If there's no such best[q] possible, then best[q] = -1.
//找到对应位置
After performing the loop with e = 0, for each 0 ≤ q ≤ N, best[q] = -1 or xr[best[q]] xor xr[q] is equal X. There exists at least one q with best[q] non-negative, because there exist such a and b, a ≤ b, that xr[a-1] xor xr[b] = X.
```

- ​第一个思路的代码(节点存储值会RE)

- ```c++
  #include <iostream>
  using namespace std;
  const int maxn = 100000 + 5;

  int N;
  int nums[maxn],a[maxn];

  struct node{
      int id;
      node* left;  //1
      node* right; //0
      node(){left = right = NULL;id = -1;};
  }root;

  int findMAX(int val)
  {
      int ans = 0;
      node * fd = &root;
      for (int pos = 20; pos >= 0; pos--) {
          int t = (val & (1 << pos)) >> pos;
          if (t) {
              if (fd->right != NULL) {
                  fd = fd->right;
                  ans += (val & (1 << pos));
              }else
                  fd = fd->left;
          }
          else
          {
              if (fd->left != NULL) {
                  fd = fd->left;
                  ans += (val & (1 << pos));
              }else
                  fd = fd->right;
          }
      }
      return fd->id;//返回左端点位置
  }
  void insert(int id,int val)
  {
      node * is = &root;
      for (int pos = 20; pos >= 0; pos--) {
          int t = (val & (1<<pos)) >> pos;
          if (t)
          {
              if (is->left == NULL)
              {
                  is->left = new node();
              }
              is = is->left;
          }
          else
          {
              if (is->right == NULL) {
                  is->right = new node();
              }
              is = is->right;
          }
      }
      is->id = id;
  }
  int main()
  {
      freopen("cowxor.in","r",stdin);
      freopen("cowxor.out","w",stdout);
      scanf("%d",&N);
      for (int i = 1; i <= N; i++) {
          scanf("%d",&nums[i]);
          a[i] = a[i-1] ^ nums[i];
      }
      int ans = 0, fr = 0, ansto = 1,to = 1;
      insert(0,a[0]);
      for (to = 1; to <= N; to++) {
          int temp = findMAX(a[to]);
          int temp1 = a[to] ^ a[temp];
          if (temp1 > ans) {
              ans = temp1;
              fr = temp;
              ansto = to;
          }
          insert(to, a[to]);
      }
      printf("%d %d %d\n",ans,fr+1,ansto);
      return 0;
  }
  ```

- 官方代码

- ```c++
  include <cstdio>

  #define MXN 101000
  using namespace std;

  int xr[MXN], prev[MXN][2], best[MXN];
  int a, wyk, res, n, x;
  int tmp[2];
  bool fnd;

  int main() {
      freopen("cowxor.in", "r", stdin);
      freopen("cowxor.out", "w", stdout);
      scanf("%i", &n);
      xr[0] = 0;
      for (a = 0; a < n; a++ ) {
          scanf("%d", &x);
          xr[a+1] = xr[a] ^ x;
      }
      for (a = 0; a <= n; a++ ) {
          prev[a][0] = a-1;
          prev[a][1] = -1;
          best[a] = a-1;
      }
      wyk = 22;
      res = 0;
      while (wyk--) {
          for (a = 1; a <= n; a++ )
              if (prev[a][0] == -1)
                  prev[a][1] = -1;
              else {
                  if (xr[a] >> wyk != xr[prev[a][0]] >> wyk) {
                      tmp[0] = prev[prev[a][0]][1];
                      tmp[1] = prev[a][0];
                  }//那一个数的wyk位有更新效果 
                	else {
                      tmp[0] = prev[a][0];
                      tmp[1] = prev[prev[a][0]][1];
                  }
                  prev[a][0] = tmp[0];
                  prev[a][1] = tmp[1];
              }
          fnd = false;
          for (a = 1; a <= n; a++ )
              if (0 <= best[a])
                  if ((xr[a] >> wyk) % 2 != (xr[best[a]] >> wyk) % 2 
                       || 0 <= prev[best[a]][1])
                      fnd = true;
          if (fnd) {
              res |= 1 << wyk;
              for (a = 1; a <= n; a++ )
                  if (0 <= best[a] &&
                      		(xr[a] >> wyk) % 2 == (xr[best[a]] >> wyk) % 2)
                      best[a] = prev[best[a]][1];
          }
      }
      for (a = 0; best[a] == -1; a++ );
      printf("%d %d %d\n", res, best[a]+1, a);
      return 0;
  }
  ```

- $(O(n))$[代码](http://www.nocow.cn/index.php/Code:USACO/cowxor/Pascal)(侵删...)

- ```pascal
  //tot[i]是上一个代码里面的a[i]
      a[1]:=1;
      for i:=2 to 21 do 
      a[i]:=a[i-1]shl 1+1;//shl 是pascal中左移运算符
      fillchar(s,sizeof(s),0);
      for i:=1 to n do
      begin
          x:=tot[i-1];
          for j:=0 to 20 do
          begin
              o:=(x and not a[j])shr j;
              s[a[21-j]+o]:=1;
          end;
          
          x:=0;now:=tot[i];
          for j:=20 downto 0 do
          begin
              x:=x+a[j]+1;
              l:=s[((x xor now)and not a[j])shr j+a[21-j]];
              if l>0 then
              begin
                 if (x>ans) 
                 then begin ans:=x;max:=i;
                 end;
              end 
              else dec(x,a[j]+1);//没看懂
          end;
      end;
   
      for j:=max downto 1 do 
      	if tot[j-1] xor tot[max]=ans 
      	then break;
      
      writeln(ans,' ',j,' ',max);
  close(input);
  close(output);
  end.
  ```

### 第三题论文笔记

浅谈信息学竞赛中的“0”和“1”																二进制思想在信息学竞赛中的应用										



- 数据结构

  - Matrix
  - 一维: 往两个点加一,查询sum
  - 二维: 每次往四个角(右下外围一步)的值加一,查询sum即可
  - 优化sum <= [树状数组](http://blog.csdn.net/uuuououlcz/article/details/43761057)

- 解题思路

  - 数独

  - 状态压缩,把每一个格子当做一个九位的二进制数

    > 这样就把每个格子表示成 0~511 中的一个数,这样每次搜索的时候,就直接枚举一个数字,通过位运算计算出这行、列以及 3*3 的块中是否可放即可									


- Requirements

  ​	十万个五维点的哈密尔顿距离最小

  转化为+-的二进制									


- Cow Xor(就是上文的O(nlogn)算法)										

# 最长回文子串O(n)_Manacher算法

$O(n^3)$暴力枚举..

$O(n^2)$算法:在某个点左右扫,分奇偶,就是校赛预选赛的一道题

> 那么以某个字符为中心的前缀和后缀都是相同的，例如以一段回文串“aba”为例，以b为中心，它的前缀和后缀都是相同的，都是a

(红书里面有模板~但..好像是错的...)

- Manacher算法

  1. 首先通过在每个字符的两边都插入一个特殊的符号，将所有可能的奇数或偶数长度的回文子串都转换成了奇数长度。比如 abba 变成 #a#b#b#a#， aba变成 #a#b#a#。

  2. 为了进一步减少编码的复杂度，可以在字符串的开始加入另一个特殊字符，这样就不用特殊处理越界问题，比如$#a#b#a#。

  3. 以字符串12212321为例，插入#和$这两个特殊符号，变成了 S[] = "$#1#2#2#1#2#3#2#1#"，然后用一个数组 P[i] 来记录以字符S[i]为中心的最长回文子串向左或向右扩张的长度（包括S[i]）。

     比如S和P的对应关系：

     - S # 1 # 2 # 2 # 1 # 2 # 3 # 2 # 1 #
     - P 1 2 1 2 5 2 1 4 1 2 1 6 1 2 1 2 1

  4. Manacher算法增加两个辅助变量id和mx，

     1. 其中id表示最大回文子串中心的位置，
     2. mx则为id+P[id]，也就是最大回文子串的边界。
     3. 如果mx > i，那么P[i] >= Min(P[2 * id - i], mx - i)**这里的证明参考资料2**

- [模板](http://blog.csdn.net/u013486414/article/details/47254435)题 poj3974

- ```c++
  #include <cstdio>
  #include <cstring>
  #define min(a,b) ((a<b)?a:b)
  #define max(a,b) ((a>b)?a:b)
  const int maxn = 2000000 + 200;
  char s[maxn];
  int p[maxn],len;

  void preprocess()
  {
      memset(p, 0, sizeof(p));
      len = (int)strlen(s);
      int i = len - 1,j = len << 1;
      s[j + 2] = '\0';
      s[j + 1] = '#';
      for (; i >= 0; i--) {
          s[j--] = s[i];
          s[j--] = '#';
      }
      s[0] = '@';//模拟新字符串的形成
  }
  int manacher()
  {
      preprocess();
      len = (int)strlen(s);//这里用 len = (len << 1) | 1会快很多
      int maxLen = 0,right = 0,cen = 0;
      for (int i = 1; i <= len;i++)
      {
          p[i] = i < right ? min(p[(cen<<1)-i], right - i) : 1;
        //如果mx > i，那么P[i] >= Min(P[2 * id - i], mx - i)
          while (s[i-p[i]] == s[i+p[i]]) 
            p[i]++;//原来统计区域外的部分
          if (right < i + p [i]) 
          {
            cen = i,right = i + p[i];
          }//更新最右边
        	maxLen = max(maxLen, p[i]);
      }
      return maxLen - 1;
  }
  ```

  ​

大段参考资料

1. [github_知乎](https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/01.05.md)

2. [参考资料](https://www.felix021.com/blog/read.php?2040)(英文参考资料里面推荐的"懂中文人,去看的最棒解释")

   ​

# USACO 6.2 最长回文串



## calfflac

其实一开始想$O(n^2)$的(数据差不多边界吧)

昨天才做了一个[最长回文串的模板](http://blog.csdn.net/qq_32209643/article/details/51603152)耶...

果然刷USACO没必要分心

这道题有一点点不同就是要输出源字符串..于是搞几个id对应过去就0s过了

果然这也是到论文题:	

许智磊(这个牛的论文好多...)

何林的论文<<求最长回文子串与最长重复子串>>**更大的牛..还是雅礼中学学长**

```c++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
const int maxn = 20000 + 5;
string temp, str;
char srt[maxn],s[maxn * 2];
int id[maxn],p[maxn * 2];
int manacher(int & midpos)
{
    int maxlen = 0,right = 0,cen = 0,len = (int)strlen(srt);
    int i = len - 1, j = len << 1;
    s[j + 2] = '\0',s[j+1] = '#';
    while (i >= 0) {
        s[j--] = srt[i];//这里一开始也写反了...
        s[j--] = '#';
        i--;
    }
    s[0] = '@';
    len = (len << 1) | 1;
    //len = strlen(s);
    for (i = 1; i <= len; i++) {
        p[i] = i < right ? min(p[(cen << 1) - i], right - i) : 1;
        while (s[i - p[i]] == s[i + p[i]])
        {
            p[i]++;
        }
        if (right < i + p[i]) {
            right = i + p[i];
            cen = i;
        }
        if (p[i] > maxlen) {
            maxlen = p[i] ;//这里赋值反了DE了半个小时bug
            midpos = i >> 1;
        }
    }
    //cerr << maxlen << endl;
    return maxlen - 1;
}
int main()
{
    freopen("calfflac.in","r",stdin);
    freopen("calfflac.out","w",stdout);
    getline(cin, str);
    while (getline(cin, temp))
    {
        str = str + '\n' + temp;
    }
    int i , pos;
    for (i = 0,pos = 0; i < str.length(); i++)
    {
        if (isalpha(str[i]))
        {
            srt[pos] = tolower(str[i]);
            id[pos++] = i;
        }
    }
    int len=0 , mid = 0,l,r;
    len = manacher(mid);
    printf("%d\n",len);
    if (len & 1) {
        l = mid - len / 2 - 1;
        r = mid + len / 2 - 1;
    }
    else
    {
        l = mid - len / 2;
        r = mid + len / 2 - 1;
    }//测试得边界
    for (int i = id[l]; i <= id[r]; i++) {
        printf("%c",str[i]);
    }
    printf("\n");
    return 0;
}
```

### 许智磊_后缀数组倍增算法_笔记

- **字符集** 一个字符集Σ是一个建立了全序关系的集合,也就是说,Σ中 的任意两个不同的元素 α 和 β 都可以比较大小,要么 α<β,要么 β<α(也就是 α>β)。字符集Σ中的元素称为字符。
- **后缀数组** 后缀数组 SA 是一个一维数组,它保存 1..n 的某个排列 SA[1],SA[2],...SA[n],并且保证 $Suffix(SA[i])<Suffix(SA[i+1])$   建立 S 的后缀数组(先在 S 的后面添加'$')
- **名次数组** 名次数组 Rank=SA-1,也就是说若 SA[i]=j,则 Rank[j]=i,不难 看出 Rank[i]保存的是 Suffix(i)在所有后缀中从小到大排列的“名次”。


- 对两个字符串 u,v 定义函数 lcp(u,v)=max{i | u=iv },也就是从头开始顺次比较 u 和 v 的对应字符,对应字符持续相等的最大位置,称为这两个字符串的**最长公共前缀**
- LCP(i,j)=min{LCP(k-1,k)|i+1≤k≤j} (LCP Theorem) => 对 i≤j<k,LCP(j,k)≥LCP(i,k)。
- 定义一维数组 height,令$ height[i]=LCP(i-1,i),1<i≤n,$并设 height[1]=0。
- 对于 i>1 且 Rank[i]>1,一定有 $h[i]≥h[i-1]-1$

#### 算法

1. 若 Rank[i]=1,则 h[i]=0。字符比较次数为 0。
2. 若 i=1 或者 h[i-1]≤1,则直接将 Suffix(i)和 Suffix(Rank[i]-1)从第一个字符开始依次比较直到有字符不相同,由此计算出 h[i]。字符比较次数为 h[i]+1,不超 过 h[i]-h[i-1]+2。
3. 否则,说明 i>1,Rank[i]>1,h[i-1]>1,根据性质 3,Suffix(i)和 Suffix(Rank[i]-1) 至少有前 h[i-1]-1 个字符是相同的,于是字符比较可以从 h[i-1]开始,直到某个 字符不相同,由此计算出 h[i]。字符比较次数为 h[i]-h[i-1]+2。
   1. 也就是说,整个算法的复杂度为 O(n)。				求出了 h 数组,根据关系式 height[i]=h[SA[i]]可以在 O(n)时间内求出 height数组,于是					可以在 O(n)时间内求出 height 数组。										

#### 例题

- `多模式串的模式匹配问题`		
  - 先用常数时间计算 LCP(mid,ans), 然后比较 LCP(mid,ans)和 max_match:
  - LCP(mid,ans)<max_match,则说明 Suffix(SA[mid])和 P 的最长公 共前缀就是 LCP(mid,ans),即直接可以确定 Step 3 中的 r=LCP(mid,ans),所以 可以直接比较两者的第 r+1 个字符
  - LCP(mid,ans)≥max_match,则说明 Suffix(SA[mid])和 Suffix(SA[ans]) 的前 max_match 个字符一定是相同的,于是 Suffix(SA[mid])和 P 的前 max_match 个字符也是相同的,于是比较两者的对应字符可以从第 max_match+1 个开始, 最后求出的 r 一定大于等于原先的 max_match,字符比较的次数为 r- max_match+1,不难看出 Step 3 执行过后 max_match 将等于 r。
- 最长回文串
  - 在 T 串的末尾添加一个特殊字符'#',规定它不等于 T 的任何一个字符,然 后把 T 串颠倒,接在'#'后,在 T'串后再添加特殊字符'$',规定它小于前面的任 何一个字符,拼接后形成的串称为 S 串。
    - max{r|Suffix(i)=r+1Suffix(i')},										

```c++
//参考代码
 void sorting(int j)//基数排序
 {
 	memset(sum,0,sizeof(sum));
 	for (int i=1; i<=s.size(); i++) sum[ rank[i+j] ]++;
 	for (int i=1; i<=maxlen; i++) sum[i]+=sum[i-1];
 	for (int i=s.size(); i>0; i--)
             tsa[ sum[ rank[i+j] ]-- ]=i;
        //对第二关键字计数排序，tsa代替sa为排名为i的后缀是tsa[i] 
 
 	memset(sum,0,sizeof(sum));
 	for (int i=1; i<=s.size(); i++) sum[ rank[i] ]++;
 	for (int i=1; i<=maxlen; i++) sum[i]+=sum[i-1];
 	for (int i=s.size(); i>0; i--)
             sa[ sum[ rank[ tsa[i] ] ]-- ]= tsa[i]; 
        //对第一关键字计数排序,构造互逆关系 
 }
 
 void get_sa()
 {
 	int p;
 	for (int i=0; i<s.size(); i++) trank[i+1]=s[i];
 	for (int i=1; i<=s.size(); i++) sum[ trank[i] ]++;
 	for (int i=1; i<=maxlen; i++) sum[i]+=sum[i-1];
 	for (int i=s.size(); i>0; i--) 
 		sa[ sum[ trank[i] ]-- ]=i;
 	rank[ sa[1] ]=1;
 	for (int i=2,p=1; i<=s.size(); i++)
 	{
 		if (trank[ sa[i] ]!=trank[ sa[i-1] ]) p++;
 		rank[ sa[i] ]=p;
 	}//第一次的sa与rank构造完成
 	for (int j=1; j<=s.size(); j*=2)
 	{
 		sorting(j);
 		trank[ sa[1] ]=1; p=1; //用trank代替rank 
 		for (int i=2; i<=s.size(); i++)
 		{
 			if ((rank[ sa[i] ]!=rank[ sa[i-1] ]) || (rank[ sa[i]+j ]!=rank[ sa[i-1]+j ])) p++;
 			trank[ sa[i] ]=p;//空间要开大一点，至少2倍
 		}
 		for (int i=1; i<=s.size(); i++) rank[i]=trank[i];
 	}
 }
```



# 2016百度之星总结帖

# [测试赛](http://bestcoder.hdu.edu.cn/contests/contest_show.php?cid=695)

选的2015资格赛的部分题目,第二题字符串处理,第三题map计数

## 1001 大搬家

>  f f (x) = x
>
>  两次置换后回到原位

dp a->b && b->a,考虑n个数

如果它独自成对，那么就有s[n-1]；

如果它与前面某个成对，首先可以有n-1个可取，然后每个都有s[n-2]；

故s[n] = s[n-1] + (n-1)s[n-2];

## 1004 放盘子



> 今天他向来访者们提出一个恶俗的游戏。他和来访者们轮流往一个正多边形内放盘子。最后放盘子的是获胜者，会赢得失败者的一个吻。玩了两次以后，小度熊发现来访者们都知道游戏的必胜策略。现在小度熊永远是先手，他想知道他是否能获胜。

脑洞题,看能不能再中间放一个盘子



# [资格赛](http://bestcoder.hdu.edu.cn/contests/contest_show.php?cid=690)



## 1001求字符串一段的hash

S[a,b] = S[b] - S[a-1]

但是这里是除法= =所以涉及乘法逆元

> 费马小定理:
>
> p is prime =>  x^(p-1) = 1(mod p)
>
> ```
> LL mod_pow(LL x, LL n) {
> LL res = 1;
> while(n) {
> if(n & 1) res = res * x % mod;
> x = x * x % mod;
> n >>= 1;
> }
> return res%mod;
> }
> ```
>
> 



据说可以用线段树做..



## 1002全1序列

>  度熊面前有一个全是由1构成的字符串，被称为全1序列。你可以合并任意相邻的两个1，从而形成一个新的序列。对于给定的一个全1序列，请计算根据以上方法，可以构成多少种不同的序列

高精度fib



## 1004统计一种无序集合出现次数

sort后丢到map统计



## 1003 字典树



增加功能: 前缀删除和存在该前缀即可

这道题让我发现以前理解的字典树是错误的..

字典树其实是一个节点数组 tr[节点数]\[26],而不是指针无限扩展= =

**插入单词时，到达一个节点，该节点的次数+1；删除单词时，先判断删除的这个前缀在当前字典树里是否存在，如果存在的话，得到这个单词结尾节点的次数m，然后用删除函数将路径上的每个节点都减去m。**

**所以解决方案是，当插入单词是，判断当前节点的次数是不是1，如果是1的话说明之前这个位置是断开的，就将当前节点之后所连接的节点次数清空。**



## 1005 闭包

> 小度熊是一个尽职尽责的程序熊，每天产出数千行的代码，代码当中存在很多判断条件。度熊想让自己代码中的这些条件不存在交集。为了简化问题，一个条件可以是一个『简单条件』或者是一个『复合条件』，简单条件由『变量』、『比较符』和『运算数』组成，其中『变量』是用小写字符表示的一串字符，『运算数』仅为整数，『运算符』包括：<、>、<=、>=、==。分别代表：小于、大于、小于等于、大于等于和等于关系。简单条件的格式固定，左边为变量名，中间为操作符，右边为数字。若干个『简单条件』中间通过英文逗号组成一个『复合条件』，各『简单条件』之间是逻辑与的关系，例如：
>
> 简单条件： a > 100
>
> 复合条件: duxiong < 1000 , a > 100

初次读题目特别像子臻老师编的那本题解里面的floyd判断闭包

但是这个题目是判断多个,而且有常数

这道题的思路是判断每个变量名的合法取值范围是否为空集

所以...就是一道模拟题?!!!...





# [2016"百度之星" - 初赛（Astar Round2A）](http://acm.hdu.edu.cn/search.php?field=problem&key=2016%26quot%3B%B0%D9%B6%C8%D6%AE%D0%C7%26quot%3B+-+%B3%F5%C8%FC%A3%A8Astar+Round2A%A3%A9&source=1&searchmode=source)

## 1001 AII X

> F(x,m) 代表一个全是由数字xx组成的mm位数字。请计算，以下式子是否成立：F(x,m) mod k ≡ c
>
> 1≤x≤9
>
> m\<= 10^{10}
>
> 0≤c<k≤10,000

显然m很大不能一个个枚举,

当然也不能十个十个枚举= =一开始直接交果然T

- 30ms解法 

  暴力ID数组对应:

  `双重数组就找个循环就好了. `

  理论上就是抽屉原理

- 15ms解法(把10^10扫一遍)

  数论

  m 对 k 取余, 再模拟循环m次.

  循环节位置是m%k

  就是xxx…x这样m个x，除以x之后就是m个1，乘9之后就是m个9，加1之后就是10m

  [证明](http://m.blog.csdn.net/article/details?id=51471639)  [求欧拉函数](http://blog.csdn.net/once_hnu/article/details/6302868)

  思路是转化除法为乘法,取一个更大的模

  `1111...111(k个1)0000...000(k个0) == 0 (mod k)`

  [证明网址](https://hk.answers.yahoo.com/question/index?qid=20110409135026AAv6fVR)(求看到这里的英语大大帮忙解释一下= =)

  >BTW CSDN百度抓取比谷歌还是要厉害一些的XD


- 0ms解法:

```c++
/*
双重快速幂,quickPow是x^m
calc中便是对于十进制(位数)的快速幂
偶数: F(x,m/2) * (10^(m/2)+1) 
奇数: F(x,m-1) * 10 + x
*/
inline int quickPow(LL times){
    int ret = 1, base = 10 % p;//base % p
    while(times){
        if(times & 1LL) ret = ret * base % p;
        base = base * base % p;
        times >>= 1;
    } return ret;
};

int calc(LL x){
    if(x == 1LL) return v;
    int ret = calc(x >> 1LL);
    ret = (ret * quickPow(x >> 1) + ret) % k;
    return (x & 1LL) ? ((ret * 10 + v) % k) : ret;
};
```

- 快速幂

  `f[0]=1f[i]=(f[i-1]*10+x)%k`加速递推式

## 1006 Gym Class



>假设最开始每个人有一个唯一的ID，从1到N，在排好队之后，每个同学会找出包括自己在内的前方所有同学的最小ID，作为自己评价这堂课的分数。
>
>有一些同学不希望某个（些）同学排在他（她）前面
>
>所有同学的评价分数最大和
>
>N M <= 100000(N不能再M后面)

一直TLE在优先队列的复杂设置,很简单的思路不能清晰下来当然跪倒底.

贪心,读入N,M的时候记录N的边数和M的度数**这时候不需要优先队列参与**

> 但是这里用queue存N的边会爆内存,需要用vector

然后优先队列直接用int,把能放的放进去,每次拿堆顶元素来更新d[边]

## 1002 Sitting in Line

给一组数据排序,使得相邻数乘积和最大

有一些数字固定.

N <= 16



第一次提交是DFS..写了好久真是手生,理所当然T

看到N<=16就觉得是状压DP..然而没写过QAQ

- 状态dp [i]\[j] ,

i为这会哪些数字已经被选（二进制表示）

​	用`__builtin_popcount(i)`得到有多少个i

j为最后一个选的数字是哪一个

dp [ i | (1<<k) ]\[k] = max( dp[i]\[j]+a[j]×a[k])

```c++
//第一个过的人的核心代码
for(int mask=0;mask<(1<<n)-1;mask++)
    for(int i=0;i<n;i++)
      	if(ok[mask][i])
    	{
          	int t=num[mask];
       	    if(at[t] != -1)
            {
              int j=at[t];
              if(mask&(1<<j))
                continue;
              dp[mask+(1<<j)][j]=max(dp[mask+(1<<j)][j],dp[mask][i]+s[i]*s[j]);
              ok[mask+(1<<j)][j]=true;
             }
             for(int j=0;j<n;j++)
             {
                if(mask&(1<<j))
                    continue;
                dp[mask+(1<<j)][j]=max(dp[mask+(1<<j)][j],dp[mask][i]+s[i]*s[j]);
                ok[mask+(1<<j)][j]=true;
             }
          }
```

## 1005 BD String



> 众所周知，度度熊喜欢的字符只有两个：B和D。
>
> 今天，它发明了一种用B和D组成字符串的规则：
>
> S(1)=BS(1)=B
>
> S(2)=BBDS(2)=BBD
>
> S(3)=BBDBBDDS(3)=BBDBBDD
>
> …
>
> S(n)=S(n-1)+B+reverse(flip(S(n-1))S(n)=S(n−1)+B+reverse(flip(S(n−1))
>
> 其中，reverse(s)reverse(s)指将字符串翻转，比如reverse(BBD)=DBBreverse(BBD)=DBB，flip(s)flip(s)指将字符串中的BB替换为DD，DD替换为BB，比如flip(BBD)=DDBflip(BBD)=DDB。
>
> 虽然度度熊平常只用它的电脑玩连连看，这丝毫不妨碍这台机器无与伦比的运算速度，目前它已经算出了S(2^{1000})S(21000)的内容，但度度熊毕竟只是只熊，一次读不完这么长的字符串。它现在想知道，这个字符串的第LL位（从1开始）到第RR位，含有的BB的个数是多少？
>
> (1≤L≤R≤10^18) 

找规律发现 S(n) = 2^n-1...

但是一方面不用高精度S(n)求不出来,另一方面需要直接构造剩下的区间

所以需要分开讨论,

- 在[这个题解](http://blog.csdn.net/zsgg_acm/article/details/51472043)中,分类讨论:

  - 如果L,R都在mid右边，那么可以把区间映射到mid左边，此时求B数量，相当于求D数量，因为flip操作。

  - 如果L<=mid && R >= mid，那么可以从mid左右两边取相等长度的区间，剩下的区间在考虑是在mid的左边还是右边，如果是右边的话，就要进行映射，换为求D的数量。

    所以整个算法只需要递归最多60层就可以得出解了。时间复杂度为log（R-L+1)**感觉是带标志变换的二分**

- [这个题解](http://blog.csdn.net/Miracle_ma/article/details/51472075)的思路是求出

  - T(R)的公式
    - 刚好 : f(k)
    - 多出来的一节 : 前面那节的D的个数(前面那节可以递归~)
  - T(R)-T(L-1)

另外,1003是dfs+线段树,1004是区间DP

# 初赛[RoundB](http://bestcoder.hdu.edu.cn/contests/contest_show.php?cid=702)





##  1003 瞬间移动



只能往 右且下 移动,求(1,1)到(n,m)的选择,mod 1000000007

2≤n,m≤100000

```c++
/*
 1    1    1    1    1    1       1       1       1
 1    2    3    4    5    6       7       8       9
 1    3    6    10    15    21      28      36      45
 1    4    10    20    35    56      84      120     165
 1    5    15    35    70    126     210     330     495
 1    6    21    56    126    252     462     792     1287
 1    7    28    84    210    462     924     1716    3003
 1    8    36    120    330    792     1716    3432    6435
 1    9    45    165    495    1287    3003    6435    12870
 */
```

打表,一眼看去是a[x]\[y] = a[x-1]\[y] + a[x]\[y-1]

继续找规律,发现a[x]\[y] = c(x + y - 4, max(x, y) - 2 );

然后用组合数公式算,算除法时候忘记除逆元就模了...Wa一次

居然以为是高精度,折腾了好久反应过来是逆元....就这样吧...858名憾别今年百度之星

高精度一直不取模算法上也可以,但是目测会超时...

## 1006 中位数计数 

给出一组数(n < 8000)求每个数是多少个区间的中位数

- 分析(暴力T):
  1. O(n)对每个数扫一遍数组,知道到第n位有多少个比它大多少个比它小
  2. O(n2)枚举区间, 通过上面数组求出比他大小的数各多少

     ​	当满足`big <= (len-1)/2&& sml <= (len-1)/2`计数器+1

- 看别人O(n2)的算法

  - 往左扫时候记录[l,i]l是第x小的数,cnt[i+x]++(统计一共有几个)

  - 往右扫的时候y += cnt[i+x];也就是加上对应位置.

  - ```c++
    int x=0;    cnt[n]++;            
    for (int j=1;i-j>0;j++)           
    {
      if (a[i-j]<a[i]) x--; else x++;              
      cnt[n+x]++;
    }            
    x=0;
    int y=cnt[n];    
    for (int j=1;i+j<=n;j++)            
    {
      if (a[i+j]<a[i]) x--; else x++;
      y+=cnt[n-x];
    }
    ```

    也就是说左右相比就可以了= =抓住主要矛盾.

## 1005区间交

> nn个非负整数的数列与mm个区间。每个区间可以表示为l_{i},r_{i}l​i​​,r​i​​。
>
> 它想选择其中kk个区间， 使得这些区间的交的那些位置所对应的数的和最大。

写了一个dfs..果断T

```c++
//优先队列,懒得写函数的FB少年读入变化
//sort后按照first先后排序
for (int i = 1; i <= m; i++) //到底i个区间
{      
  if (q.size() < k)//如果不够k个
  {
    q.push(-a[i].second);//把结尾丢进去(由于是负数在后面)
  }
  else if (-q.top() < a[i].second)//(k个)如果原来最先结尾先于现在结尾,更换
  {    
    q.pop();
    q.push(-a[i].second);        
  }
  if (q.size() == k) 
  {  
    ans = max(ans, s[-q.top()] - s[a[i].first - 1]);//到现在这一个区间的开头        
  }    
}
```



## 1001[区间的价值](http://bestcoder.hdu.edu.cn/contests/contest_showproblem.php?cid=702&pid=1001)

> 我们定义“区间的价值”为一段区间的最大值*最小值。
>
> 一个区间左端点在L，右端点在R，那么该区间的长度为(R-L+1)
>
> 现在聪明的杰西想要知道，对于长度为k的区间，最大价值的区间价值是多少。
>
> 当然，由于这个问题过于简单。
>
> 我们肯定得加强一下。
>
> 我们想要知道的是，对于长度为1∼n的区间，最大价值的区间价值分别是多少。
>
> 第一行一个数n(1≤n≤100000)。
>
> 第二行n个正整数(1≤ai≤109)，下标从1开始。
>
> 输出n行,每行一个最大乘积

第一直觉是线段树...

- [rmq](http://www.cnblogs.com/zxhl/p/5517112.html)


- 左右贪心扫

```c++
for (i=1;i<=n;i++)        
{                
  l = r = i;   
  minn=a[i];          
  ans[r-l+1]=max(ans[r-l+1],(ll)minn*a[i]);//初始化,假设a[i]最大值
            
  while(a[l-1]<=a[i]||a[r+1]<=a[i])
  {           
    while(l>1) 
      if (a[l-1]<=a[i]&&a[l-1]>=minn)//左边的可以更新最小值       
        l--,ans[r-l+1]=max(ans[r-l+1],(ll)minn*a[i]);    
      else break;
                
    while(r<n)              
      if (a[r+1]<=a[i]&&a[r+1]>=minn)      
        r++,ans[r-l+1]=max(ans[r-l+1],(ll)minn*a[i]); //右边的可以更新最小值                
      else break;
                
    if (a[l-1]<=a[i] && a[r+1]<=a[i])  //更新          
    {    
      if (a[l-1]>a[r+1])                 
        l--,minn=a[l],ans[r-l+1]=max(ans[r-l+1],(ll)minn*a[i]);             
      else                 
        r++,minn=a[r],ans[r-l+1]=max(ans[r-l+1],(ll)minn*a[i]);            
    }
             
    else if (a[l-1]<=a[i]) 
        l--,minn=a[l],ans[r-l+1]=max(ans[r-l+1],(ll)minn*a[i]);   
    else if (a[r+1]<=a[i]) 
        r++,minn=a[r],ans[r-l+1]=max(ans[r-l+1],(ll)minn*a[i]);
  }//end of while        
}//end of for
```





## 1004[货物运输](http://bestcoder.hdu.edu.cn/contests/contest_showproblem.php?cid=702&pid=1004)

> 公元2222年，l国发生了一场战争。
>
> 小Y负责领导工人运输物资。
>
> 其中有m种物资的运输方案，每种运输方案形如l_{i},r_{i}。表示存在一种货物从l_{i}运到r_{i}。
>
> 这里有n个城市，第i个城市与第i+1个城市相连（这里1号城市和n号城市并不相连），并且从i号城市走到i+1号或者从i+1号走到ii号需要耗费1点时间。 
>
> 由于高科技的存在，小Y想到了一种节省时间的好方案。在X号城市与Y号城市之间设立传送站，只要这么做，在X号城市走到Y号城市不需要耗费时间，同样的，从Y号城市走到X号城市也不需要耗费时间。
>
> 但是为了防止混乱，只能设立这么一条传送站。
>
> 现在这些运输方案同时进行，小Y想让最后到达目的地的运输方案时间最短。
>
> 第一行两个整数n,m(1≤n,m≤1000000)。
>
> 接下来mm行，每行两个整数l_{i},r_{i}(1≤li,ri≤n)
>
> t[a]\[a] = 0

这么明显的二分= =居然二逼了

诶看代码到一半发现就是求树的直径啊?[题解](http://blog.csdn.net/tc_to_top/article/details/51476095)

```c++
int solve(int tmp) {
     int right = Inf;
     int up = Inf;
     int down = -Inf;
     int left = -Inf;
     for (int i = 0; i < move.size(); ++i)
        if (move[i].r - move[i].l > tmp) {
            downdate(right, move[i].l + move[i].r + tmp);
            downdate(up, move[i].l - move[i].r + tmp);
            update(down, move[i].l - move[i].r - tmp);
            update(left, move[i].l + move[i].r - tmp);
            if (left > right || down > up) return 0;
        }
     return 1;
}
```





## 1002刷题计划



看上去就是一个背包啊..但是过的人实在太少了...

然后发现,是和(A)\*和(B),不是和(A*B)

> 摆在你面前的是n道题目，第 i(1≤i≤n) 道题目能提升ai 点智力值，代码量为 bi KB，无聊值为 ci ，求至少提升m点智力值的情况下，所做题目代码量之和*无聊值之和最小为多少。

然后过了的代码都是Point(x,y)搜索的..

找了一个宏定义少的代码贴过来

```c++
#include<algorithm>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include  <stdio.h>
#include   <math.h>
#include   <time.h>
#include   <vector>
#include   <bitset>
#include    <queue>
#include      <set>
#include      <map>
using namespace std;

typedef long long LL;

const int N=805;

struct Point
{
    int x,y;
};

int n,m,s,a[N],b[N],c[N];
LL Ans,val[N],v[N];
Point f[N];

Point get(LL x,LL y)
{
    for(int i=1;i<=s;i++)
        f[i].x=f[i].y=1e7,val[i]=(LL)1e7*(x+y);
    for(int j=1;j<=n;j++)
        v[j]=b[j]*x+y*c[j];
    for(int j=1;j<=n;j++)
        for(int k=min(m+a[j],s);k>=a[j];k--)
            if(val[k-a[j]]+v[j]<val[k])
                val[k]=val[k-a[j]]+v[j],f[k].x=f[k-a[j]].x+b[j],f[k].y=f[k-a[j]].y+c[j];
    Point c=f[m];
    for(int i=m+1;i<=s;i++)
        if(f[i].x*x+f[i].y*y<c.x*x+c.y*y)
            c=f[i];
    Ans=min(Ans,c.x*(LL)c.y);
    return c;
}

void dfs(Point a,Point b)
{
    Point c=get(a.y-b.y,b.x-a.x);
    if((c.x-a.x)*(LL)(b.y-a.y)-(c.y-a.y)*(LL)(b.x-a.x)>0)
        dfs(a,c),dfs(c,b);
}

int main()
{
    while(cin>>n>>m)
    {
        s=0;
        for(int i=1;i<=n;i++)
            scanf("%d%d%d",&a[i],&b[i],&c[i]),s+=a[i];
        Ans=1LL<<60;
        Point x=get(1,0),y=get(0,1);
        dfs(x,y);
        printf("%I64d\n",Ans);
    }
    return 0;
}
Close

```





# 总结

1. 学好贪心.
2. 打开脑洞(找规律)
3. 数论除法乘逆元
4. 之后才能聊一聊DP(就像省赛一样…挑战级别还没到DP)

## 头文件系列

百度之星最好的在于可以看到大神们的头文件长什么样...

- include

  ```c++
  #include<stdio.h>
  #include<cstring>
  #include<cstdlib>
  #include<cmath>
  #include<iostream>
  #include<algorithm>
  #include<vector>
  #include<map>
  #include<set>
  #include<queue>
  #include<bitset>
  #include<utility>
  #include<functional>
  #include<iomanip>
  #include<sstream>
  #include<ctime>
  #include<cassert>
  using namespace std;
  ```


- stdio读写

  ```c++
  #if ( ( _WIN32 || __WIN32__ ) && __cplusplus < 201103L)
      #define lld "%I64d"
  #else
      #define lld "%lld"
  #endif
  inline void gn(long long&x){
      int sg=1;char c;while(((c=getchar())<'0'||c>'9')&&c!='-');c=='-'?(sg=-1,x=0):(x=c-'0');
      while((c=getchar())>='0'&&c<='9')x=x*10+c-'0';x*=sg;
  }
  inline void gn(int&x){long long t;gn(t);x=t;}
  inline void gn(unsigned long long&x){long long t;gn(t);x=t;}
  inline void gn(double&x){double t;scanf("%lf",&t);x=t;}
  inline void gn(long double&x){double t;scanf("%lf",&t);x=t;}
  inline void gs(char *s){scanf("%s",s);}
  inline void gc(char &c){while((c=getchar())>126 || c<33);}
  inline void pc(char c){putchar(c);}
  ```


- [二进制处理](http://www.cnblogs.com/nysanier/archive/2011/04/19/2020778.html)

  ```c++
  #define cnti(x) (__builtin_popcount(x))//1的个数
  #define cntl(x) (__builtin_popcountll(x))
  #define clzi(x) (__builtin_clz(x))//左起第一个1前0的个数
  #define clzl(x) (__builtin_clzll(x))
  #define ctzi(x) (__builtin_ctz(x))//右起第一个1之后0的个数
  #define ctzl(x) (__builtin_ctzll(x))
  ```

- 常数

  ```c++
  typedef long long ll;
  typedef double db;
  typedef pair<int,int> pii;
  typedef vector<int> vi;
  typedef vector<pii> vpii;

  const int inf=0x3f3f3f3f;
  const db pi=3.14159265358979323846264338327950288L;
  const db eps=1e-6;
  const int mo=0;
  //另一个
  const int low(int x) { return x&-x; }
  const int INF = 0x7FFFFFFF;
  const int mod = 1e9 + 7;
  const int maxn = 1e5 + 10;
  ```

- 函数

  ```c++
  inline ll sqr(ll a){return a*a;}
  inline db sqrf(db a){return a*a;}

  #define fil(a,b) memset((a),(b),sizeof(a))
  #define cl(a) fil(a,0)
  #define siz(x) ((int)(x).size())
  #define all(x) (x).begin(),(x).end()
  #define foreach(it,a) for(__typeof((a).begin()) it=(a).begin();it!=(a).end();it++)

  #define rep(i,a,b) for (int i=(a),_ed=(b);i<_ed;i++)
  #define per(i,a,b) for (int i=(b)-1,_ed=(a);i>=_ed;i--)

  #define pw(x) ((ll(1))<<(x))
  #define upmo(a,b) (((a)=((a)+(b))%mo)<0?(a)+=mo:(a))
  #define mmo(a,b) (((a)=1ll*(a)*(b)%mo)<0?(a)+=mo:(a))

  void getre(){int x=0;printf("%d\n",1/x);}
  void gettle(){int res=1;while(1)res<<=1;printf("%d\n",res);}

  template<typename T,typename S>inline bool upmin(T&a,const S&b){return a>b?a=b,1:0;}
  template<typename T,typename S>inline bool upmax(T&a,const S&b){return a<b?a=b,1:0;}
  template<typename N,typename PN>inline N flo(N a,PN b){return a>=0?a/b:-((-a-1)/b)-1;}
  template<typename N,typename PN>inline N cei(N a,PN b){return a>0?(a-1)/b+1:-(-a/b);}
  template<typename N>N gcd(N a,N b){return b?gcd(b,a%b):a;}
  template<typename N>inline int sgn(N a){return a>0?1:(a<0?-1:0);}//返回参数的正负
  int qp(int a,ll b){int n=1;do{if(b&1)n=1ll*n*a%mo;a=1ll*a*a%mo;}while(b>>=1);return n;}

  ```

- 完整版

  ```c++
  #include<stdio.h>
  #include<cstring>
  #include<cstdlib>
  #include<cmath>
  #include<iostream>
  #include<algorithm>
  #include<vector>
  #include<map>
  #include<set>
  #include<queue>
  #include<bitset>
  #include<utility>
  #include<functional>
  #include<iomanip>
  #include<sstream>
  #include<ctime>
  #include<cassert>
  using namespace std;
  #define y0 y0z
  #define y1 y1z
  #define yn ynz
  #define j0 j0z
  #define j1 j1z
  #define jn jnz
  #define tm tmz
  #define buli(x) (__builtin_popcountll(x))
  #define bur0(x) (__builtin_ctzll(x))
  #define bul2(x) (63-__builtin_clzll(x))
  #define mp make_pair
  #define pb push_back
  #define fi first
  #define se second
  #define fil(a,b) memset((a),(b),sizeof(a))
  #define cl(a) fil(a,0)
  #define siz(x) ((int)(x).size())
  #define all(x) (x).begin(),(x).end()
  #define foreach(it,a) for(__typeof((a).begin()) it=(a).begin();it!=(a).end();it++)
  #define rep(i,a,b) for (int i=(a),_ed=(b);i<_ed;i++)
  #define per(i,a,b) for (int i=(b)-1,_ed=(a);i>=_ed;i--)
  #define pw(x) ((ll(1))<<(x))
  #define upmo(a,b) (((a)=((a)+(b))%mo)<0?(a)+=mo:(a))
  #define mmo(a,b) (((a)=1ll*(a)*(b)%mo)<0?(a)+=mo:(a))
  void getre(){int x=0;printf("%d\n",1/x);}
  void gettle(){int res=1;while(1)res<<=1;printf("%d\n",res);}
  typedef pair<int,int> pii;
  typedef vector<int> vi;
  typedef vector<pii> vpii;
  template<typename T,typename S>inline bool upmin(T&a,const S&b){return a>b?a=b,1:0;}
  template<typename T,typename S>inline bool upmax(T&a,const S&b){return a<b?a=b,1:0;}
  template<typename N,typename PN>inline N flo(N a,PN b){return a>=0?a/b:-((-a-1)/b)-1;}
  template<typename N,typename PN>inline N cei(N a,PN b){return a>0?(a-1)/b+1:-(-a/b);}
  template<typename N>N gcd(N a,N b){return b?gcd(b,a%b):a;}
  template<typename N>inline int sgn(N a){return a>0?1:(a<0?-1:0);}
  #if ( ( _WIN32 || __WIN32__ ) && __cplusplus < 201103L)
      #define lld "%I64d"
  #else
      #define lld "%lld"
  #endif
  inline void gn(long long&x){
      int sg=1;char c;while(((c=getchar())<'0'||c>'9')&&c!='-');c=='-'?(sg=-1,x=0):(x=c-'0');
      while((c=getchar())>='0'&&c<='9')x=x*10+c-'0';x*=sg;
  }

  inline void gn(int&x){long long t;gn(t);x=t;}
  inline void gn(unsigned long long&x){long long t;gn(t);x=t;}
  inline void gn(double&x){double t;scanf("%lf",&t);x=t;}
  inline void gn(long double&x){double t;scanf("%lf",&t);x=t;}
  inline void gs(char *s){scanf("%s",s);}
  inline void gc(char &c){while((c=getchar())>126 || c<33);}
  inline void pc(char c){putchar(c);}
  #ifdef JCVB
  #define debug(...) fprintf(stderr, __VA_ARGS__)
  #else
  #define debug(...) 
  #endif
  typedef long long ll;
  typedef double db;
  inline ll sqr(ll a){return a*a;}
  inline db sqrf(db a){return a*a;}
  const int inf=0x3f3f3f3f;
  const db pi=3.14159265358979323846264338327950288L;
  const db eps=1e-6;
  ```

  ​

- 另一个更全的

  ```c++
  /*
   * Package: StandardCodeLibrary.Core
   * */
  //引进常用的头文件并使用std名字空间;
  #include <iostream>
  #include <fstream>
  #include <sstream>
  #include <iomanip>
  #include <utility>
  #include <vector>
  #include <list>
  #include <string>
  #include <stack>
  #include <queue>
  #include <deque>
  #include <set>
  #include <map>
  #include <algorithm>
  #include <functional>
  #include <numeric>
  #include <bitset>
  #include <complex>
  #include <cstdio>
  #include <cstring>
  #include <cmath>
  #include <cstdlib>
  #include <ctime>
  #include <climits>
  using namespace std;

  //用于减少代码量的宏;
  #define lp for(;;)
  #define repf(i,a,b) for (int i=(a);i<(b);++i)
  #define ft(i,a,b) for (int i=(a);i<=(b);++i)
  #define fdt(i,a,b) for (int i=(a);i>=(b);--i)
  #define rrepf(i,a,b) fdt(i,(a)-1,b)
  #define rep(i,n) repf(i,0,n)
  #define rrep(i,n) rrepf(i,n,0)
  #define for_each(e,s) for (__typeof__((s).begin()) e=(s).begin();e!=(s).end();++e)
  #define for_nonempty_subsets(subset,set) for (int subset=set;subset;subset=(subset-1)&(set))
  #define for_in_charset(i,charset) for (cstr i=(charset);*i;i++)
  #define whl while
  #define rtn return
  #define fl(x,y) memset((x),char(y),sizeof(x))
  #define clr(x) fl(x,char(0))
  #define cpy(x,y) memcpy(x,y,sizeof(x))
  #define sf scanf
  #define pf printf
  #define vec vector
  #define pr pair
  #define que queue
  #define prq priority_queue
  #define itr iterator
  #define x first
  #define y second
  #define pb push_back
  #define mp make_pair
  #define ins insert
  #define ers erase
  #define lb lower_bound
  #define ub upper_bound
  #define rnk order_of_key
  #define sel find_by_order
  #define ctz __builtin_ctz
  #define clz __builtin_clz
  #define bc __builtin_popcount
  #define sz(x) (int((x).size()))
  #define all(x) (x).begin(),(x).end()
  #define srt(x) sort(all(x))
  #define uniq(x) srt(x),(x).erase(unique(all(x)),(x).end())
  #define rev(x) reverse(all(x))
  #define shf(x) random_shuffle(all(x))
  #define nxtp(x) next_permutation(all(x))

  //调试相关的宏;
  #ifndef DEBUG
  #define prt(x) (cerr)
  #define asrtWA(s) do if(!(s))exit(0);whl(0)
  #define asrtTLE(s) do if(!(s))whl(1);whl(0)
  #define asrtMLE(s) do if(!(s))whl(new int);whl(0)
  #define asrtOLE(s) do if(!(s))whl(1)puts("OLE");whl(0)
  #define asrtRE(s) do if(!(s))*(int*)0=0;whl(0)
  #define runtime() (cerr)
  #define input(in) freopen(in,"r",stdin)
  #define output(out) freopen(out,"w",stdout)
  #else
  #define prt(x) cerr<<"第"<<__LINE__<<"行\t: "<<#x"\t="<<(x)<<endl
  #define asrtWA(s) do if(!(s))cerr<<"assert("#s")"<<endl;whl(0)
  #define asrtTLE(s) do if(!(s))cerr<<"assert("#s")"<<endl;whl(0)
  #define asrtMLE(s) do if(!(s))cerr<<"assert("#s")"<<endl;whl(0)
  #define asrtOLE(s) do if(!(s))cerr<<"assert("#s")"<<endl;whl(0)
  #define asrtRE(s) do if(!(s))cerr<<"assert("#s")"<<endl;whl(0)
  #define runtime() cerr<<"Used: "<<db(clock())/CLOCKS_PER_SEC<<"s"<<endl
  #define input(in)
  #define output(out)
  #endif

  //常用数据类型;
  typedef long long int lli;
  typedef double db;
  typedef const char* cstr;
  typedef string str;
  typedef vec<int> vi;
  typedef vec<vi> vvi;
  typedef vec<lli> vl;
  typedef vec<vl> vvl;
  typedef vec<bool> vb;
  typedef vec<vb> vvb;
  typedef vec<char> vc;
  typedef vec<vc> vvc;
  typedef vec<str> vs;
  typedef pr<int,int> pii;
  typedef pr<lli,lli> pll;
  typedef pr<db,db> pdd;
  typedef vec<pii> vpii;
  typedef vec<pll> vpll;
  typedef vec<pdd> vpdd;
  typedef map<int,int> mii;
  typedef map<str,int> msi;
  typedef map<char,int> mci;
  typedef set<int> si;
  typedef set<str> ss;
  typedef que<int> qi;

  //常用常量:int的最大值;lli的最大值;db的误差相关常数;欧拉常数;圆周率;移动向量;取模使用的除数;
  int oo=(~0u)>>1;
  lli ooll=(~0ull)>>1;
  db inf=1e+10;
  db eps=1e-10;
  db gam=0.5772156649015328606;
  db pi=acos(-1.0);
  int dx[]={1,0,-1,0,1,-1,-1,1,0};
  int dy[]={0,1,0,-1,1,1,-1,-1,0};
  int MOD=1000000007;

  //常用函数:最大最小值更新;数学相关函数;输入和输出;树状数组;并查集;可合并堆;
  template<typename type>inline bool cmax(type& a,const type& b){rtn a<b?a=b,true:false;}
  template<typename type>inline bool cmin(type& a,const type& b){rtn b<a?a=b,true:false;}
  template<typename type>inline type sqr(const type& x){rtn x*x;}
  template<typename type>inline type mod(const type& x){rtn x%MOD;}
  inline int sgn(const db& x){rtn (x>+eps)-(x<-eps);}
  inline int dbcmp(const db& a,const db& b){rtn sgn(a-b);}
  template<typename type>inline pr<type,type> operator-(const pr<type,type>& x){rtn mp(-x.x,-x.y);}
  template<typename type>inline pr<type,type> operator+(const pr<type,type>& a,const pr<type,type>& b){rtn mp(a.x+b.x,a.y+b.y);}
  template<typename type>inline pr<type,type> operator-(const pr<type,type>& a,const pr<type,type>& b){rtn mp(a.x-b.x,a.y-b.y);}
  template<typename type>inline pr<type,type> operator*(const pr<type,type>& a,const type& b){rtn mp(a.x*b,a.y*b);}
  template<typename type>inline pr<type,type> operator/(const pr<type,type>& a,const type& b){rtn mp(a.x/b,a.y/b);}
  template<typename type>inline pr<type,type>& operator-=(pr<type,type>& a,const pr<type,type>& b){rtn a=a-b;}
  template<typename type>inline pr<type,type>& operator+=(pr<type,type>& a,const pr<type,type>& b){rtn a=a+b;}
  template<typename type>inline pr<type,type>& operator*=(pr<type,type>& a,const type& b){rtn a=a*b;}
  template<typename type>inline pr<type,type>& operator/=(pr<type,type>& a,const type& b){rtn a=a/b;}
  template<typename type>inline type cross(const pr<type,type>& a,const pr<type,type>& b){rtn a.x*b.y-a.y*b.x;}
  template<typename type>inline type dot(const pr<type,type>& a,const pr<type,type>& b){rtn a.x*b.x+a.y*b.y;}
  template<typename type>inline type gcd(type a,type b){if(b)whl((a%=b)&&(b%=a));rtn a+b;}
  template<typename type>inline type lcm(type a,type b){rtn a*b/gcd(a,b);}
  inline lli bin_pow(lli x,lli y){lli z=1;whl(y){if(y&1)z=mod(z*x);x=mod(sqr(x)),y>>=1;}rtn z;}
  template<typename istream,typename first_type,typename second_type>inline istream& operator>>(istream& cin,pr<first_type,second_type>& x){rtn cin>>x.x>>x.y;}
  template<typename ostream,typename first_type,typename second_type>inline ostream& operator<<(ostream& cout,const pr<first_type,second_type>& x){rtn cout<<x.x<<" "<<x.y;}
  template<typename istream,typename type>inline istream& operator>>(istream& cin,vec<type>& x){rep(i,sz(x))cin>>x[i];rtn cin;}
  template<typename ostream,typename type>inline ostream& operator<<(ostream& cout,const vec<type>& x){rep(i,sz(x))cout<<x[i]<<(i+1==sz(x)?"":" ");rtn cout;}
  inline ostream& pdb(int prcs,db x){rtn cout<<setprecision(prcs)<<fixed<<(sgn(x)?(x):0);}
  template<typename type>inline void bit_inc(vec<type>& st,int x,type inc){whl(x<sz(st))st[x]+=inc,x|=x+1;}
  template<typename type>inline type bit_sum(const vec<type>& st,int x){type s=0;whl(x>=0)s+=st[x],x=(x&(x+1))-1;rtn s;}
  template<typename type>inline type bit_kth(const vec<type>& st,int k){int x=0,y=0,z=0;whl((1<<(++y))<=sz(st));rrep(i,y){if((x+=1<<i)>sz(st)||z+st[x-1]>k)x-=1<<i;else z+=st[x-1];}rtn x;}
  inline void make_set(vi& st){rep(i,sz(st))st[i]=i;}
  inline int find_set(vi& st,int x){int y=x,z;whl(y!=st[y])y=st[y];whl(x!=st[x])z=st[x],st[x]=y,x=z;rtn y;}
  inline bool union_set(vi& st,int a,int b){a=find_set(st,a),b=find_set(st,b);rtn a!=b?st[a]=b,true:false;}
  inline void make_set(vpii& st){rep(i,sz(st))st[i]=mp(i,1);}
  inline int find_set(vpii& st,int x){int y=x,z;whl(y!=st[y].x)y=st[y].x;whl(x!=st[x].x)z=st[x].x,st[x].x=y,x=z;rtn y;}
  inline bool union_set(vpii& st,int a,int b){a=find_set(st,a),b=find_set(st,b);rtn a!=b?(st[a].y>st[b].y?st[a].x=b,st[a].y+=st[b].y:st[b].x=a,st[b].y+=st[a].y),true:false;}
  template<typename type>inline void merge(type& a,type& b){if(sz(a)<sz(b))swap(a,b);whl(sz(b))a.ins(*b.begin()),b.ers(b.begin());}

  //初始化;
  struct Initializer{
  #ifndef DEBUG
  Initializer(){ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);}
  #else
  ~Initializer(){runtime();}
  #endif
  }initializer;
  ```

  ​