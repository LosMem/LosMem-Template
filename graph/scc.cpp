#include <bits/stdc++.h>

using namespace std;

/*
Tarjan强连通分量：SCC t(n);t.adde(u,v);auto bel=t.scc();
点编号0..n，n为最大编号；使用1..n时调用scc(1)。
先完成加边，再调用一次scc；支持重边和自环。
返回bel数组，bel[u]为u所属分量，编号1..tot，tot为分量数。
跨分量边u->v满足bel[u]>bel[v]，分量编号从大到小为拓扑序。
时间O(n+m)，空间O(n+m)。
*/
struct SCC
{
	int n,cnt=0,tot=0;
	vector<vector<int>>g;
	vector<int>dfn,low,bel,st;
	SCC(int n):n(n),g(n+1),dfn(n+1),low(n+1),bel(n+1){}
	void adde(int u,int v)
	{
		g[u].push_back(v);
	}
	void dfs(int u)
	{
		dfn[u]=low[u]=++cnt;
		st.push_back(u);
		for(auto v:g[u])
			if(!dfn[v])
			{
				dfs(v);
				low[u]=min(low[u],low[v]);
			}
			else if(!bel[v])low[u]=min(low[u],dfn[v]);
		if(low[u]==dfn[u])
		{
			++tot;
			int v;
			do
			{
				v=st.back();st.pop_back();
				bel[v]=tot;
			}while(v!=u);
		}
	}
	vector<int> scc(int l=0)
	{
		for(int u=l;u<=n;++u)
			if(!dfn[u])dfs(u);
		return bel;
	}
};
