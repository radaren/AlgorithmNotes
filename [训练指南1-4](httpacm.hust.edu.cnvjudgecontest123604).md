[训练指南1-4](http://acm.hust.edu.cn/vjudge/contest/123604)

## HASH

UVA10125 hash+中途相遇 求最大a[d] 使得a[d] = a[a] + a[b] + a[c] 

```c++
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 1024;
const int mod = 1<<18;
int x[MAXN],head[mod],len;
/* 存取sum的hash值
** 因为a,b不能重复,所以同样需要记录
*/
struct data{
    int a,b,sum,next;
}data[MAXN*MAXN];

inline int gethash(int x)
{
    if(x < 0) x = -x;
    return x & (mod - 1);
}
inline void insert(int a,int b,int sum)
{
    int cur = gethash(sum);
    for (int i = head[cur]; i != -1; i = data[i].next) {
        if(data[i].sum == sum && data[i].a == a && data[i].b == b)return;
    }
    data[len].a = a;
    data[len].b = b;
    data[len].sum = sum;
    data[len].next = head[cur] ;
    head[cur] = len++;
}
inline bool search(int a,int b,int sum)
{
    int cur = gethash(sum);
    for (int i = head[cur]; i != -1; i = data[i].next) {
        if (data[i].sum != sum || data[i].a == a || data[i].a == b
            || data[i].b == a||data[i].b == b) {
            continue;
        }
        return true;
    }
    return false;
}

int main()
{
    int n;
    while (scanf("%d",&n),n) {
        len = 0;
        memset(head, -1, sizeof(head));
        for (int i = 0; i < n; i++) {
            scanf("%d",&x[i]);
        }
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                insert(i, j, x[i] + x[j]);
            }
        }
        bool ok = false;
        int ans = -999999999;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    continue;
                }
                if (search(i, j, x[i]-x[j])) {
                    ok = true;
                    ans = max(ans,x[i]);
                    break;
                }
            }
        }
        if (ok) {
            printf("%d\n",ans);
        }else
        printf("no solution\n");
        
    }
}
```

UVA11054 问两个字符串拼接能否hash

set 40ms,BKD/DJB/内置hash 10ms

```c++
//BKD/DJB
const int mod = 1000003;
int HASH[mod + 10];

inline unsigned long BKDhash(const char * str)
{
    unsigned long seed = 131313,hash = 0;
    while(*str) hash = hash * seed + (*str++);
    return (hash&0x7FFFFFFF)%mod;
}
inline unsigned long DJBhash(const char *str)
{
    unsigned int sum = 0;
    while(*str) sum = (sum << 5) + (*str++);
    return (sum&0x7FFFFFFF)%mod;
}

int head[mod],nt[mod];
string str[mod];
int cnt;
void initial()
{
    cnt = 0;
    memset(head, -1, sizeof(head));
}
void add(string st){
    unsigned long c = BKDhash(st.c_str());
    str[cnt] = st;
    nt[cnt] = head[c];
    head[c] = cnt++;
}

bool canfind(string st){
    unsigned long c = BKDhash(st.c_str());
    for(int i = head[c];i != -1;i = nt[i]){
        if(str[i] == st)return true;
    }
    return false;
}
//内置
#include <functional>
using namespace std;
const int mod = 1000003;
int HASH[mod + 10];
hash<string> str_hash;

int head[mod],nt[mod];
string str[mod];
int cnt;
void initial()
{
    cnt = 0;
    memset(head, -1, sizeof(head));
}
void add(string st){
    unsigned long c = str_hash(st)%mod;
    str[cnt] = st;
    nt[cnt] = head[c];
    head[c] = cnt++;
}

bool canfind(string st){
    unsigned long c = str_hash(st)%mod;
    for(int i = head[c];i != -1;i = nt[i]){
        if(str[i] == st)return true;
    }
    return false;
}
```

