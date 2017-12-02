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