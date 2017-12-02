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

  - >  每条航线都是自西向东，本题可以转化为求航线图中从1到N两条不相交的路径，使得路径长度之和最大。转化为网络流模型，就是找两条最长的增广路。由于每个城市只能访问一次，要把城市拆成两个点，之间连接一条容量为1的边，费用设为1。因为要找两条路径，所以起始点和终点内部的边容量要设为2。那么费用流值-2就是两条路径长度之和，为什么减2，因为有两条容量为2的边多算了1的费用。求最大费用最大流后，如果(<1.a>,<1.b>)不是满流，那么我们找到的路径不够2条（可能是1条，也可能0条），所以无解。

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