#include<bits/stdc++.h>
#define int long long
using namespace std;
#define MAXN 100010
int n,m;
vector<int> ver[MAXN],e[MAXN];
inline void add(int u,int v,int w){
	ver[u].push_back(v);
	e[u].push_back(w);
}
int s,t,cnt;
int dis[MAXN];
int vis[MAXN];
queue<int> q;
inline void spfa(){
	memset(dis,0x3f,sizeof dis);
	dis[s]=0;
	q.push(s);
	vis[s]=1;
	int u;
	while(!q.empty()){
		u=q.front();q.pop();
		for(int i=0,v;i<ver[u].size();i++){
			if(dis[v=ver[u][i]]<=dis[u]+e[u][i]) continue;
			dis[v]=dis[u]+e[u][i];
			if(!vis[v])
				q.push(v),vis[v]=1,++cnt;
		}
		vis[u]=0;
	}
}
signed main(){
	freopen("hackspfa.in","r",stdin);
	scanf("%lld%lld%lld",&n,&m,&s);
	for(int u,v,w,i=1;i<=m;i++){
		scanf("%lld%lld%lld",&u,&v,&w);
		add(u,v,w);
		add(v,u,w);
	}
	spfa();
	printf("times : %lld\n",cnt);
	return 0;
}
