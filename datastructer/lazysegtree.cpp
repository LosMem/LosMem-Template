#include <bits/stdc++.h>

#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1

using namespace std;

/*
用法：segtree<info,tag> Tr(a);输入数组a必须非空，点编号为0..n-1。
upd(l,r,x)对闭区间[l,r]应用标记x；que(l,r)查询闭区间[l,r]；set(p,x)覆盖单点；all()查询全部。
非空区间须满足0<=l<=r<n，单点须满足0<=p<n，不检查越界；l>r时upd无操作、que返回info{}。
info和tag须可默认构造及复制；info支持operator+和apply(const tag&,int len)，tag支持apply(const tag&)。
old.apply(x)须表示先执行old再执行x；has单独记录标记是否存在，因此tag{}不必是单位标记。
若使用空区间查询，则info{}须为operator+的单位元。
建树O(n)，区间修改、区间查询和单点覆盖O(log n)，all()为O(1)，空间O(n)。
*/
template<class info,class tag>
struct segtree
{
	int n;
	vector<info>tr;
	vector<tag>lz;
	vector<char>has;

	void up(int rt){tr[rt]=tr[rt<<1]+tr[rt<<1|1];}
	void apply(const tag& x,int l,int r,int rt)
	{
		tr[rt].apply(x,r-l+1);
		if(has[rt])lz[rt].apply(x);
		else lz[rt]=x,has[rt]=1;
	}
	void dw(int l,int r,int rt)
	{
		if(!has[rt])return;
		int m=(l+r)>>1;
		apply(lz[rt],lson);
		apply(lz[rt],rson);
		has[rt]=0;
	}
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
	void upd(int a,int b,const tag& x,int l,int r,int rt)
	{
		if(a<=l&&r<=b)
		{
			apply(x,l,r,rt);
			return;
		}
		dw(l,r,rt);int m=(l+r)>>1;
		if(a<=m)upd(a,b,x,lson);
		if(b>m)upd(a,b,x,rson);
		up(rt);
	}
	info que(int a,int b,int l,int r,int rt)
	{
		if(a<=l&&r<=b)return tr[rt];
		dw(l,r,rt);int m=(l+r)>>1;
		if(a<=m&&b>m)return que(a,b,lson)+que(a,b,rson);
		if(a<=m)return que(a,b,lson);
		return que(a,b,rson);
	}
	void set(int a,const info& x,int l,int r,int rt)
	{
		if(l==r)
		{
			tr[rt]=x;has[rt]=0;
			return;
		}
		dw(l,r,rt);int m=(l+r)>>1;
		if(a<=m)set(a,x,lson);
		else set(a,x,rson);
		up(rt);
	}

	segtree(const vector<info>& a):
		n((int)a.size()),tr(n*4+4),lz(n*4+4),has(n*4+4)
	{build(a,0,n-1,1);}
	void upd(int l,int r,const tag& x){if(l<=r)upd(l,r,x,0,n-1,1);}
	info que(int l,int r){return l>r?info{}:que(l,r,0,n-1,1);}
	void set(int p,const info& x){set(p,x,0,n-1,1);}
	info all(){return tr[1];}
};
