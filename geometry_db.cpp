//-fsanitize=float-divide-by-zero
#include <bits/stdc++.h>

typedef long double db;

using namespace std;

const db eps=1e-9L;
const db meps=1e-18L;
const db pi=acosl(-1.0L);

int sgn(db x,db e=eps){return (x>e)-(x<-e);}
bool eq(db a,db b){return abs(a-b)<=max(eps,meps*max(abs(a),abs(b)));}
struct vec
{
	db x,y;
	void read(){cin>>x>>y;}
	vec operator-()const{return {-x,-y};}
	vec operator+(vec b)const{return {x+b.x,y+b.y};}
	vec operator-(vec b)const{return {x-b.x,y-b.y};}
	vec operator*(db k)const{return {x*k,y*k};}
	vec operator/(db k)const{return {x/k,y/k};}
	db operator^(vec b)const{return x*b.x+y*b.y;}
	db operator*(vec b)const{return x*b.y-y*b.x;}
	bool operator==(vec b)const{return eq(x,b.x)&&eq(y,b.y);}
	bool operator!=(vec b)const{return !(*this==b);}
	bool operator<(vec b)const{return x!=b.x?x<b.x:y<b.y;}
	vec rot(db d)const{vec w={sinl(d),cosl(d)};return {*this*w,*this^w};}
	vec rot90(int z=1)const{return {-y*z,x*z};}
	db d1()const{return abs(x)+abs(y);}
	db d()const{return sqrtl(x*x+y*y);}
};
int dir(vec a,vec b){return sgn(a*b,meps*a.d1()*b.d1());}
int ori(vec a,vec b,vec c){return dir(b-a,c-a);}
struct line
{
	vec s,v;
	int operator|(vec t)const{return sgn(v*(t-s),eps*v.d1());}
	db operator%(vec t)const{return abs((t-s)*v)/v.d();}
	int operator&(line b)const
	{
		return dir(v,b.v)?1:(*this|b.s?0:2);
	}
	vec operator*(line b)const
	{
		return s+v*(((s-b.s)*b.v)/(b.v*v));
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
		db e=eps*v.d1();
		return !sgn(v*(u-s),e)
			&&sgn((u-s)^v,e)>=0
			&&sgn((u-t)^v,e)<=0;
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
	db C()
	{
		db ans=0;
		for(int i=0,n=a.size();i<n;++i)ans+=(a[i]-a[(i+1)%n]).d();
		return ans;
	}
	db S()
	{
		db ans=0;
		for(int i=0,n=a.size();i<n;++i)
			ans+=(a[i]-a[0])*(a[(i+1)%n]-a[0]);
		return ans/2;
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
poly half_plane(vector<line> s)
{
	if(s.empty())return {};
	auto hf=[](vec v){return v.y<0||(v.y==0&&v.x<0);};
	auto par=[](line a,line b){return dir(a.v,b.v)==0;};
	sort(s.begin(),s.end(),[&](line a,line b)
    {
        int x=hf(a.v),y=hf(b.v);
        if(x!=y)return x<y;
        db c=a.v*b.v;
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
	vector<vec>a;
	for(int i=0,n=q.size();i<n;++i)
		a.push_back(q[i]*q[(i+1)%n]);
	return {a};
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
// 用直线分割凸多边形，顶点按边界顺序排列；返回值分别为 (l|p)>=0 与 <=0 两侧。
// 切线或共边时零面积部分为空，退化或非凸输入不保证正确。
pair<poly,poly> split(const poly& P,line l)
{
	int i,n=P.a.size(),fl=0,fr=0;
	vector<int> b(n);for(i=0;i<n;++i){b[i]=l|P.a[i];fl|=b[i]==1;fr|=b[i]==-1;}
	if(!fr)return {P,{}};
	if(!fl)return {{},P};
	poly A,B;
	for(i=0;b[i]==1;++i);
	for(;b[i]!=1;i=(i+1)%n);
	A.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	for(;b[i]==1;i=(i+1)%n)A.a.push_back(P.a[i]);
	A.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	if(b[i]==0)i=(i+1)%n;
	B.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	for(;b[i]==-1;i=(i+1)%n)B.a.push_back(P.a[i]);
	B.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	return {A,B};
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
db simpson_rec(db l,db r,db e,int d,db fl,db fm,db fr,db S,db (*f)(db))
{
	db m=(l+r)/2,x=(l+m)/2,y=(m+r)/2,fx=f(x),fy=f(y);
	db L=(fl+4*fx+fm)*(m-l)/6,R=(fm+4*fy+fr)*(r-m)/6,t=L+R-S;
	if(abs(t)<15*e&&d<=0)return L+R+t/15;
	return simpson_rec(l,m,e/2,d-1,fl,fx,fm,L,f)
			 +simpson_rec(m,r,e/2,d-1,fm,fy,fr,R,f);
}
// 自适应辛普森积分：f 应在 [l,r] 上连续且足够光滑，eps 为误差阈值。
// step 为初始强制递归层数而非最大深度，奇异或不连续函数可能递归过深。
db simpson(db l,db r,db eps,int step,db (*f)(db x))
{
	db m=(l+r)/2,fl=f(l),fm=f(m),fr=f(r),S=(fl+4*fm+fr)*(r-l)/6;
	return simpson_rec(l,r,eps,step,fl,fm,fr,S,f);
}
