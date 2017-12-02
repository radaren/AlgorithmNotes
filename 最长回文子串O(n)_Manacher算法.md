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

