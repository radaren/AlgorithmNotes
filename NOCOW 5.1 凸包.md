# NOCOW 5.1 凸包

>  类似把顶点放入多边形的题目通常是求凸包。如果题目要求一个面积最小的凸多边形，或者周长最小的凸多边形，那么我们几乎可以确定是要求凸包了。
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