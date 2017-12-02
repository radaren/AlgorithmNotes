//欧拉函数phi(m)
//0...m-1中与m互素的个数
//如果gcd(a,m) == 1,${a^phi(m)} == 1 mod m$

int Phi(int n) // O( Sqrt(N) ) 
{
	int i,j,ret=n;
	for(i=2,j=4; j<=n; i++,j+=i+i−1)
	    if (!(n%i)) 
	    {
			ret = ret / i ∗ (i−1);//phi(a) = a II(1-1/pi)
			while( !(n%i) ) n/=i; 
		}
	if( n>1 ) ret = ret / n ∗ (n−1);//如果还剩一个素数
	return ret ;
}

#define maxn 10000000 
#define maxp 1000000
int phi [maxn] , prime [maxp] , pnum;
void GenPhi(int n) // O( N loglog N )
{
	int i, j, k; 
	pnum = 0;
	memset(phi,0,(n+1)*sizeof(phi[0]));
	for (int i = 2; i <= n; ++i)
	{
		if (!phi[i])
		{
			prime[pnum++] = i;
			for (int j = i; j <= n ; j += i)
			{
				if (!phi[j])phi[j] = j;
				phi[j] = phi[j] / i * (i-1);
			}
		}
	}
}

//快速幂
int pmod(int a,int p, int m)
{
	if (p == 0) return 1 % m;
	long long ret = 1, remain = a;
	while(p > 0)
	{
		if (p & 1) ret = ret * remain % m;
		remain = remain * remain % m;
		p = p >> 1;
	}
	return ret;
}

//3.3LCA 高效查询树上两个点中点
const int maxD = 15;
int A[10001][maxD],d[10001];
void dfs(int root, int deep, int p){
	d[root] = deep;
	A[root][0] = p;//记录父亲
	for(int i = 1; i < 15; i++)
		A[root][i] = A[A[root][i-1]][i-1];//记录root所有祖先
	for(int i = 0;i < g[root].size();++i)
		if(!d[g[root][i]])
			dfs(g[root][i],deep+1,root);
}//O(NlogN)预处理

int Ancestor(int x, int len)
{
	for(int i = maxD - 1;i >= 0; i--)
		if(len >= 1 << i)//递归查询
		{
			len -= 1 << i;
			x = A[x][i];
		}
	return x;
}//查询与x相距为len的祖先

int LCA(int x, int y){
	if(d[x] > d[y])//优先提升深度较大的节点
		x = Ancestor(x, d[x] - d[y]);
	else
		y = Ancestor(y, d[y] - d[x]);
	if(x == y) return x;
	int ret;
	int i = maxD;
	while(i --){
		if (A[x][i] == A[y][i])
			ret = A[x][i];
		else
			x = A[x][i],y = A[y][i];
	}
	return ret;
}

void Answer(int x,int y){
	int p = LCA(x,y);
	int lenx = d[x] - d[p],leny = d[y]-d[p];
	int midlen = (lenx + leny) >> 1;
	int nx,it;
	if (lenx >= leny)
	{
		nx = Ancestor(x,midlen);//第一个中点
		if((lenx + leny) % 2){直接输出};
		else{ny = A[nx][0];}//第二个中点
	}else{...}//类似
}

//3.4 非递归欧拉回路,德布鲁序列
int a[1<<20];//顶点状态
stack<int> s;//深度优先搜索
string& dfs(int k){
	string * ret = new string();
	memset(a,0,sizeof(a));
	s.push(0)//从000.000k个0开始搜索
	while(s.size()){
		int core = s.top() % (1<<k-1);//栈顶元素对应顶点
		if(a[core] == 0){
			a[core]++;			//vis
			s.push(core << 1);	//压入由0边出发的顶点
		}
		else if(a[core] == 1)//压入过0边
		{
			a[core]++;
			s.push((core << 1) + 1);
		}
		else//搜索完了..
		{
			if (s.size() > 1)//顶点不是第一个不输出
			{
				if(s.top() % 2 == 0) *ret += "0";
				else *ret += "1";  
				s.pop();
			}
		}
	}
	return *ret;
}

//4.1 树状DP
int t[101][2];//t[i][0]=>Ai,t[i][1]=>Bi
int f[101][2][2];//[i][a/b][0/1]
int v[101];
void dfs(int r,int fa){
	v[r] = fa;
	for(int i = 0;i < g[r].size();i++)
		if(!v[g[r][i]])
			dfs(g[r][i],r);
	for (int a = 0; a < 2; ++a)//被A或B占领
	{	
		f[r][a][0] = t[r][a] / 2;
		int b = 1 - a;
		int change = t[r][a] - t[r][a]/2;//将半合法解变成合法的最小代价
		for(int i = 0;i < g[r].size();i++)
			if (v[g[r][i]] == r)
			{
				int s = g[r][i];
				f[r][a][0] += min(f[s][a][0],f[s][b][1]);
				update(change, f[s][a][1] - min(f[s][a][0],f[s][b][1]));
			}
			f[r][a][1] = f[r][a][0] + change;
	}
}
//4.2 数论
int extended_gcd(int a,int b,int d,int &x,int &y)//ax + by =d;
{
	int t,g;
	if(b == 0){x = d/a; y = 0; g = a;}
	else{
		g = extended_gcd(b,a%b,d,x,y);
		t = x;x = y;y = t - (a/b)*y;
	}
	return g;
} 
//4.3 二分图删点,匹配
int path(int u)//从u开始增广路
{
	find the unvisit neighbor v
	if (!cy[v] || path(cy[v]))
	{
		cx[u] = v;
		cy[v] = u;
		return 1;//存在增广路
	}
}
int match()
{
	memset cx,cy;
	int ret = 0;
	for(int i = 1;i <= nx;i++)
		if (!cx[i])
		{
			memset vis;
			ret += path(i);
		}
	return ret;
}
int match2(int y)//delete y
{
	memset vis
	memcpy cx,cy
	vis[y] = 1;	  //删除
	int x = cy[y];//原来匹配节点
	cx[x] = cy[y] = 0;
	int ret = path(x);//看是否存在另一条路
	restore cx,cy
}