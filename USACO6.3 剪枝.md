# USACO6.3 剪枝_hash

从论文题到抄答案题...

## *fence8

多重背包.

给出一堆木板要求不剩余的切成一定长度的小木板

问最多能满足多少小木板



写了个爆搜立马爆在第四个数据..然后开始学习

- 思路

- 剪枝方法汇总

  - 转载自[另一个CSDNblog](http://blog.csdn.net/damonhao/article/details/20290891)

    既然要装下尽可能多的物品，那么就应该先选入小的物品。

    所以，先把物品按照重量递增排序。那么： 

    1. 如果前k物品不能装入背包，那么即使把其中一个物品P换成k+1~R中的一个物品Q，

       由于Q的重量大于P，因此也绝对不可能成功装入背包。 

       1. 如果前k个物品可以装入背包，那么前k-1个物品也一定能装入背包。 
       2. 如果前k个物品不能装入背包，那么前k+1个物品也一定不能装入背包。 

       这样，一个最值问题就转化成了判定性问题，只要枚举出可行的最大k值就可以了。 

    2. 记所有背包的容量和为boardsum，前k个物品重量和为railsum[k]。 

       1. 重量大于最大背包容量的物品可以直接删去，容量小于最小物品重量的背包也可以直接删去。（这不算剪枝，应该是预处理） 
       2. 如果可以把前k个物品全部装入，那么浪费掉的容量waste一定是$boardsum - railsum[k]$。
       3. 当一个背包的剩余容量小于物品重量的最小值， 即不能装入任何物品时，把剩余的容量加到current_waste中，一旦$current_waste > waste$,就说明这种情况一定不能找到解，应该回溯（比较强大）。 

    3. 如果存在两个物品p1,p2的重量相等，因此可以把他们看作是一样的，所以可以直接固定p2的顺序，把它放在p1后面（非常强大)。 

    4. 如果$railsum[k]>boardsum$, 那么这k个物品一定不能放入背包。 

    5. 改变搜索顺序也可以加快速度，由于放入较大的物品是比较难的，因此dfs的时候可以从大的物品开始。 

  - 总结

    - 排序后便于有大小关系的剪枝
    - 统计最小n项和有利于剪枝
    - 搜索顺序从大到小
    - 相同元素尝试相同层次搜索


