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
	bool operator<(vec b)const{return x!=b.x?x<b.x:y<b.y;}
	vec rot(db d)const{vec w={sinl(d),cosl(d)};return {*this*w,*this^w};}
	vec rot90(int z=1)const{return {-y*z,x*z};}
	db d1()const{return abs(x)+abs(y);}
	db d2()const{return *this^*this;}
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
