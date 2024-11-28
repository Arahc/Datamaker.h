#include<bits/stdc++.h>
#include"Datamaker.h"
using namespace std;

int n;

Polygon<int> P;

inline void Gener(int sbid,int dtid){
    n=(dtid+1)*5;
    P=Polygon<int>(n);
    if(sbid==1){
        P.random(-20,20);
    }
    else if(sbid==2){
        P.convex(-20,20);
    }
    for(int i=0;i<n;++i)
        printf("(%d,%d)\n",P[i].x,P[i].y);
}

int main(int argc,char *argv[]){
    registerGen(argc,argv);
    autoGenerate("polygon",{2,2},Gener,"","");
    return 0;
}