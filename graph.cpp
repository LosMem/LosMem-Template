#include <bits/stdc++.h>

#define N 100005 //
#define M 500005 //

typedef long long ll;

using namespace std;

// 点编号为1..n，边权为非负int，最短路长度不能溢出ll。
// sol返回s到t的距离；调用后d[i]为s到i的最短距离，不可达为-1。
struct dijk
{
	vector<ll>d;
	vector<vector<pair<int,int>>>g;
	dijk(int n):d(n+1),g(n+1){}
	void adde(int u,int v,int w){g[u].push_back({v,w});}
	ll sol(int s,int t)
	{
		d.assign(d.size(),-1);
		priority_queue<pair<ll,int>>q;
		d[s]=0;q.emplace(0,s);
		while(q.size())
		{
			auto [du,u]=q.top();q.pop();
			if(-du!=d[u])continue;
			for(auto [v,w]:g[u])
				if(d[v]<0||d[v]>d[u]+w)
					q.emplace(-(d[v]=d[u]+w),v);
		}
		return d[t];
	}
};

// 点为1..n，容量和lim非负，要求s!=t且总流量不溢出ll。
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
		for(int i=1;i<(int)e.size();++i)
			vis[i]=d[i]!=-1;
		return vis;
	}
};

struct costflow
{
	ll d[N],ans;
	bool inq[N];
	int n,en,head[N],cur[N],q[N*10];
	const ll inf=0x3f3f3f3f3f3f3f3fll;
	struct edge{int u,v,w,c,nxt;}e[M*2];
	void init(int nn){n=nn;ans=0;en=1;for(int i=0;i<=n;++i)head[i]=0;}
	void _adde(int u,int v,int w,int c){e[++en]={u,v,w,c,head[u]};head[u]=en;}
	void adde(int u,int v,int w,int c){_adde(u,v,w,c);_adde(v,u,0,-c);}
	bool spfa(int s,int t)
	{
		int i,u,v,hd=1,tl=1;
		fill(d,d+n+1,inf);
		d[q[1]=s]=0;
		while(hd<=tl)
		{
			inq[u=q[hd++]]=0;
			for(i=head[u];i;i=e[i].nxt)
				if(e[i].w&&d[v=e[i].v]>d[u]+e[i].c)
				{
					d[v]=d[u]+e[i].c;
					if(!inq[v])q[++tl]=v;
				}
		}
		return d[t]!=inf;
	}
	ll dfs(int u,int t,ll fw)
	{
		int v;ll k,sm=0;
		if(u==t)return fw;
		inq[u]=1;
		for(int& i=cur[u];i;i=e[i].nxt)
			if(e[i].w&&d[v=e[i].v]==d[u]+e[i].c&&!inq[v]&&(k=dfs(v,t,min(1ll*e[i].w,fw-sm))))
			{
				e[i].w-=k;e[i^1].w+=k;
				sm+=k;ans+=k*e[i].c;
				if(sm==fw)break;
			}
		inq[u]=0;
		return sm;
	}
	pair<ll,ll> sol(int s,int t)
	{
		ll fl=0;
		while(spfa(s,t))
		{
			for(int i=0;i<=n;++i)cur[i]=head[i];
			fl+=dfs(s,t,inf);
		}
		return {fl,ans};
	}
};

