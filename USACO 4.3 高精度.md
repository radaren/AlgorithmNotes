

# USACO 4.3 高精度



## buylow

给出一个数组,求最长下降子序列以及个数



主要是个数..如果用通常的LIS是做不出来的.所以用`vector<pair<long, string> >cnt[maxn];`这样的一个vector来存每一个数所应该贡献的位置.并用M记录最大值(这里忘记用string了...)然后从后往前扫

> /*  cnt[i]是当前为i的长度的所有序偶
>  第一个是它的值    第二个是它的数量
>  每次扫到小于M[i-1]的时候更新cnt[i] 以及M[i] *

更新cnt[i]的时候判断cnt[i]中是否有相同的元素,有的话直接更新存在的元素(所以这里应该用map?)

后面几组数据需要使用高精度.

[nocow](http://www.nocow.cn/index.php/USACO/buylow)上的题解

> 对于第二问求最长下降序列的数量，可以通过求第一问的过程解决。设MaxCnt[i]为第i项为末尾中最长下降序列的个数。
>
> 对于所有的j(1≤j≤i-1)如果有(s[j]>s[i] 并且 MaxLength[j]+1>MaxLength[i])则MaxCnt[i]=MaxCnt[j]，否则如果(MaxLength[j]+1= =MaxLength[i])可利用加法原理，MaxCnt[i]=MaxCnt[i]+MaxCnt[j]。
>
> 考虑到题目中说的不能又重复的序列，我们可以增加一个域Next[i]表示大于i且离i最近的Next[i]使得第Next[i]个数与第i个数相同。如果不存在这样的数则Next[i]=0。这样我们在DP的时候如果出现Next[j]不为0且Next[j]<i可直接跳过。

### 优化技巧

1. 可以在给出的序列的末尾增加一个0，这样直接统计以最后一个0结尾的最长下降子序列即可。
2. 显然题目的规模要求我们用高精度。可以使用longint，8位8位的加以节省时间。
3. ```c++
   //nocow上奇妙的高精度
   /* our bignum structure will store 6 digits per struct */
   /* the numbers will also be stored in INCREASING significance */
   /* thus 86,376,576 is represented as 376576 -> 86 */
   typedef struct BIGNUM *bignum_t;
   struct BIGNUM
    {
     int val;
     bignum_t next;
    };

   int num[5000]; /* the numbers (prices) */
   int len[5000]; /* the length of the maximum sequence which ends here */
   int nlen; /* number of numbers */

   bignum_t cnt[5000]; /* the number of maximal sequences which end here */

   /* allocate a bignum_t */
   bignum_t get_big(void)
    { /* because the bignum structure is so small, we'll allocate them
         in blocks */
     static bignum_t block;
     static int size = 0;
     if (size == 0)
      { /* no more structures left, allocate a new block */
       block = (bignum_t)malloc(sizeof(*block)*128);
       size = 128;
      }
     size--;
     return block++;
    }

   /* initialize a bignum to this value */
   void init_big(bignum_t *num, int val)
    {
     *num = get_big(); /* allocate space */

     /* initialize */
     (*num)->val = val;
     (*num)->next = NULL;
    }

   /* add b to a (a <- a+b) */
   void add(bignum_t a, bignum_t b)
    {
     int c; /* carry */
     
     c = 0;
     while (b || c) /* while either there's more b, or there's a carry */
      {
       a->val += c; /* add the carry */
       if (b) a->val += b->val; /* add b, if there's more b */

       /* if a->val is too large, we need to carry */
       c = (a->val / 1000000);
       a->val = (a->val % 1000000);

       if (b) b = b->next;
       if (!a->next && (b || c))
        { /* allocate if we need to */
         a->next = get_big();
         a = a->next;

         /* initialize the allocated space to 0 */
         a->val = 0;
         a->next = NULL;
        } else a = a->next;
      }
    }

   /* output the number v to the FILE f. no leading spaces or trailing newline */
   void out_num(FILE *f, bignum_t v)
    {
     if (v->next)  /* not at first digits */
      {
       out_num(f, v->next); /* output the more significant digits */
       fprintf (f, "%06i", v->val); /* output these digits, 0 padding is necc */
      }
     else 
       fprintf (f, "%i", v->val); /* most significant digits, no 0-padding */
    }
   ```

## lgame

每个字符有个值

给一个字符串

给一个字典

求最大重新排列的值的和,可以多个相加



发现两个长度都是3->7..直接枚举就过了......

为了sort标记词组

```c++
		if (v > maxValue){
                ans.clear();
                maxValue = v;
                ans.push_back(three[i] + three[j] + '3');}
```



## race3

给一个有向好图(v<50,e<100),有起点和终点,找出所有有下列条件的点

1. 必须经过

2. 可以把图分成两块好图

   - 好图的定义:

     - 从任意一个路口都可以到达终点。
     - 终点不通往任何路口
     - 每一个路口都可以由起点到达。

     ​	

想了好久没想出算法..看了眼数据大小(v<50,e<100)直接暴力啊....

一开始对于好图的理解不对,所以就错了很多次,就是分割点不能引出来往左边的点

第一个枚举+SPFA

第二个枚举+并查集

话说..第二个居然不能枚举第一个的结果...



## 总结

先不要把问题想复杂..先思考暴力能不能做出来…没准就....