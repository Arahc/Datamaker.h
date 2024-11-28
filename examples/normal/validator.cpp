#include<bits/stdc++.h>
#define int long long
using namespace std;
bool Begin;
const int max_n=100005,mod=998244353;
inline int read(){
    int x=0;bool w=0;char c=getchar();
    while(!isdigit(c)){
        if(c==EOF) exit(1);
        w|=c=='-',c=getchar();
    }
    while(isdigit(c)) x=(x<<1)+(x<<3)+(c^48),c=getchar();
    return w?-x:x;
}
inline void write(int x){
    if(x<0) putchar('-'),x=-x;
    if(x>9) write(x/10);
    putchar(x%10^48);
}
inline int mi(int a,int p=mod-2){
    int res=1;
    while(p){
        if(a&1) res=res*a%mod;
        a=a*a%mod,p>>=1;
    }
    return res;
}

int n,a[max_n],f[max_n];

bool End;
#define File ""
signed main(){
    // #ifndef ONLINE_JUDGE
    // freopen(File ".in","r",stdin);
    // freopen(File ".out","w",stdout);
    // #endif
    // cerr<<"Memory : "<<(&Begin-&End)/1024.0/1024<<"\n";
    n=read();
    if(n<1 || n>100) return 1;
    for(int i=1;i<=n;++i){
        a[i]=read();
        if(a[i]<1 || a[i]>1000000000) return 1;
    }
    return 0;
}
