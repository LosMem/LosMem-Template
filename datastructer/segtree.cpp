#include <bits/stdc++.h>

#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1

using namespace std;

/*
用法：segtree<info> Tr(a);输入数组a必须非空，点编号为0..a.size()-1。
set(p,x)覆盖单点；que(l,r)查询闭区间[l,r]；all()查询全部。
info须可默认构造及复制并支持operator+，operator+满足结合律但不要求交换律。
info{}不必是单位元，仅当que的l>r时作为空区间结果返回。
合法非空区间须满足0<=l<=r<n，单点须满足0<=p<n，不检查越界。
建树O(n)，单点修改和区间查询O(log n)，all()为O(1)，空间O(n)。
*/
template<class info>
struct segtree
{
	int n;
	vector<info>tr;

	void up(int rt){tr[rt]=tr[rt<<1]+tr[rt<<1|1];}
	void build(const vector<info>& a,int l,int r,int rt)
	{
		if(l==r)
		{
			tr[rt]=a[l];
			return;
		}
		int m=(l+r)>>1;
		build(a,lson);
		build(a,rson);
		up(rt);
	}
	void set(int a,const info& x,int l,int r,int rt)
	{
		if(l==r)
		{
			tr[rt]=x;
			return;
		}
		int m=(l+r)>>1;
		if(a<=m)set(a,x,lson);
		else set(a,x,rson);
		up(rt);
	}
	info que(int a,int b,int l,int r,int rt)
	{
		if(a<=l&&r<=b)return tr[rt];
		int m=(l+r)>>1;
		if(b<=m)return que(a,b,lson);
		if(a>m)return que(a,b,rson);
		return que(a,b,lson)+que(a,b,rson);
	}

	segtree(const vector<info>& a):n((int)a.size()),tr(n*4+4)
	{build(a,0,n-1,1);}
	void set(int p,const info& x){set(p,x,0,n-1,1);}
	info que(int l,int r){return l>r?info{}:que(l,r,0,n-1,1);}
	info all(){return tr[1];}
};
