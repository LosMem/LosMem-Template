#include <bits/stdc++.h>

#define N 1000005 //
#define mod 998244353 //

typedef long long ll;

using namespace std;

ll ft[N],ftv[N],iv[N];
ll C(int n,int m){return n>=m&&m>=0?ft[n]*ftv[m]%mod*ftv[n-m]%mod:0;}
ll qpow(ll a,int b){ll ans=1;b=(b+mod-1)%(mod-1);while(b){if(b&1)ans=ans*a%mod;a=a*a%mod;b>>=1;}return ans;}
void Cinit()
{
	ft[0]=ftv[0]=1;
	for(int i=1;i<N;++i)
	{
		ft[i]=ft[i-1]*i%mod;
		iv[i]=i==1?1:(mod-mod/i)*iv[mod%i]%mod;
		ftv[i]=ftv[i-1]*iv[i]%mod;
	}
}