struct dijcostflow
{
	ll d[N],h[N];
	bool inq[N],vis[N];
	int n,en,dep[N],head[N],cur[N],q[N*10+M];
	const ll inf=0x3f3f3f3f3f3f3f3fll;
	struct edge{int u,v,w,c,nxt;}e[M*2];
	void init(int nn){n=nn;en=1;for(int i=0;i<=n;++i)head[i]=0;}
	void _adde(int u,int v,int w,int c){e[++en]={u,v,w,c,head[u]};head[u]=en;}
	void adde(int u,int v,int w,int c){_adde(u,v,w,c);_adde(v,u,0,-c);}
	void spfa(int s)
	{
		int i,u,v,hd=1,tl=1;
		fill(h,h+n+1,inf);
		h[q[1]=s]=0;
		while(hd<=tl)
		{
			inq[u=q[hd++]]=0;
			for(i=head[u];i;i=e[i].nxt)
				if(e[i].w&&h[v=e[i].v]>h[u]+e[i].c)
				{
					h[v]=h[u]+e[i].c;
					if(!inq[v])q[++tl]=v;
				}
		}
	}
	bool dij(int s,int t)
	{
		int i,u,v;
		priority_queue<pair<ll,int>>q;
		for(i=0;i<=n;++i){d[i]=inf;vis[i]=0;}
		d[s]=0;q.push({0,s});
		while(!q.empty())
		{
			u=q.top().second;q.pop();
			if(vis[u])continue;vis[u]=1;
			for(i=head[u];i;i=e[i].nxt)
				if(e[i].w&&d[v=e[i].v]>d[u]+(e[i].c+h[u]-h[v]))
				{
					d[v]=d[u]+(e[i].c+h[u]-h[v]);
					q.push({-d[v],v});
				}
		}
		return d[t]!=inf;
	}
	bool bfs(int s,int t)
	{
		int i,u,v,hd=1,tl=1;
		fill(dep,dep+n+1,N);dep[q[1]=s]=0;
		while(hd<=tl)
			for(i=head[u=q[hd++]];i;i=e[i].nxt)
				if(e[i].w&&h[v=e[i].v]==h[u]+e[i].c&&dep[v]>dep[u]+1)
					dep[q[++tl]=v]=dep[u]+1;
		return dep[t]!=N;
	}
	ll dfs(int u,int t,ll fw)
	{
		int v;ll k,sm=0;
		if(u==t)return fw;
		for(int& i=cur[u];i;i=e[i].nxt)
			if(e[i].w&&h[v=e[i].v]==h[u]+e[i].c&&dep[v]==dep[u]+1&&(k=dfs(v,t,min(1ll*e[i].w,fw-sm))))
			{
				e[i].w-=k;e[i^1].w+=k;sm+=k;
				if(sm==fw)break;
			}
		return sm;
	}
	pair<ll,ll> sol(int s,int t)
	{
		spfa(s);
		ll fl=0,vl=0;
		while(dij(s,t))
		{
			for(int i=0;i<=n;++i)if(d[i]!=inf)h[i]+=d[i];
			while(bfs(s,t))
			{
				for(int i=0;i<=n;++i)cur[i]=head[i];
				ll x=dfs(s,t,inf);fl+=x;vl+=x*h[t];
			}
		}
		return {fl,vl};
	}
};

struct dominator_tree
{
	vector<int>g[N],h[N],q[N];
	int cnt,f[N],fa[N],in[N],fm[N],dfn[N],sdm[N],idm[N];
	void init(int n){cnt=0;for(int i=0;i<=n;++i){f[i]=in[i]=sdm[i]=i;dfn[i]=0;g[i].clear();h[i].clear();q[i].clear();}}
	void adde(int u,int v){g[u].push_back(v);h[v].push_back(u);}
	void find(int u)
	{
		if(u==f[u])return;
		find(f[u]);if(dfn[sdm[in[u]]]>dfn[sdm[in[f[u]]]])in[u]=in[f[u]];f[u]=f[f[u]];
	}
	void dfs(int u)
	{
		fm[dfn[u]=++cnt]=u;
		for(auto v:g[u])if(!dfn[v]){fa[v]=u;dfs(v);}
	}
	vector<int> sol(int n,int s)
	{
		dfs(s);
		for(int k=n;k>=2;--k)
		{
			int u=fm[k];
			for(auto v:q[u]){find(v);idm[v]=in[v];}
			for(auto v:h[u])
			{
				if(dfn[v]<dfn[sdm[u]]&&dfn[sdm[u]]>dfn[v])sdm[u]=v;
				if(dfn[v]>dfn[u])
				{
					find(v);
					if(dfn[sdm[u]]>dfn[sdm[in[v]]])sdm[u]=sdm[in[v]];
				}
			}
			q[sdm[u]].push_back(u);f[u]=fa[u];
		}
		for(auto v:q[1]){find(v);idm[v]=in[v];}
		for(int k=2;k<=n;++k)
		{
			int u=fm[k];
			if(sdm[u]!=sdm[idm[u]])idm[u]=idm[idm[u]];
			else idm[u]=sdm[u];
		}
		vector<int>ans;
		for(int i=0;i<=n;++i)ans.push_back(idm[i]);
		return ans;
	}
};

