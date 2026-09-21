#include <bits/stdc++.h>

#define N 100005

using namespace std;

struct treeintree
{
	#define M 100 //log^2
	#define lson l,m,ls[rt]
	#define rson m+1,r,rs[rt]
	#define lowbit(x) ((x)&-(x))

	int n,cnt,tr[N*M],ls[N*M],rs[N*M],rti[N];
	void init(int nn)
	{
		n=nn;cnt=0;
		for(int i=0;i<=n*M;++i)tr[i]=ls[i]=rs[i]=0;
		for(int i=0;i<=n;++i)rti[i]=0;
	}
	void up(int rt){tr[rt]=tr[ls[rt]]+tr[rs[rt]];}
	void add(int a,int b,int l,int r,int& rt)
	{
		if(!rt)rt=++cnt;
		if(l==r){tr[rt]+=b;return;}
		int m=(l+r)>>1;
		if(a<=m)add(a,b,lson);
		else add(a,b,rson);
		up(rt);
	}
	int que(int a,int b,int l,int r,int rt)
	{
		if(!rt)return 0;
		if(a<=l&&r<=b)return tr[rt];
		int m=(l+r)>>1,ans=0;
		if(a<=m)ans=que(a,b,lson);
		if(b>m)ans+=que(a,b,rson);
		return ans;
	}
	void add(int x,int y,int w)
	{for(int i=x;i<=n;i+=lowbit(i))add(y,w,1,n,rti[i]);}
	int que(int l1,int r1,int l2,int r2)
	{
		int ans=0;
		for(int i=r1;i;i-=lowbit(i))ans+=que(l2,r2,1,n,rti[i]);
		for(int i=l1-1;i;i-=lowbit(i))ans-=que(l2,r2,1,n,rti[i]);
		return ans;
	}

	#undef lson
	#undef rson
};
struct basis
{
	int sz,w[N+1];
	void add(int x)
	{
		for(int i=N;i>=0;--i)
			if(x&(1<<i))
			{
				if(!w[i]){w[i]=x;++sz;break;}
				else x^=w[i];
			}
	}
	int que(int x)
	{
		for(int i=N;i>=0;--i)
			if(x&(1<<i))
			{
				if(!w[i])return 0;
				x^=w[i];
			}
		return 1;
	}
	basis operator+(basis b)
	{
		for(int i=N;i>=0;--i)
			if(w[i])b.add(w[i]);
		return b;
	}
};