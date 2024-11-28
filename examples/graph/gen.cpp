#include<bits/stdc++.h>
#include"Datamaker.h"
using namespace std;

int n=15,m=25;
Graph<> G;

map< pair<int,int>,bool > mp;

inline void Gener(int sbid,int dtid){
    G=Graph<>(n,m);
    if(dtid==1){
        G=Graph<>(n,15);
        G.random1(0);
    }
    if(dtid==2){
        G=Graph<>(n,15);
        G.random2(0);
    }
    if(dtid==3){
        G.DAG({-1});
    }
    if(dtid==4){
        G=Graph<>(5,0);
        G.complete();
    }
    if(dtid==5){
        vector<int> vec{3,2,2,2,2,2,1,1,1,1,2,2,2,2,1,1,0};
        G.limdegree1(vec,1);
    }
    if(dtid==6){
        vector<int> vec{3,3,4,4,4,6,6,3,3,2,4,3,3,1,1};
        G.limdegree2(vec);
    }
    if(dtid==7){
        G.circle();
    }
    if(dtid==8){
        G.bipartite(8);
    }
    if(dtid==9){
        vector<int> tmp={};
        for(int i=0;i<n;++i) tmp.emplace_back(i);
        G.DAG(tmp);
        G.redirect(0,0);
    }
    if(dtid==10){
        G.circle();
        Graph<> G2(n,m);
        G2.circle();
        G.merge(G2,{Edge<>(0,0,0)});
    }
    G.shuffle();
    int N=G.size().first,M=G.size().second;
    printf("%d %d\n",N,M);
    for(int i=0;i<M;++i)
        printf("%d %d\n",G[i].u+1,G[i].v+1);
}

int main(int argc,char *argv[]){
    registerGen(argc,argv);
    autoGenerate("graph",{10},Gener,"","");
    return 0;
}