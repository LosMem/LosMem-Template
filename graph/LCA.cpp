#include <bits/stdc++.h>

using namespace std;

/*
树链剖分 LCA：LCA t(n);t.adde(u,v);t.build(rt);int p=t.lca(u,v);
点编号可为0..n（n为最大编号），每条无向边仅调用一次adde，内部自动双向加边。
实际使用的节点须组成一棵非空树；build后才能查询，根和查询点须属于该树。
可重复build换根；修改树后须重新build，且修改后的图仍须为树。
d为深度（根深度为0），fa为父亲（根的父亲为自身），tp为重链链头。
sz为子树大小，sn为重儿子，无重儿子时为-1。
无权距离为(d[u]-d[p])+(d[v]-d[p])，其中p=lca(u,v)。
建树O(n)，每次LCA查询O(log n)，空间O(n)。
*/
struct LCA
{
	vector<vector<int>>g;
	vector<int>d,sz,sn,fa,tp;
	LCA(int n):g(n+1),d(n+1),sz(n+1),sn(n+1),fa(n+1),tp(n+1){}
	void adde(int u,int v)
	{
		g[u].push_back(v);
		g[v].push_back(u);
	}
	void dfs1(int u)
	{
		sz[u]=1;sn[u]=-1;
		for(auto v:g[u])
			if(v!=fa[u])
			{
				d[v]=d[fa[v]=u]+1;
				dfs1(v);sz[u]+=sz[v];
				if(sn[u]==-1||sz[sn[u]]<sz[v])sn[u]=v;
			}
	}
	void dfs2(int u,int p)
	{
		tp[u]=p;
		if(sn[u]!=-1)dfs2(sn[u],p);
		for(auto v:g[u])
			if(v!=fa[u]&&v!=sn[u])
				dfs2(v,v);
	}
	void build(int rt)
	{
		d[fa[rt]=rt]=0;dfs1(rt);
		dfs2(rt,rt);
	}
	int lca(int u,int v)
	{
		while(tp[u]!=tp[v])
		{
			if(d[tp[u]]>d[tp[v]])u=fa[tp[u]];
			else v=fa[tp[v]];
		}
		return d[u]<d[v]?u:v;
	}
};
