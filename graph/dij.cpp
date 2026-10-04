#include <bits/stdc++.h>

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
