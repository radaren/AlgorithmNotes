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

- 这道题毒性太重,第一次有想贴完整代码冲动

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

  ​

# range

​	递推 

```c++
if (len[row] == '1')
{
  dia[line][row] = dia[line-1][row-1]+1;
  lef[line][row] = lef[line][row-1]+1;
  up[line][row]  = up[line-1][row]+1;
  dia[line][row] = min(dia[line][row], lef[line][row]);
  dia[line][row] = min(dia[line][row], up[line][row]);
  for (int add = 2; add <= dia[line][row]; ++add)
  {
    cnt[add]++;
  }//这里的叠加可以放到输出时候再来一个递推的...
}
else
{
  dia[line][row] = lef[line][row] = up[line][row] = 0;
}

```



# game1

左右取数,博弈DP...

```c++
if ( (len & 1) != (Num & 1))//player1 makes dp max
{
  dp[begPos][endPos] = max(number[begPos] + dp[begPos+1][endPos],
                           number[endPos] + dp[begPos][endPos-1]);
}
else//player2 makes dp min
{
  dp[begPos][endPos] = min(dp[begPos+1][endPos] , dp[begPos][endPos-1]);
}
```

# *camlot

> SPFA(Shortest Path Faster Algorithm)
>
> 算法大致流程是用一个队列来进行维护。 初始时将源加入队列。 每次从队列中取出一个元素，并对所有与他相邻的点进行[松弛](http://www.nocow.cn/index.php/%E6%9D%BE%E5%BC%9B)，若某个相邻的点松弛成功，则将其入队。 直到队列为空时算法结束。
>
> 设Dist代表S到I点的当前最短距离，Fa代表S到I的当前最短路径中I点之前的一个点的编号。开始时Dist全部为+∞，只有Dist[S]=0，Fa全部为0。
>
> 维护一个队列，里面存放所有需要进行迭代的点。初始时队列中只有一个点S。用一个布尔数组记录每个点是否处在队列中。
>
> 每次迭代，取出队头的点v，依次枚举从v出发的边v->u，设边的长度为len，判断Dist[v]+len是否小于Dist[u]，若小于则改进Dist[u]，将Fa[u]记为v，并且由于S到u的最短距离变小了，有可能u可以改进其它的点，所以若u不在队列中，就将它放入队尾。这样一直迭代下去直到队列变空，也就是S到所有的最短距离都确定下来，结束算法。若一个点入队次数超过n，则有负权环。
>
> 优化算法 SLF 和 LLL： 
>
> SLF：Small Label First 策略，设要加入的节点是j，队首元素为i，若dist(j)<dist(i)，则将j插入队首，否则插入队尾。 LLL：Large Label Last 策略，设队首元素为i，队列中所有dist值的平均值为x，若dist(i)>x则将i插入到队尾，查找下一元素，直到找到某一i使得dist(i)<=x，则将i出对进行松弛操作。 SLF 可使速度提高 15 ~ 20%；
>
> 
>
> SLF + LLL 可提高约 50%。 在实际的应用中SPFA的算法时间效率不是很稳定，为了避免最坏情况的出现，通常使用效率更加稳定的Dijkstra算法。



题解:

```c++
fin >> maxC >> maxR;//注意顺序
    	
	for (r = 0; r < maxR; ++r)
    {
        for (c = 0; c < maxC; ++c)
        {
            cost[r][c][1] = kdis[r][c] = max(abs(r - x), abs(c - y));
        }
    }//king
    
    while (fin >> temp >> y)
    {
        x = temp - 'A' , y--;
        bfs(x, y);
        for (r = 0; r < maxR; ++r)
        {
            for (c = 0; c < maxC; ++c)
            {
                cost[r][c][0]+= dis[r][c][0];
                cost[r][c][1] = min(cost[r][c][1],dis[r][c][1] - dis[r][c][0]);
            }
        }
    }//knight

void bfs(int px, int py)
{
    memset(inq, 0, sizeof(inq));
    for (r = 0; r < maxR; ++r)
    {
        for (c = 0; c < maxC; ++c)
        {
            dis[r][c][0] = dis[r][c][1] = inf;
        }
    }//iniliatiaze
    
    dis[px][py][0] = 0;
    while (que.size())
    {
        que.pop();
    }
    
    que.push(Node(px,py,0));
    //用spfa优化
    /*
     对于每个骑士都需要初始化,不然会爆空间(存所有骑士)
     */
    while (que.size())
    {
        Node Now = que.front();
        que.pop();
        inq[Now.x][Now.y][Now.k] = false;
        int d = dis[Now.x][Now.y][Now.k];
        int k = Now.k;
        
        for (int dre = 0; dre < 8; ++dre)
        {
            int x = Now.x + kx[dre],y = Now.y + ky[dre];
            if (x < 0 || x >= maxR || y < 0 || y >= maxC) {
                continue;
            }
            if (dis[x][y][k] > d+1)
            {
                dis[x][y][k] = d+1;
                if (!inq[x][y][k])
                {
                    inq[x][y][k] = true;
                    que.push(Node(x,y,k));
                }
            }
        }
        
        if( !k && dis[Now.x][Now.y][1] > d + kdis[Now.x][Now.y])
        {
            dis[Now.x][Now.y][1] = d + kdis[Now.x][Now.y];
            if (!inq[Now.x][Now.y][1])
            {
                inq[Now.x][Now.y][1] = true;
                que.push(Node(Now.x,Now.y,1));
            }
        }//important:
        /*
         判断带上王的距离,也就是王走到那里的距离+d
         */
    }
}
```

