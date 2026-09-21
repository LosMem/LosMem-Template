#include <bits/stdc++.h>

#define N 1000005 //
#define mod 998244353 //

typedef long long ll;

using namespace std;

ll qpow(ll a,int b){ll ans=1;b=(b+mod-1)%(mod-1);while(b){if(b&1)ans=ans*a%mod;a=a*a%mod;b>>=1;}return ans;}
void ntt(vector<ll>& a,int n,int fl)
{
	int i,k,l;
	static int rv[N*4];a.resize(n);
	for(i=1;i<n;++i){rv[i]=(rv[i>>1]>>1)|(i&1)*(n/2);if(i<rv[i])swap(a[i],a[rv[i]]);}
	for(k=1;k<n;k*=2)
	{
		ll w=qpow(3,fl*(mod-1)/k/2);
		for(l=0;l<n;l+=2*k)
		{
			ll wk=1;
			for(i=l;i<l+k;++i)
			{
				a[i+k]=(a[i]-a[i+k]*wk%mod+mod)%mod;
				a[i]=(2*a[i]-a[i+k]+mod)%mod;
				wk=wk*w%mod;
			}
		}
	}
}
vector<ll> operator*(vector<ll> a,vector<ll> b)
{
	if(a.empty()||b.empty())return {};
	int za=a.size(),zb=b.size(),n=1<<__lg(za+zb-2)+1;
	ntt(a,n,1);ntt(b,n,1);ll in=qpow(n,mod-2);
	for(int i=0;i<n;++i)a[i]=a[i]*b[i]%mod*in%mod;
	ntt(a,n,-1);a.resize(za+zb-1);
	return a;
}
vector<ll> SubConvol(vector<ll> a,vector<ll> b)
{
	reverse(b.begin(),b.end());auto c=a*b;
	if(c.size()<b.size())c={};else c.erase(c.begin(),c.begin()+b.size()-1);
	return c;
}
void pmod(vector<ll>& a,int s){while(a.size()>s){(*(a.end()-s-1)+=a.back())%=mod;a.pop_back();}}
vector<ll> qpow(vector<ll> a,int b,int s=N)
{
	vector<ll> ans={1};
	while(b)
	{
		if(b&1){ans=ans*a;pmod(ans,s);}
		a=a*a;pmod(a,s);b>>=1;
	}
	return ans;
}
