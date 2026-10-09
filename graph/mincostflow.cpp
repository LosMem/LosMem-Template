#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

/*
最小费用最大流：costflow f(n);f.add_edge(u,v,cap,cost);auto [fl,fee]=f.flow(s,t,lim);
点编号可为0..n（n为最大编号）；add_edge加入u->v、容量为cap、单位费用为cost的有向边。
flow在当前残量网络上发送至多lim流量，返回{本次新增流量,本次最小费用}；lim默认为LLONG_MAX。
容量和lim须非负，要求s!=t；允许负费用，但从s可达的残量网络不能有负费用环。
flow会修改残量网络，可用相同源汇继续调用；有效最短路须小于inf，且所有数值运算均不能溢出ll。
每轮用SPFA求最短路，再用当前弧DFS批量增广最短路网络；设SPFA轮数为k，最坏O(knm)，空间O(n+m)。
设本次最终流量为F，则k<=F+1，故最坏O(Fnm)；若lim=O(1)，则为O(nm)。
单位容量图中F<=m，最坏O(nm^2)；单位容量二分图匹配中F=O(n)，最坏O(n^2m)。
费用最短路网络不同于Dinic分层图，不能直接套用Dinic在单位网络上的复杂度。
*/
struct costflow
{
	static constexpr ll inf=LLONG_MAX/4;
	struct edge{int v,rev;ll w,c;};
	vector<vector<edge>>e;
	vector<ll>d;
	vector<int>cur;
	vector<char>inq;

	costflow(int n):e(n+1),d(n+1),cur(n+1),inq(n+1){}
	void add_edge(int u,int v,ll w,ll c)
	{
		int a=e[u].size(),b=e[v].size();b+=u==v;
		e[u].push_back({v,b,w,c});e[v].push_back({u,a,0,-c});
	}
	bool spfa(int s,int t)
	{
		fill(d.begin(),d.end(),inf);
		queue<int>q;d[s]=0;q.push(s);
		while(q.size())
		{
			int u=q.front();q.pop();inq[u]=0;
			for(auto& x:e[u])
				if(x.w&&d[x.v]>d[u]+x.c)
				{
					d[x.v]=d[u]+x.c;
					if(!inq[x.v])q.push(x.v),inq[x.v]=1;
				}
		}
		return d[t]!=inf;
	}
	ll dfs(int u,int t,ll fw)
	{
		if(u==t)return fw;
		ll sm=0;inq[u]=1;
		for(int& i=cur[u];i<(int)e[u].size();++i)
		{
			edge& x=e[u][i];
			if(x.w&&!inq[x.v]&&d[x.v]==d[u]+x.c)
			{
				ll k=dfs(x.v,t,min(x.w,fw-sm));
				x.w-=k;e[x.v][x.rev].w+=k;sm+=k;
				if(sm==fw)break;
			}
		}
		inq[u]=0;
		return sm;
	}
	pair<ll,ll> flow(int s,int t,ll lim=LLONG_MAX)
	{
		ll fl=0,cost=0;
		while(fl<lim&&spfa(s,t))
		{
			fill(cur.begin(),cur.end(),0);
			ll x=dfs(s,t,lim-fl);
			fl+=x;cost+=x*d[t];
		}
		return {fl,cost};
	}
};
