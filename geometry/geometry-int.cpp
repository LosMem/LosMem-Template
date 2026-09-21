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
