#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;
using namespace __gnu_pbds;

struct Hash
{
	size_t operator()(ull x)const
	{
		static const ull S=chrono::steady_clock::now().time_since_epoch().count();
		x+=S;
		x=(x^(x>>30))*0xbf58476d1ce4e5b9ULL;
		x=(x^(x>>27))*0x94d049bb133111ebULL;
		return x^(x>>31);
	}
};

unordered_map<ll,int,Hash>mp;
gp_hash_table<ll,int,Hash>gp;
