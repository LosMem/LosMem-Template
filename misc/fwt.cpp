#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

/*
快速沃尔什变换，用于按位OR、AND、XOR卷积。
可用using FWT=fwt<mod>;定义类型别名。
FWT::OR/AND/XOR(f)为正变换，FWT::OR/AND/XOR(f,1)为逆变换。
要求P>1，使用XOR逆变换时P必须为奇数；数组长度必须是2的幂，元素初值在[0,P)内。
元素类型需为能容纳[-P+1,2P-2]的有符号整数。
支持vector、array及定长C数组，不支持仅有首地址的指针。
单次变换时间复杂度O(n log n)，额外空间O(1)。
*/
template<int P>
struct fwt
{
	static constexpr ll iv2=(P+1ll)/2;
	template<class T>
	static T add(T x,T y)
	{
		x+=y;
		if(x>=P)x-=P;
		return x;
	}
	template<class T>
	static T sub(T x,T y)
	{
		x-=y;
		if(x<0)x+=P;
		return x;
	}

	template<class A>
	static void OR(A& f,bool inv=0)
	{
		int n=(int)size(f);
		for(int k=1;k<n;k<<=1)
			for(int i=0;i<n;i+=k<<1)
				for(int j=0;j<k;++j)
				{
					auto& x=f[i+j+k];
					x=inv?sub(x,f[i+j]):add(x,f[i+j]);
				}
	}

	template<class A>
	static void AND(A& f,bool inv=0)
	{
		int n=(int)size(f);
		for(int k=1;k<n;k<<=1)
			for(int i=0;i<n;i+=k<<1)
				for(int j=0;j<k;++j)
				{
					auto& x=f[i+j];
					x=inv?sub(x,f[i+j+k]):add(x,f[i+j+k]);
				}
	}

	template<class A>
	static void XOR(A& f,bool inv=0)
	{
		int n=(int)size(f);
		ll ivn=1;
		for(int k=1;k<n;k<<=1)
		{
			if(inv)ivn=ivn*iv2%P;
			for(int i=0;i<n;i+=k<<1)
				for(int j=0;j<k;++j)
				{
					auto x=f[i+j],y=f[i+j+k];
					f[i+j]=add(x,y);
					f[i+j+k]=sub(x,y);
				}
		}
		if(inv)
			for(auto& x:f)
				x=x*ivn%P;
	}
};
