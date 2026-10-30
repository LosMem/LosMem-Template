#include <bits/stdc++.h>

using namespace std;

/*
虚树（树链剖分LCA），点编号0..n，n为最大编号。
virtual_tree t(n);t.adde(u,v);t.build(rt);
int p=t.lca(u,v);auto e=t.que(s);
原树非空，每条无向边只adde一次；先build再查询，修改原树或换根后须重新build。
que支持重复点和空集合，不同关键点不足2个时返回空边表。
虚树根为关键点的LCA；需要原树根参与时将其加入s。
返回Edge{u,v,w}：父亲u指向儿子v，w为对应原树路径的边数。
边按自底向上顺序返回，可依次进行子节点到父节点的DP转移。
设k为传入点数：预处理O(n)，LCA O(log n)，que O(k log k+k log n)，空间O(n+k)。
*/
struct virtual_tree
{
	int cnt=0;
	vector<vector<int>>g;
	vector<int>d,sz,sn,fa,dfn,tp,st;
	virtual_tree(int n):g(n+1),d(n+1),sz(n+1),sn(n+1),fa(n+1),dfn(n+1),tp(n+1){}
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
		tp[u]=p;dfn[u]=++cnt;
		if(sn[u]!=-1)dfs2(sn[u],p);
		for(auto v:g[u])
			if(v!=fa[u]&&v!=sn[u])
				dfs2(v,v);
	}
	void build(int rt)
	{
		cnt=0;d[fa[rt]=rt]=0;
		dfs1(rt);dfs2(rt,rt);
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
	struct Edge{int u,v,w;};
	vector<Edge> que(vector<int> s)
	{
		st.clear();
		vector<Edge> ans;
		sort(s.begin(),s.end(),[&](int u,int v){return dfn[u]<dfn[v];});
		auto ins=[&](int u,int v){ans.push_back({u,v,d[v]-d[u]});};
		auto pop=[&](){ins(st[st.size()-2],st.back());st.pop_back();};
		for(auto u:s)
			if(st.empty())st.push_back(u);
			else
			{
				int z=lca(u,st.back());
				while(st.size()>1&&d[st[st.size()-2]]>=d[z])pop();
				if(st.back()!=z)ins(z,st.back()),st.back()=z;
				if(z!=u)st.push_back(u);
			}
		while(st.size()>1)pop();
		return ans;
	}
};
