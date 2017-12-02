# USACO 6.2 最长回文串_枚举_矩形切割_线段树



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

## packrec

给出四个矩形,求最小包围矩形...



枚举四个矩形

1. 横着竖着摆

2. 六种给定形状

   - 第六种复杂一些,把上面两个联立,下面两个联立后缩短

     ```c++
     void test(int l1,int l2,int l3)
     {
         int l4 = 6 - l1 - l2 - l3;
       	/*宽度是两个部分的最大宽度*/
         int w = max(vect[l1].w + vect[l3].w , vect[l2].w + vect[l4].l);
         /*分成两个的高度之和(没有任何压缩)*/
       	int l = max(vect[l1].l, vect[l3].l) + max(vect[l2].l, vect[l4].w);
         /*上面那个矩形比下面那个宽度小的时候 => 有压缩空间*/
         if (vect[l2].w <= vect[l1].w) {
             l -= max(0,min(vect[l2].l - vect[l4].w,vect[l3].l-vect[l1].l));
         }
         update(w, l);
     }
     ```

   - 好像题解里面有人模拟填色...


## 矩形切割

最大10000 * 10000 区域内不同颜色矩形掉落,

求每种颜色最后可见面积



- 思路一: 矩形切割(递归)

  - 贴一段来自[nocow](http://www.nocow.cn/index.php/Code:USACO/rect1/C%2B%2B)的代码

    ```c++
    /*
    ID: prime421
    LANG: C++
    TASK: rect1
    */
    #include <fstream>//来一个紧凑一点的代码，太长的容易吓走人
    #include <algorithm>//这里使用cstdio然后max和min手写可能能提高点速度
    using namespace std;
    ifstream fin("rect1.in");
    ofstream fout("rect1.out");
    int x1[1001],y1[1001],x2[1001],y2[1001],color[1001]={1},cnt[2501],N;//记录各种信息，cnt数组统计各种颜色的面积，第0号矩形就是地板
    void cover(int lx,int ly,int rx,int ry,int c,int h)//矩形切割上浮的思想，递归版本（啊说了这个代码特点是短而不是快- -），c是颜色，h是高度
    {
    	if (lx==rx || ly==ry) return;//优化：面积为0的矩形要舍掉，否则会TLE（第6个点就1.6秒了）
    	if (h>N) cnt[c]+=(ry-ly)*(rx-lx);//上浮到最高后计算面积，由于面积一定不会被覆盖所以直接加上去就好了
    	else{//对称的分为四块，嗯就是小学奥数时候见过的那个叫“弦图”的奇葩东西……不知道是不是都听说过
    		if (ly<y1[h]) cover(min(x2[h],lx),ly,min(x2[h],rx),min(y1[h],ry),c,h+1);
    		if (rx>x2[h]) cover(max(x2[h],lx),min(y2[h],ly),rx,min(y2[h],ry),c,h+1);
    		if (ry>y2[h]) cover(max(x1[h],lx),max(y2[h],ly),max(x1[h],rx),ry,c,h+1);
    		if (lx<x1[h]) cover(lx,max(y1[h],ly),min(x1[h],rx),max(y1[h],ry),c,h+1);
    	}//注意这里对矩形位置的讨论思想是：只要凸出来就“切”开，注意比较边界坐标来：1、确保切割不重复。2、确保不切不存在的内容
    }//例如如果矩形整个位于它“上面”矩形的左上角，直接判断会出错。注意这样子之后有可能切出来面积为0的矩形，必须剪枝！
    int main()
    {
    	fin>>x2[0]>>y2[0]>>N;//读入数据，直接存在地板那块矩形的数据里
    	for (int i=1;i<=N;++i) fin>>x1[i]>>y1[i]>>x2[i]>>y2[i]>>color[i];//读入数据读入数据……
    	cnt[color[N]]+=(x2[N]-x1[N])*(y2[N]-y1[N]);//最上面那块直接算
    	for (int i=N-1;i>=0;--i) cover(x1[i],y1[i],x2[i],y2[i],color[i],i+1);//依次向下处理每一块矩形
    	for (int i=1;i<=2500;++i) if (cnt[i]) fout<<i<<" "<<cnt[i]<<endl;//输出
    	return 0;
    }//加上全部注释和花括号31行= =至于速度，第8，9个点0.011sec，第11个点0.130sec，其余0.000sec过

    ```

    ​


- 思路二: 离散化+各种优化


- 论文: 薛矛
  - a,b,Left,Right 这 4 个域是描述一棵线段树所必须的 4 个量。根据实际需要, 我们可以增加其它的域,例如增加 Cover 域来计算该线段被覆盖的次数,bj 域用 来表示结点的修改标记(后面将会提到)等等。
  - 将一维线段树改成二维线段树,有两种方法。一种就是给原来线段树中的每 个结点都加多一棵线段树,即“树中有树”。
  - 另一种方式是直接将原来线段树结点中的线段变成矩形。即每个结点代表一个矩形。因此矩形树用的是四分的思想,每个矩形分割为 4 个子矩形。矩形(x1,y1,x2,y2)有4 个儿子
  - 矩形切割的思想:
    - 1、先对被切割矩形进行 x 方向上的切割。取(x1,x2),(x3,x4)的交集(k1,k2)
      - 1 若 x1<k1,则加入矩形(x1,y1,k1,y2)
      - 2 若 k2<x2,则加入矩形(k2,y1,x2,y2)
    -  2、再对切剩的矩形(k1,y1,k2,y2) 进行 y 方向上的切割。取(y1,y2),(y3,y4)的交集(k3,k4)
      - 1 若 y1<k3,则加入矩形(k1,y1,k2,k3)
      -  2 若 k4<y2,则加入矩形(k1,k4,k2,y2)
    - 3、把矩形(x1,y1,x2,y2)从矩形集合中删除。
  - 线段树时间复杂度较优,矩形切割空间复杂度较优