#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

ll qpow(ll a,ll b,ll mod){ll ans=1;while(b){if(b&1)ans=(__int128)ans*a%mod;a=(__int128)a*a%mod;b>>=1;}return ans;}
ll exgcd(ll a,ll b,ll& x,ll& y)
{
	if(b==0){x=1;y=0;return a;}
	ll g=exgcd(b,a%b,x,y);
	x-=a/b*y;swap(x,y);
	return g;
}
pair<ll,ll> excrt(vector<pair<ll,ll>> S)
{
	ll A=1,B=0,x,y;
	for(auto [a,b]:S)
	{
		ll g=exgcd(A,a,x,y);
		if((b-B)%g)return {0,0};
		x*=(b-B)/g;x=(x%(a/g)+a/g)%(a/g);
		B+=A*x;A=A/g*a;B=(B%A+A)%A;
	}
	return {A,B};
}
ll exbsgs(ll g,ll a,ll p)
{
	g%=p;a%=p;if(a==1||p==1)return 0;
	ll s=1,z=__lg(p),c,gk=1,ml;int i,k=sqrt(p)+1;
	for(i=1;i<=z;++i){s=s*g%p;if(s==a)return i;}
	c=gcd(s,p);if(a%c)return -1;s/=c;a/=c;p/=c;
	unordered_map<ll,int>mp;
	for(i=0;i<k;++i){mp[a*gk%p]=i;gk=gk*g%p;}
	for(i=1,ml=s*gk%p;i<=k;++i,ml=ml*gk%p)
		if(mp.find(ml)!=mp.end())
			return 1ll*i*k-mp[ml]+z;
	return -1;
}
bool MRtest(ll p)
{
	if(p<=2||p%2==0)return p==2;
	ll s,t,x;for(s=p-1,t=0;s%2==0;s/=2,++t);
	for(auto a:{2,325,9375,28178,450775,9780504,1795265022})
	{
		x=qpow(a,s,p);
		if(x<=1||x==p-1)continue;
		for(int i=1;i<=t;++i)
		{
			x=(__int128)x*x%p;
			if(x==p-1&&i!=t){x=1;break;}
			if(x==1)return 0;
		}
		if(x!=1)return 0;
	}
	return 1;
}
ll PRho(ll p)
{
	ll s=0,t=0,g,ml=1,c=rand()%p;
	for(int k=1;;k*=2,s=t)
		for(int i=0;i<k;++i)
		{
			t=((__int128)t*t+c)%p;
			if(i%127==0||(__int128)ml*(t-s)%p==0)
				if((g=gcd(abs(ml),p))!=1)
				{
					if(g==p)return PRho(p);
					return g;
				}
			ml=(__int128)ml*(t-s)%p;
		}
}
vector<ll>factor(ll n)
{
	if(n==1)return {};
	if(MRtest(n))return {n};
	ll x=PRho(n);auto A=factor(x),B=factor(n/x);vector<ll> C(A.size()+B.size());
	merge(A.begin(),A.end(),B.begin(),B.end(),C.begin());
	return C;
}
struct min25
{
	#define N 300005 //
	#define mod 1000000007 //

	bool vis[N];
	ll n,d[N],g[N][3];
	int m,s,cnt,p[N];
	ll f(ll p,int c) //
	{
		if(c==0)return 1;
		return p^c;
	}
	int id(ll x){return x<=m?x:s-n/x+1;}
	ll calc(ll nn)
	{
		int i,j;
		m=sqrt(n=nn);s=2*m-(1ll*m*m==n);
		for(i=2;i<=m;++i)
		{
			if(!vis[i])p[++cnt]=i;
			for(j=1;p[j]*i<=m;++j)
			{
				vis[p[j]*i]=1;
				if(i%p[j]==0)break;
			}
		}
		for(i=1;i<=m;++i)d[i]=i;
		for(i=1;i<=m;++i)d[s-i+1]=n/i;
		for(i=1;i<=s;++i)
		{
			g[i][1]=(d[i]-1)%mod; //
			g[i][2]=((__int128)d[i]*(d[i]+1)/2)%mod-1; //
		}
		for(i=1;i<=cnt;++i)
			for(j=s;d[j]>=1ll*p[i]*p[i];--j)
			{
				(g[j][1]-=(g[id(d[j]/p[i])][1]-g[id(p[i-1])][1])*1)%=mod; //
				(g[j][2]-=(g[id(d[j]/p[i])][2]-g[id(p[i-1])][2])*p[i])%=mod; //
			}
		for(i=1;i<=s;++i)
			g[i][0]=(g[i][2]-g[i][1]+(d[i]>=2)*2)%mod; //
		return (S(n,0)+f(0,0)+mod)%mod;
	}
	ll S(ll n,int j)
	{
		int k,t;ll ml,ans=(g[id(n)][0]-g[id(p[j])][0])%mod;if(n<=p[j])ans=0;
		for(k=j+1;k<=cnt&&1ll*p[k]*p[k]<=n;++k)
			for(t=1,ml=p[k];;++t,ml*=p[k])
			{
				(ans+=f(p[k],t)*(S(n/ml,k)+(t!=1)))%=mod;
				if(ml>n/p[k])break;
			}
		return ans;
	}

	#undef N
	#undef mod
}A;
