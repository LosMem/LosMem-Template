#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// 点编号可为0..n（n为最大编号），容量和lim非负，要求s!=t且总流量不溢出ll。
// lim默认为LLONG_MAX；flow返回本轮新增流量，可在当前残量网络上重复调用。
// 确认已完整增广后，min_cut(s)[u]表示u是否位于最小割的源点侧。
// flow会修改网络；若要从头计算或更换源汇，需要重新建图。Dinic复杂度O(n^2m)。
// 单位容量图为O(m min(n^(2/3),sqrt(m)))；单位网络（如二分图匹配）为O(m sqrt(n))。
struct maxflow
{
	struct edge{int v,rev;ll w;};
	vector<vector<edge>>e;
	vector<int>d,cur,q;

	maxflow(int n):e(n+1),d(n+1),cur(n+1),q(n+1){}
	void add_edge(int u,int v,ll w)
	{
		int a=e[u].size(),b=e[v].size();b+=u==v;
		e[u].push_back({v,b,w});e[v].push_back({u,a,0});
	}
	void bfs(int s)
	{
		fill(d.begin(),d.end(),-1);
		int l=0,r=0;d[q[r++]=s]=0;
		while(l<r)
		{
			int u=q[l++];
			for(auto& x:e[u])
				if(x.w&&d[x.v]==-1)
					d[q[r++]=x.v]=d[u]+1;
		}
	}
	ll dfs(int u,int t,ll fw)
	{
		ll k,sm=0;
		if(u==t)return fw;
		for(int& i=cur[u];i<e[u].size();++i)
		{
			edge& x=e[u][i];
			if(x.w&&d[x.v]==d[u]+1&&(k=dfs(x.v,t,min(x.w,fw-sm))))
			{
				x.w-=k;e[x.v][x.rev].w+=k;sm+=k;
				if(sm==fw)break;
			}
		}
		if(!sm)d[u]=-1;
		return sm;
	}
	ll flow(int s,int t,ll lim=LLONG_MAX)
	{
		ll ans=0;
		while(ans<lim)
		{
			bfs(s);
			if(d[t]==-1)break;
			fill(cur.begin(),cur.end(),0);
			ans+=dfs(s,t,lim-ans);
		}
		return ans;
	}
	vector<bool> min_cut(int s)
	{
		bfs(s);
		vector<bool>vis(e.size());
		for(int i=0;i<(int)e.size();++i)
			vis[i]=d[i]!=-1;
		return vis;
	}
};
