#include <bits/stdc++.h>

typedef long long ll;
typedef __int128 i128;

using namespace std;

template<typename T>
int sgn(T x){return (x>0)-(x<0);}
struct vec
{
	ll x,y;
	void read(){cin>>x>>y;}
	vec operator-()const{return {-x,-y};}
	vec operator+(vec b)const{return {x+b.x,y+b.y};}
	vec operator-(vec b)const{return {x-b.x,y-b.y};}
	vec operator*(ll k)const{return {x*k,y*k};}
	ll operator^(vec b)const{return x*b.x+y*b.y;}
	ll operator*(vec b)const{return x*b.y-y*b.x;}
	bool operator==(vec b)const{return x==b.x&&y==b.y;}
	bool operator!=(vec b)const{return !(*this==b);}
	bool operator<(vec b)const{return x!=b.x?x<b.x:y<b.y;}
	vec rot90(int z=1)const{return {-y*z,x*z};}
	ll d1()const{return abs(x)+abs(y);}
};
struct rvec{i128 x,y,z;};
int dir(vec a,vec b){return sgn(a*b);}
int ori(vec a,vec b,vec c){return dir(b-a,c-a);}
struct line
{
	vec s,v;
	int operator|(vec t)const{return sgn(v*(t-s));}
	int operator|(rvec t)const
	{
		return sgn(v.x*(t.y-s.y*t.z)
				  -v.y*(t.x-s.x*t.z));
	}
	int operator&(line b)const
	{
		return dir(v,b.v)?1:(*this|b.s?0:2);
	}
	rvec operator*(line b)const
	{
		i128 z=v*b.v,t=(b.s-s)*b.v;
		i128 x=s.x*z+v.x*t;
		i128 y=s.y*z+v.y*t;
		if(z<0)x=-x,y=-y,z=-z;
		return {x,y,z};
	}
};
struct seg
{
	vec s,t;
	line operator~()const{return {s,t-s};}
	bool operator&(vec u)const
	{
		vec v=t-s;
		if(v.d1()==0)return u==s;
		return !sgn(v*(u-s))
			&&sgn((u-s)^v)>=0
			&&sgn((u-t)^v)<=0;
	}
	int operator&(seg b)const
	{
		if(*this&b.s||*this&b.t||b&s||b&t)return 2;
		line a=~*this,c=~b;
		return (a|b.s)*(a|b.t)==-1
			&&(c|s)*(c|t)==-1;
	}
};
struct poly
{
	vector<vec> a;
	ll S2()
	{
		ll ans=0;
		for(int i=0,n=a.size();i<n;++i)
			ans+=(a[i]-a[0])*(a[(i+1)%n]-a[0]);
		return ans;
	}
};
// fl = 0：严格凸包，不保留边上的共线点
// fl = 1：保留凸包边界上的共线点，要求凸包非退化
// 返回点按逆时针排列，首尾不重复
poly convex_hull(vector<vec> s,int fl=0)
{
	sort(s.begin(),s.end());
	s.erase(unique(s.begin(),s.end(),
		[](vec a, vec b){return a.x==b.x&&a.y==b.y;}),s.end());
	vector<vec>d,u;
	auto add=[&](vector<vec>& q,vec t)
	{
		while(q.size()>1&&ori(q[q.size()-2],q[q.size()-1],t)<=-fl)
			q.pop_back();
		q.push_back(t);
	};
	for(auto t:s)add(d,t);
	for(int i=(int)s.size()-1;i>=0;--i)add(u,s[i]);
	for(int i=1;i<(int)u.size()-1;++i)
		d.push_back(u[i]);
	return {d};
}
// 半平面交：交集应为有界、非退化的凸多边形；空集、无界或退化情况返回空多边形。
vector<rvec> half_plane(vector<line> s)
{
	if(s.empty())return {};
	auto hf=[](vec v){return v.y<0||(v.y==0&&v.x<0);};
	auto par=[](line a,line b){return dir(a.v,b.v)==0;};
	sort(s.begin(),s.end(),[&](line a,line b)
    {
        int x=hf(a.v),y=hf(b.v);
        if(x!=y)return x<y;
        ll c=a.v*b.v;
        return c?c>0:b.v*(a.s-b.s)>0;
    });
	deque<line>q;
	auto trim=[&](line t,bool ps)
	{
		while(q.size()>1)
		{
			line a=ps?q.back():q.front();
			line b=ps?q[q.size()-2]:q[1];
			if(par(a,b))return 0;
			if((t|(a*b))>0)break;
			if(ps)q.pop_back();
			else q.pop_front();
		}
		return 1;
	};
	for(auto t:s)
	{
		if(!q.empty()&&par(q.back(),t)&&(q.back().v^t.v)>0)
		{
			if((t|q.back().s)>=0)
				continue;
			q.pop_back();
		}
		if(!trim(t,1)||!trim(t,0))return {};
		q.push_back(t);
	}
	if(q.size()<3
	||!trim(q.front(),1)
	||!trim(q.back(),0)
	||q.size()<3
	||dir(q.back().v,q[0].v)<=0)
		return {};
	vector<rvec>a;
	for(int i=0,n=q.size();i<n;++i)
		a.push_back(q[i]*q[(i+1)%n]);
	return a;
}
// 闵可夫斯基和：要求 P、Q 均为非退化凸多边形，顶点按逆时针排列且首尾不重复。
poly minkowski(poly P,poly Q)
{
	if(P.a.empty()||Q.a.empty())return {};
	auto fix=[](vector<vec>& p)
	{
		int k=min_element(p.begin(),p.end())-p.begin();
		rotate(p.begin(),p.begin()+k,p.end());
	};
	fix(P.a);fix(Q.a);
	int i=0,j=0,n=P.a.size(),m=Q.a.size();
	vec u=P.a[0]+Q.a[0];
	vector<vec>c;
	c.push_back(u);
	while(i<n||j<m)
	{
		vec ea=i<n?P.a[(i+1)%n]-P.a[i]:vec{0,0};
		vec eb=j<m?Q.a[(j+1)%m]-Q.a[j]:vec{0,0};
		if(j==m||(i<n&&ea*eb>=0)){u=u+ea;++i;}
		else{u=u+eb;++j;}
		c.push_back(u);
	}
	c.pop_back();
	return {c};
}
// 在线维护插入点的凸包：ins 只支持插入，chk 判断点是否在当前凸包内（含边界）。
// 重复点会被忽略；插入均摊 O(log n)，查询 O(log n)，不支持删除。
struct dynamic_convex
{
	set<vec>u,d,all;
	static bool inside(const set<vec>&h,vec p,int z)
	{
		if(h.empty())return 0;
		auto r=h.lower_bound(p);
		if(r==h.begin())return *r==p;
		if(r==h.end())return 0;
		return z*(~seg{*prev(r),*r}|p)<=0;
	}
	static void add(set<vec>&h,vec p,int z)
	{
		auto r=h.lower_bound(p);
		if(r!=h.begin()&&r!=h.end()&&z*(~seg{*prev(r),*r}|p)<=0)
			return;
		auto t=h.insert(p).first;
		while(r!=h.end()&&next(r)!=h.end())
		{
			auto nr=next(r);
			if(z*(~seg{p,*nr}|*r)>0)break;
			h.erase(r);r=nr;
		}
		while(t!=h.begin())
		{
			auto l=prev(t);
			if(l==h.begin())break;
			auto pl=prev(l);
			if(z*(~seg{*pl,p}|*l)>0)break;
			h.erase(l);
		}
	}
	void ins(vec p)
	{
		if(!all.insert(p).second)return;
		add(u,p,1);add(d,p,-1);
	}
	bool chk(vec p)const
	{
		return inside(u,p,1)&&inside(d,p,-1);
	}
};
