#include <bits/stdc++.h>

#define N 1000005

using namespace std;

struct SA
{
	int sa[N],rk[N*2],tp[N],ct[N],h[N];
	void sol(char* st)
	{
		int i,k,n=strlen(st+1),ps;
		fill(ct,ct+128,0);
		fill(rk,rk+2*n+1,0);
		for(i=1;i<=n;++i)ct[st[i]]=1;
		for(i=1;i<=128;++i)ct[i]+=ct[i-1];
		for(i=1;i<=n;++i)rk[i]=ct[st[i]];
		for(k=1;k<=n;k<<=1)
		{
			fill(ct,ct+n+1,0);
			for(i=1;i<=n;++i)++ct[rk[i+k]];
			for(i=1;i<=n;++i)ct[i]+=ct[i-1];
			for(i=n;i;--i)tp[ct[rk[i+k]]--]=i;
			fill(ct,ct+n+1,0);
			for(i=1;i<=n;++i)++ct[rk[i]];
			for(i=1;i<=n;++i)ct[i]+=ct[i-1];
			for(i=n;i;--i)sa[ct[rk[tp[i]]]--]=tp[i];
			for(i=1,ps=0;i<=n;++i)
			{
				if(rk[sa[i]]!=rk[sa[i-1]]||rk[sa[i]+k]!=rk[sa[i-1]+k])++ps;
				tp[sa[i]]=ps;
			}
			for(i=1;i<=n;++i)rk[i]=tp[i];
		}
		for(i=1,k=0;i<=n;++i)
		{
			k=max(k-1,0);
			while(st[i+k]==st[sa[rk[i]-1]+k])++k;
			h[rk[i]]=k;
		}
	}
};
