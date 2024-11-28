#include<bits/stdc++.h>
#include"Datamaker.h"
using namespace std;

const int max_n=100005,up=1e9;

int n,a[max_n];

inline void Generate(int subid,int dataid){
    if(subid==1){
        n=5;
        for(int i=1;i<=n;++i)
            a[i]=rnd.rand(1,up);
    }
    if(subid==2){
        n=20;
        vector<int> res=rnd.permutation(1,20);
        for(int i=1;i<=n;++i)
            a[i]=res[i-1];
    }
    if(subid==3){
        n=50;
        vector<int> res=rnd.distinct(n,1,100);
        for(int i=1;i<=n;++i)
            a[i]=res[i-1];
    }
    if(subid==4){
        n=100;
        vector<int> res=rnd.partition(n,up,100000);
        for(int i=1;i<=n;++i)
            a[i]=res[i-1];
    }
    printf("%d\n",n);
    for(int i=1;i<=n;++i)
        printf("%d ",a[i]);
    printf("\n");
}

int main(int argc,char *argv[]){
    registerGen(argc,argv);
    autoGenerate("tree",{2,2,3,3},Generate,"std","val");
    return 0;
}