- 代码(来自[yali wx](http://www.nocow.cn/index.php/Code:USACO/fence8/C%2B%2B))

  ```c++

  #include <iostream>
  #include <cstring>
  #include <algorithm>
  #include <iostream>
  #include <string>
  #include <vector>
  using namespace std;

  int totB,board[50 + 5],cb[50 + 5];//存储原始木板长度
  int totN,need[1024],rail[1024];//存储小木板长度
  int mx = -1,sum = 0,mid,cost;
  void input()
  {
      freopen("fence8.in","r",stdin);
      freopen("fence8.out","w",stdout);
      scanf("%d",&totB);
      for (int i = 1; i <= totB; i++) {
          scanf("%d",&board[i]);
          sum += board[i];
      }//统计总长,便于剪枝
      sort(board+1, board + totB+1);
      scanf("%d",&totN);
      for (int i = 1; i <= totN; i++) 
          scanf("%d",&need[i]);
      sort(need + 1, need + totN + 1);
      while (totN > 0 && need[totN] > board[totB]) 
          totN--;//清理大于最大木板长度的木板
      for (int i = 1; i <= totN; i++) 
          rail[i] = rail[i-1] + need[i];//统计排序后,到第i个木板的长度和
  }
  /*
  搜索层次
  mid:  二分得到的最多木板数
  	=> rail[mid] 最小需要的长度和
  	=> sum - rail[mid]最多能浪费的量//剪枝
  step: 小木板位置
  	=> step == 0 成功搜索(允许剩余)
  pos:  最小大模板编号//剪枝所需
  */
  bool check(int step,int pos)
  {
      if (step < 1) 
          return true;//二分搜索,能否大于等于(允许剩余)
      if (cost > sum - rail[mid])
          return false;//剪枝: 如果残料过多
      /*从大到小搜(搜索顺序的优化)*/
      for (int i = totB; i >= pos; i--) {
          if (cb[i] >= need[step]) {//当前木板剩余足够,才会往下一层搜
              cb[i] -= need[step];
              if (cb[i] < need[1]) 
                  cost += cb[i];//当前木板已经不能满足最小的要求
             /*
             (搜索顺序优化)
             如果下一个小木板和当前大小一样,
             那么优先考虑最大为当前木板大小的情况
             */
              if (need[step] == need[step+1])
                  if (check(step -1, i)) 
                      return true;
              /*搜索下一个小木板*/
              else if (check(step -1, 1))
                  return true;
            	/*恢复原来情况*/
              if (cb[i] < need[1]) 
                  cost -= cb[i];
              cb[i] += need[step];
          }
      }
      return false;
  }
  void solve()
  {
      int l = 1,r = totN;
      mid = (l + r) >> 1;
      while (l <= r) {
          cost = 0;
          memcpy(cb, board, sizeof(board));
          if (check(mid , 0)) {
              l = mid + 1;
          }else
              r = mid - 1;
          mid = (l + r) >> 1;
      }//二分.注意不一定要求完全用完
  }
  int main()
  {
      input();
      solve();
      printf("%d\n",mid);
      return 0;
  }
  ```

  ​

## cryptcow

给定一个目标字符串(不含`COW`)

给定一个初始字符串,其中可能有COW,每一个COW交换CO和OW之间的字符串

求能否得到目标字符串= =



- 一开始觉得从两边搜`C W`然后搜`O`,立马被`CCCC...OOOO...WWWW`这组数据给坑了

- 然后上nocow发现有两种方法

  - 剪枝+hash

  - > 1. 由于添加的COW是一起的，因此给出的字符串的字符个数应该等于47（目标字符串的长度）+3*k。如果不满足就可直接判断无解。
    > 2. 除了COW三个字符外，其他的字符的个数应该和目标串相一致。如果不一致也可直接判断无解。
    > 3. 搜索中间肯定会出现很多相同的情况，因此需要开一个hash来记录搜索到过哪些字符串，每搜索到一个字符串，就判重。如果重复直接剪枝。这里的字符串的hash函数可以采用ELFhash，但由于ELFhash的数值太大，所以用函数值对一个大质数（我用的是99991）取余，这样可以避免hash开得太大，同时又可以减少冲突。
    > 4. 对搜索到的字符串，设不包含COW的最长前缀为n前缀（同样也可以定义n后缀），那么如果n前缀不等于目标串的长度相同的前缀，那么当前字符串一定无解，剪枝。N后缀也可采取相同的判断方法。
    > 5. 一个有解的字符串中，COW三个字母最早出现的应该是C，最后出现的应该是W，如果不满足则剪枝。
    > 6. **当前字符串中任意两个相邻的COW字母中间所夹的字符串一定在目标串中出现过。如果不符合可立即剪枝。**
    > 7. 需要优化搜索顺序。经过试验我们可以发现，O的位置对于整个COW至关重要。可以说，O的位置决定了整个串是否会有解。因此，我们在搜索时，应该先枚举O的位置，然后再枚举C和W的位置。其中W要倒序枚举。这样比依次枚举COW至少要快20~30倍。
    > 8. 在判断当前串的子串是否包含在目标串中的时候，可以先做一个预处理：记录每一个字母曾经出现过的位置，然后可以直接枚举子串的第一个字母的位置。这样比用pos要快2倍左右。

  - ```c++
    int elfhash(char * str) {
     
    	unsigned int res = 0,g;
    	while (*str)
    	{
    		res = (res << 4) + (*str++);
    	    g = res & 0xF0000000;
    		if (g) res ^= g >> 24;
    		res &= ~g;
    	}
    	return res % MAXH;
    }
    ```

  - ```c++
    把nocow上adventop牛的代码抄了一遍交上去
    http://www.nocow.cn/index.php/Code:USACO/cryptcow/C%2B%2B
    #include<fstream> //by adventop
    #include<cstring>
    #define BIG 1500000
    #define LEN 77
    using namespace std;
    const char End[] = "Begin the Escape execution at the Break of Dawn";
    ofstream fout ("cryptcow.out",ios::out);
    ifstream fin ("cryptcow.in",ios::in);
    bool Hash[BIG];
    long END=0,HASH;
    char start[LEN],fir[LEN],STR[LEN];
    //hash..不懂原理..就是编码吧
    inline unsigned long BKDRhash(const char *str){ //BKDRhash
        unsigned long seed=131313,hash=0;
        while(*str) hash=hash*seed+(*str++);
        return (hash & 0x7FFFFFFF) % 1000003;
    }
    //拷贝字符串,这里代码风格极佳
    //自己一开始尝试的时候没有封装成函数,没有拿cow作为变量名,只能调试输出
    inline void change(char *str,int c,int o,int w){
        int len=-1;
        memset(STR,0,sizeof(STR));
        for(int i=0;i<c;i++) STR[++len]=str[i];
        for(int i=o+1;i<w;i++) STR[++len]=str[i];
        for(int i=c+1;i<o;i++) STR[++len]=str[i];
        for(int i=w+1;i<strlen(str);i++) STR[++len]=str[i];
        strcpy(str,STR);
    }
    //剪枝函数
    inline bool dfscut(const char *str){
        int len=-1,Len,i,flag,Flag,lens=strlen(str);
      //首位不是C或W
        while(++len<lens && str[len]!='C') if(End[len]!=str[len]) return true;
        i=47;Len=len+1;len=lens;
        while(len>0 && str[len-1]!='W') if(End[--i]!=str[--len]) return true;
      //len - 1 位 最右'W'
      //Len 位 最左'C'
        lens=len;len=Len;Len=-1;
      //lens 最右'W'
      //len  最左'C'
      //Len  fir(拷贝)字符串的位置
        while(len<lens){
          //拷贝之间的字符串
            while(++len<lens && str[len]!='C' && str[len]!='O' && str[len]!='W')
                fir[++Len]=str[len];
            Flag=0;//能否找到对应的字符串
            if(len<=lens)
                for(int i=0;i<strlen(End)-Len;i++){
                    flag=1;
                    for(int j=i;j<=i+Len;j++)
                        if(fir[j-i]!=End[j]) {flag=0;break;}
                    if(flag) {Flag=1;break;}
                }
            if(!Flag) return true;
            Len=-1;
        }
        return false;
    }
    //搜索函数
    bool dfs(char *str,int step){
        char DFS[LEN];
        HASH=BKDRhash(str);
        if(Hash[HASH]) return false;
        else if(HASH==END) {fout<<"1 "<<step<<endl;return true;}
        Hash[HASH]=true;
        if(dfscut(str)) return false;
      //前面各种剪枝/排重
      //枚举O->CW搜索
        for(int o=0;o<strlen(str);o++)
            if(str[o]=='O')
            for(int c=0;c<o;c++)
                if(str[c]=='C')
                for(int w=strlen(str)-1;w>o;w--)
                    if(str[w]=='W'){
                        strcpy(DFS,str); //avoid to use dynamic point
                        change(DFS,c,o,w);
                        if(dfs(DFS,step+1)) return true;
                    }
        return false;
    }
    int main(){
        memset(Hash,0,sizeof(Hash));
        END=BKDRhash(End);
        fin.getline(start,LEN+2,'\n');//注意输入方式
        if(!((strlen(start)-47)%3) && !dfs(start,0)) fout<<"0 0"<<endl;
        return 0;
    }
    ```

  - 官方题解(代码也就近500行…不贴了)

  - > 大意大概是：C，O，W将新字符串分割为3k+1段。这3k+1段首先应能在原字符串里找到。然后
    >
    > **若原字符串第i段在新字符串中由C结尾，则原字符串第i + 1段必在新字符串里由O开头。**
    >
    > **若原字符串第i段在新字符串中由O结尾，则原字符串第i + 1段必在新字符串里由W开头。**
    >
    > **若原字符串第i段在新字符串中由W结尾，则原字符串第i + 1段必在新字符串里由C开头。**
    >
    > 例如
    >
    > CBegin the EscCution at the BreOape execWak of OWDawn
    >
    > 原字符串分为
    >
    > 1：Begin the Esc 
    >
    > 2：ape exec
    >
    > 3：ution at the Bre 
    >
    > 4：ak of 
    >
    > 5：Dawn, 
    >
    > 另外6，7两段为空段。
    >
    > 新字符串简化为
    >
    > C 1 C 3 o 2 W 4 o W 5（由于‘O'难分辨此处改为小写）
    >
    > 1由C结尾则2必由0开头，2由W结尾则3必由C开头.........
    >
    > 然后1结尾的C必和2开头的0是同一次操作的C，O，W。
    >
    > 而2结尾的W必和3开头的C是同一次操作。
    >
    > 而三开头 的C又是1结尾的C。
    >
    > 所以可以判断1结尾的C，2开头的0(3结尾的0），2结尾的W为一次操作....

## cowcycle

dfs模拟题,难看懂,模拟易出错

- 100*100的整数=>浮点数除法,打表优化时间
- 然后暴力枚举就过了.