#include<bits/stdc++.h>
#include"Datamaker.h"
using namespace std;

int n=20;
vector<int> res;
Tree<> A,B;

inline void Generate(int subid,int dataid){
    A=Tree<>(n);
    if(dataid==1){
        A.random1(0);
    }
    if(dataid==2){
        A.random2(0.5,0.75);
    }
    if(dataid==3){
        A.random3(5);
    }
    if(dataid==4){
        A.chain();
    }
    if(dataid==5){
        A.flower();
    }
    if(dataid==6){
        A.limchild(0,3);
    }
    if(dataid==7){
        A.complete(0,2);
    }
    if(dataid==8){
        A.silkworm(0);
    }
    if(dataid==9){
        A.star(0);
        A.redirect(1,0);
    }
    if(dataid==10){
        A.chain(0);
        A.addpoint(rnd.rand(0,A.size()-1));
    }
    if(dataid==11){
        B=Tree<>(n);
        A.chain(0),B.flower(0);
        A.merge(B,0,0);
    }
    if(dataid==12){
        A=Tree<>(4),B=Tree<>(5);
        A.chain(0),B.flower(0);
        A.expand(B,0,0);
    }
    A.shuffle();
    printf("%ld\n",A.size());
    for(int i=0;i<A.size()-1;++i)
        printf("%d %d\n",A[i].u+1,A[i].v+1);
}

int main(int argc,char *argv[]){
    registerGen(argc,argv);
    autoGenerate("tree",{12},Generate,"","");
    return 0;
}