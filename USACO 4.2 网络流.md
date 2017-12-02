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

