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



