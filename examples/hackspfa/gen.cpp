#include<bits/stdc++.h>
#include"Datamaker.h"
using namespace std;

int n,m;
Graph<> A;
vector<int> tmp;

int main(int argc,char *argv[]){
    registerGen(argc,argv);
    startTest("hackspfa.in");
    n=100000,m=200000;
    A=Graph<>(n,m);
    A.hackspfa(1,1000000000);
    tmp.resize(n);
    tmp=A.shuffle(1);
    printf("%d %d %d\n",n,m,tmp[0]+1);
    for(int i=0;i<m;++i)
        printf("%d %d %d\n",A[i].u+1,A[i].v+1,A[i].w);
    return 0;
}