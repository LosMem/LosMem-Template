#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

/*
原始对偶最小费用最大流：primal_dual f(n);f.add_edge(u,v,cap,fee);auto [fl,cost]=f.flow(s,t,lim);
点编号可为0..n（n为最大编号），容量和lim非负，要求s!=t；lim默认为LLONG_MAX。
flow返回本次新增的{流量,最小费用}；可用相同源汇在当前残量网络上继续调用。
允许负费用边，但输入网络不能有负费用环；费用和所有中间计算不能溢出ll，有限最短路须小于inf。
初始无负费用边时直接使用零势能，否则先用SPFA求势能；每轮Dijkstra后在零约化费用图上批量增广。
每轮Dijkstra为O(n+m log m)，批量增广使用Dinic；初始SPFA无多项式最坏界，空间O(n+m)。
*/
struct primal_dual
{
	static constexpr ll inf=LLONG_MAX/4;
	struct edge{int v,rev;ll w,c;};
	vector<vector<edge>>e;
	vector<ll>h,d;
	vector<int>dep,cur,q;
	vector<char>inq;
	bool neg=0;

	primal_dual(int n):e(n+1),h(n+1),d(n+1),dep(n+1),cur(n+1),q(n+1),inq(n+1){}
	void add_edge(int u,int v,ll w,ll c)
	{
		int a=(int)e[u].size(),b=(int)e[v].size();b+=u==v;
		e[u].push_back({v,b,w,c});e[v].push_back({u,a,0,-c});
		if(w&&c<0)neg=1;
	}
	void spfa(int s)
	{
		fill(h.begin(),h.end(),inf);
		queue<int>q;h[s]=0;q.push(s);
		while(q.size())
		{
			int u=q.front();q.pop();inq[u]=0;
			for(auto& x:e[u])
				if(x.w&&h[x.v]>h[u]+x.c)
				{
					h[x.v]=h[u]+x.c;
					if(!inq[x.v])inq[x.v]=1,q.push(x.v);
				}
		}
		for(auto& x:h)if(x==inf)x=0;
	}
	bool dij(int s,int t)
	{
		fill(d.begin(),d.end(),inf);
		priority_queue<pair<ll,int>>q;
		d[s]=0;q.push({0,s});
		while(q.size())
		{
			auto [du,u]=q.top();q.pop();
			if(-du!=d[u])continue;
			for(auto& x:e[u])
				if(x.w)
				{
					ll nd=d[u]+x.c+h[u]-h[x.v];
					if(nd<d[x.v])d[x.v]=nd,q.push({-nd,x.v});
				}
		}
		if(d[t]==inf)return 0;
		for(int i=0;i<(int)e.size();++i)
			if(d[i]!=inf)h[i]+=d[i];
		return 1;
	}
	bool bfs(int s,int t)
	{
		fill(dep.begin(),dep.end(),-1);
		int l=0,r=0;dep[q[r++]=s]=0;
		while(l<r)
		{
			int u=q[l++];
			for(auto& x:e[u])
				if(x.w&&dep[x.v]==-1&&x.c+h[u]-h[x.v]==0)
					dep[q[r++]=x.v]=dep[u]+1;
		}
		return dep[t]!=-1;
	}
	ll dfs(int u,int t,ll fw)
	{
		if(u==t)return fw;
		ll sm=0;
		for(int& i=cur[u];i<(int)e[u].size();++i)
		{
			auto& x=e[u][i];
			if(x.w&&dep[x.v]==dep[u]+1&&x.c+h[u]-h[x.v]==0)
			{
				ll k=dfs(x.v,t,min(x.w,fw-sm));
				if(k&&x.c>0)neg=1;
				x.w-=k;e[x.v][x.rev].w+=k;sm+=k;
				if(sm==fw)break;
			}
		}
		if(!sm)dep[u]=-1;
		return sm;
	}
	pair<ll,ll> flow(int s,int t,ll lim=LLONG_MAX)
	{
		ll fl=0,cost=0;
		if(!lim)return {0,0};
		if(neg)spfa(s);
		else fill(h.begin(),h.end(),0);
		while(fl<lim&&dij(s,t))
			while(fl<lim&&bfs(s,t))
			{
				fill(cur.begin(),cur.end(),0);
				ll w=dfs(s,t,lim-fl);
				fl+=w;cost+=w*h[t];
			}
		return {fl,cost};
	}
};
