# 计算几何

## 杂言

符号体系可能比较混乱。

使用板子前，建议先看看是否需要修改 eps。

sgn 函数的作用是判断浮点数 x 的符号，相比在代码其余部分每次都与 eps 比较，这么做有一些好处：1. 较为直观 2. 能快速把浮点数意义下的板子转化为只有整数运算的板子（因为你不需要一个个去修改代码中出现的 eps，它已经把所有需要修改的部分都包含在了这里）。

## 向量 (vec)

我认为点和向量是相似的，没必要分开写，而且抄板子也是需要时间的。

- read() 函数用于读入点，格式为 x 和 y 由空格隔开（注意题目将 x 和 y 分为两行输入时就不要用了）。
- vec+vec=vec 和 vec-vec=vec 分别为向量加减法，vec*db=vec 是向量数乘。
- vec*vec=vec 是叉乘，vec^vec=vec 是点乘（可能和其他人的板子不太一样，因为我认为按位或和点乘都是对每个"位置"分别运算，更符合我的理解）。
- == 和 != 判断向量是否相同。
- < 以 x 为第一关键字，y 为第二关键字，用于判断向量大小
- rot(db d) 返回本向量逆时针旋转弧度 d 后的向量
- rot90(int z) 返回本向量旋转弧度 $\frac \pi 2$ 后的向量，$z=1$ 时为逆时针，$z=-1$ 时为顺时针
- d() 返回向量长度

## 直线 (line)

直线表示为二元组 {s,v}，其中 s 为直线上某点，v 是这个直线的方向向量。

- line&vec 返回 0/1，表示这个点是否在直线上。
- line|vec 返回 -1/0/1，表示这个点与直线的方位，点的位置在：顺时针(-1)，直线上(0)，逆时针(1)
- line%vec 返回点到直线的距离
- line&line 返回 0/1/2，表示两条直线关系：平行(0)，相交(1)，重合(2)
- line*line 返回直线的交点，请保证这两条直线相交，可先用 line&line 判断。

## 线段 (seg)

线段表示为 {s,t}，其中 s 和 t 为线段的两个端点，**这与直线不同**。

- ~seg 返回线段所在的直线
- seg&vec 返回 0/1，表示这个点是否在线段上。
- seg&seg 返回 0/1/2，表示两条线段关系：无重叠部分(0)，严格相交(1)，有重叠部分但不严格相交(2)。严格相交指两线段有唯一交点，且交点不与任何一个线段的端点重合。

## 多边形 (poly)

由一个 vector<vec> 构成，其中从 0 开始编号，逆时针存储多边形上的点。

- C() 返回多边形周长

- S() 返回多边形面积

- 初始化函数 poly(vector<vec> s,int fl=0) 为凸包模板

  传入无序点集 vector<vec>s ，和参数 int fl (默认为 0 )。当 fl 为 0 时，为严格凸包（不存在三点共线），当 fl 为 1 时，为非严格凸包（所有在凸包边上的点都含有），求解出的凸包存入本 poly 结构体，其中的点逆时针排列。

- 初始化函数 poly(vector<line> s) 为半平面交模板

  **注意本半平面交模板不支持对解集不封闭的情况处理，如需处理，请手动添加外边框（注意大小足够大）**

  传入无序有向直线集 vector<line> s。将半平面（有向直线的逆时针方向）交成的凸多边形存入本 poly 结构体，其中的点逆时针排列。如果解集为空，则存入的凸多边形点集为空。

- poly+poly 返回 poly 表示两个凸多边形的闵可夫斯基和

- pair<poly,poly> split(line l) 返回凸多边形由直线 l 分割后的结果

  传入直线 line l。返回两个凸多边形，pair 的 first 为分割后在直线逆时针方向的凸多边形，second 为分割后在直线顺时针方向的凸多边形。如果某一个方向上无原凸多边形的任何一个点，那么 pair 中对应位置的 poly 点集为空。

  ## 杂项

  - 自适应辛普森积分：db simpson(db l,db r,db eps,int step,db (*f)(db x))

    返回定积分 $\int_l^r f(x) dx$ 的近似值，精度要求为绝对精度 eps，最少迭代层数为 step。

    通常来说 eps 需要比需求的精度稍小一些，程序至少会递归 $2^{step}$ 次， step 建议取一个比较小的值，或根据 $ f$ 函数的时间复杂度估算。

```cpp
#include <bits/stdc++.h>

typedef long double db;

using namespace std;

const db eps=1e-9; //
const db pi=acos(-1);

int sgn(db x){return x<-eps?-1:x>eps;}
struct vec
{
	db x,y;
	void read(){cin>>x>>y;}
	vec operator+(vec b)const{return {x+b.x,y+b.y};}
	vec operator-(vec b)const{return {x-b.x,y-b.y};}
	vec operator*(db k)const{return {k*x,k*y};}
	db operator^(vec b)const{return x*b.x+y*b.y;}
	db operator*(vec b)const{return x*b.y-y*b.x;}
	bool operator==(vec b)const{return !sgn(x-b.x)&&!sgn(y-b.y);}
	bool operator!=(vec b)const{return sgn(x-b.x)||sgn(y-b.y);}
	bool operator<(vec b)const{return sgn(x-b.x)?x<b.x:sgn(y-b.y)==-1;}
	vec rot(db d){vec w={sin(d),cos(d)};return {*this*w,*this^w};}
	vec rot90(int z){return {-y*z,x*z};}
	db d(){return sqrt(x*x+y*y);}
};
struct line
{
	vec s,v;
	bool operator&(vec t){return !sgn(v*(t-s));}
	int operator|(vec t){return sgn(v*(t-s));}
	db operator%(vec t){return abs((t-s)*v)/v.d();}
	int operator&(line b){return !sgn(v*b.v)?(sgn((b.s-s)*v)?0:2):1;}
	vec operator*(line b){return s+v*(((s-b.s)*b.v)/(b.v*v));}
};
struct seg
{
	vec s,t;
	line operator~(){return {s,t-s};}
	bool operator&(vec u){return !sgn((t-s)*(u-s))&&sgn((u-s)^(u-t))<=0;}
	int operator&(seg b)
	{
		if(*this&b.s||*this&b.t||b&s||b&t)return 2; // Unstrick Situation
		return sgn((t-s)*(b.s-s))*sgn((t-s)*(b.t-s))==-1
		&&sgn((b.t-b.s)*(s-b.s))*sgn((b.t-b.s)*(t-b.s))==-1;
	}
};
struct poly
{
	vector<vec> a;
	poly(){};
	poly(vector<vec> s,int fl=0)
	{
		vector<vec>b;
		sort(s.begin(),s.end());
		for(auto t:s)
		{
			while(a.size()>=2)
			{
				vec u=a[a.size()-2],v=a[a.size()-1];
				if(sgn((v-u)*(t-v))<=-fl)a.pop_back();else break;
			}
			a.push_back(t);
		}
		reverse(s.begin(),s.end());
		for(auto t:s)
		{
			while(b.size()>=2)
			{
				vec u=b[b.size()-2],v=b[b.size()-1];
				if(sgn((v-u)*(t-v))<=-fl)b.pop_back();else break;
			}
			b.push_back(t);
		}
		for(int i=1;i<(int)b.size()-1;++i)
			a.push_back(b[i]);
	}
	poly(vector<line> s)
	{
		deque<line>q;
		sort(s.begin(),s.end(),[](line a,line b){
			db d1=atan2(a.v.y,a.v.x),d2=atan2(b.v.y,b.v.x);
			return sgn(d1-d2)?d1<d2:(b|a.s)==1; 
		});
		for(auto t:s)
		{
			if(!q.empty()&&sgn(q.back().v*t.v)==0&&sgn(q.back().v^t.v)==1)continue;
			while(q.size()>1&&(t|q.back()*q[q.size()-2])<=0)q.pop_back();
			while(q.size()>1&&(t|q[0]*q[1])<=0)q.pop_front();
			if(!q.empty()&&sgn(q.back().v*t.v)==0&&(t|q.back().s)<=0)return;
			q.push_back(t);
		}
		while(q.size()>1&&(q[0]|q.back()*q[q.size()-2])<=0)q.pop_back();
		while(q.size()>1&&(q.back()|q[0]*q[1])<=0)q.pop_front();
		if(sgn(q.back().v*q[0].v)<=0)return;
		for(int i=0;i<q.size();++i)
			a.push_back(q[i]*q[(i+1)%q.size()]);
	}
	poly operator+(poly b)
	{
		int i,sa=0,sb=0,u,v;
		for(i=0;i<a.size();++i)if(a[i]<a[sa])sa=i;
		for(i=0;i<b.a.size();++i)if(b.a[i]<b.a[sb])sb=i;
		vec s=a[sa]+b.a[sb];u=sa;v=sb;poly ans;
		while(u!=-1||v!=-1)
		{
			if(v==-1||(u!=-1&&
			sgn((a[(u+1)%a.size()]-a[u])*(b.a[(v+1)%b.a.size()]-b.a[v]))>=0
			))
			{
				s=s+a[(u+1)%a.size()]-a[u];
				if((u=(u+1)%a.size())==sa)u=-1;
			}
			else
			{
				s=s+b.a[(v+1)%b.a.size()]-b.a[v];
				if((v=(v+1)%b.a.size())==sb)v=-1;
			}
			ans.a.push_back(s);
		}
		return ans;
	}
	db C(){db ans=0;for(int i=0;i<a.size();++i)ans+=(a[i]-a[(i+1)%a.size()]).d();return ans;}
	db S(){db ans=0;for(int i=0;i<a.size();++i)ans+=0.5*(a[i]*a[(i+1)%a.size()]);return ans;}
	pair<poly,poly> split(line l)
	{
		int i,n=a.size(),fl=0,fr=0;
		vector<int> b(n);for(i=0;i<n;++i){b[i]=l|a[i];fl|=b[i]==1;fr|=b[i]==-1;}
		if(!fr)return {*this,{}};if(!fl)return {{},*this};
		poly A,B;
		for(i=0;b[i]==1;++i);
		for(;b[i]!=1;i=(i+1)%n);
		A.a.push_back(~seg{a[(i+n-1)%n],a[i]}*l);
		for(;b[i]==1;i=(i+1)%n)A.a.push_back(a[i]);
		A.a.push_back(~seg{a[(i+n-1)%n],a[i]}*l);
		if(b[i]==0)i=(i+1)%n;
		B.a.push_back(~seg{a[(i+n-1)%n],a[i]}*l);
		for(;b[i]==-1;i=(i+1)%n)B.a.push_back(a[i]);
		B.a.push_back(~seg{a[(i+n-1)%n],a[i]}*l);
		return {A,B};
	}
};
db simpson(db l,db r,db eps,int step,db (*f)(db x))
{
	auto calc=[&](db l,db r)
	{
		db m=(l+r)/2;
		return (4*f(m)+f(l)+f(r))*(r-l)/6;
	};
	db m=(l+r)/2,F=calc(l,r),Fl=calc(l,m),Fr=calc(m,r),t;
	if(abs(t=Fl+Fr-F)<15*eps&&step<0)return Fl+Fr+t/15;
	return simpson(l,m,eps/2,step-1,f)+simpson(m,r,eps/2,step-1,f);
}
struct dynamic_convex
{
	set<vec>u,d;
	bool chk(vec v)
	{
		if(u.empty())return 0;
		auto r=u.lower_bound(v);
		if(r==u.begin()&&*r==v)return 1;
		if(r==u.begin()||r==u.end()||sgn((*r-*prev(r))*(v-*prev(r)))>0)return 0;
		r=d.lower_bound(v);
		if(r==d.begin()&&*r==v)return 1;
		return !(r==d.begin()||r==d.end()||sgn((*r-*prev(r))*(v-*prev(r)))<0);
	}
	void ins(vec v)
	{
		auto r=u.lower_bound(v);
		if(r==u.begin()||r==u.end()||sgn((*r-*prev(r))*(v-*prev(r)))>0)
		{
			auto t=u.insert(v).first;
			while(r!=u.end()&&next(r)!=u.end()&&sgn(((*r)-v)*(*next(r)-*r))>=0){r=next(r);u.erase(prev(r));}
			if(t!=u.begin())
			{
				auto l=prev(t);
				while(l!=u.begin()&&sgn((*l-*prev(l))*(v-*l))>=0){l=prev(l);u.erase(next(l));}
			}
		}
		r=d.lower_bound(v);
		if(r==d.begin()||r==d.end()||sgn((*r-*prev(r))*(v-*prev(r)))<0)
		{
			auto t=d.insert(v).first;
			while(r!=d.end()&&next(r)!=d.end()&&sgn(((*r)-v)*(*next(r)-*r))<=0){r=next(r);d.erase(prev(r));}
			if(t!=d.begin())
			{
				auto l=prev(t);
				while(l!=d.begin()&&sgn((*l-*prev(l))*(v-*l))<=0){l=prev(l);d.erase(next(l));}
			}
		}
	}
};

```

# 图论

```cpp
#include <bits/stdc++.h>

#define N 100005 //
#define M 200005 //

typedef long long ll;

using namespace std;

struct dijk
{
	const ll inf=0x3f3f3f3f3f3f3f3fll;

	ll d[N];
	bool vis[N];
	vector<pair<int,int>>g[N];
	void init(int n=N-5){fill(d,d+n+1,inf);for(int i=0;i<=n;++i){vis[i]=0;g[i].clear();}}
	void adde(int u,int v,int w){g[u].push_back({v,w});}
	ll sol(int n,int s,int t)
	{
		priority_queue<pair<ll,int>>q;
		d[s]=0;q.push({0,s});
		while(!q.empty())
		{
			int u=q.top().second;q.pop();
			if(vis[u])continue;vis[u]=1;
			for(auto [v,w]:g[u])
				if(d[v]>d[u]+w)
				{
					d[v]=d[u]+w;
					q.push({-d[v],v});
				}
		}
		for(int i=0;i<=n;++i)if(d[i]==inf)d[i]=-1;
		return d[t];
	}
};

struct maxflow
{
	const int inf=0x3f3f3f3f; //
	int n,en,d[N],q[N],head[N],cur[N];
	struct edge{int u,v,w,nxt;}e[M*2];
	void _adde(int u,int v,int w){e[++en]={u,v,w,head[u]};head[u]=en;}
	void adde(int u,int v,int w){_adde(u,v,w);_adde(v,u,0);}
	void init(int nn){n=nn;en=1;for(int i=0;i<=n;++i)head[i]=0;}
	bool bfs(int s,int t)
	{
		int i,hd,tl,u,v;
		for(i=0;i<=n;++i){cur[i]=head[i];d[i]=inf;}
		d[q[hd=tl=1]=s]=0;
		while(hd<=tl)
			for(i=head[u=q[hd++]];i;i=e[i].nxt)
				if(e[i].w&&d[v=e[i].v]>d[u]+1)
					d[q[++tl]=v]=d[u]+1;
		return d[t]!=inf;
	}
	int dfs(int u,int t,int fw)
	{
		int k,v,sm=0;
		if(u==t)return fw;
		for(int& i=cur[u];i;i=e[i].nxt)
			if(e[i].w&&d[v=e[i].v]==d[u]+1&&(k=dfs(v,t,min(e[i].w,fw-sm))))
			{
				e[i].w-=k;e[i^1].w+=k;sm+=k;
				if(sm==fw)break;
			}
		return sm;
	}
	int sol(int s,int t)
	{
		int ans=0;
		while(bfs(s,t))ans+=dfs(s,t,inf);
		return ans;
	}
};

struct costflow
{
	ll d[N],ans;
	bool inq[N];
	int n,en,head[N],cur[N],q[N*10];
	const ll inf=0x3f3f3f3f3f3f3f3fll;
	struct edge{int u,v,w,c,nxt;}e[M*2];
	void init(int nn){n=nn;ans=0;en=1;for(int i=0;i<=n;++i)head[i]=0;}
	void _adde(int u,int v,int w,int c){e[++en]={u,v,w,c,head[u]};head[u]=en;}
	void adde(int u,int v,int w,int c){_adde(u,v,w,c);_adde(v,u,0,-c);}
	bool spfa(int s,int t)
	{
		int i,u,v,hd=1,tl=1;
		fill(d,d+n+1,inf);
		d[q[1]=s]=0;
		while(hd<=tl)
		{
			inq[u=q[hd++]]=0;
			for(i=head[u];i;i=e[i].nxt)
				if(e[i].w&&d[v=e[i].v]>d[u]+e[i].c)
				{
					d[v]=d[u]+e[i].c;
					if(!inq[v])q[++tl]=v;
				}
		}
		return d[t]!=inf;
	}
	ll dfs(int u,int t,ll fw)
	{
		int v;ll k,sm=0;
		if(u==t)return fw;
		inq[u]=1;
		for(int& i=cur[u];i;i=e[i].nxt)
			if(e[i].w&&d[v=e[i].v]==d[u]+e[i].c&&!inq[v]&&(k=dfs(v,t,min(1ll*e[i].w,fw-sm))))
			{
				e[i].w-=k;e[i^1].w+=k;
				sm+=k;ans+=k*e[i].c;
				if(sm==fw)break;
			}
		inq[u]=0;
		return sm;
	}
	pair<ll,ll> sol(int s,int t)
	{
		ll fl=0;
		while(spfa(s,t))
		{
			for(int i=0;i<=n;++i)cur[i]=head[i];
			fl+=dfs(s,t,inf);
		}
		return {fl,ans};
	}
};

struct dijcostflow
{
	ll d[N],h[N];
	bool inq[N],vis[N];
	int n,en,dep[N],head[N],cur[N],q[N*10+M];
	const ll inf=0x3f3f3f3f3f3f3f3fll;
	struct edge{int u,v,w,c,nxt;}e[M*2];
	void init(int nn){n=nn;en=1;for(int i=0;i<=n;++i)head[i]=0;}
	void _adde(int u,int v,int w,int c){e[++en]={u,v,w,c,head[u]};head[u]=en;}
	void adde(int u,int v,int w,int c){_adde(u,v,w,c);_adde(v,u,0,-c);}
	void spfa(int s)
	{
		int i,u,v,hd=1,tl=1;
		fill(h,h+n+1,inf);
		h[q[1]=s]=0;
		while(hd<=tl)
		{
			inq[u=q[hd++]]=0;
			for(i=head[u];i;i=e[i].nxt)
				if(e[i].w&&h[v=e[i].v]>h[u]+e[i].c)
				{
					h[v]=h[u]+e[i].c;
					if(!inq[v])q[++tl]=v;
				}
		}
	}
	bool dij(int s,int t)
	{
		int i,u,v;
		priority_queue<pair<ll,int>>q;
		for(i=0;i<=n;++i){d[i]=inf;vis[i]=0;}
		d[s]=0;q.push({0,s});
		while(!q.empty())
		{
			u=q.top().second;q.pop();
			if(vis[u])continue;vis[u]=1;
			for(i=head[u];i;i=e[i].nxt)
				if(e[i].w&&d[v=e[i].v]>d[u]+(e[i].c+h[u]-h[v]))
				{
					d[v]=d[u]+(e[i].c+h[u]-h[v]);
					q.push({-d[v],v});
				}
		}
		return d[t]!=inf;
	}
	bool bfs(int s,int t)
	{
		int i,u,v,hd=1,tl=1;
		fill(dep,dep+n+1,N);dep[q[1]=s]=0;
		while(hd<=tl)
			for(i=head[u=q[hd++]];i;i=e[i].nxt)
				if(e[i].w&&h[v=e[i].v]==h[u]+e[i].c&&dep[v]>dep[u]+1)
					dep[q[++tl]=v]=dep[u]+1;
		return dep[t]!=N;
	}
	ll dfs(int u,int t,ll fw)
	{
		int v;ll k,sm=0;
		if(u==t)return fw;
		for(int& i=cur[u];i;i=e[i].nxt)
			if(e[i].w&&h[v=e[i].v]==h[u]+e[i].c&&dep[v]==dep[u]+1&&(k=dfs(v,t,min(1ll*e[i].w,fw-sm))))
			{
				e[i].w-=k;e[i^1].w+=k;sm+=k;
				if(sm==fw)break;
			}
		return sm;
	}
	pair<ll,ll> sol(int s,int t)
	{
		spfa(s);
		ll fl=0,vl=0;
		while(dij(s,t))
		{
			for(int i=0;i<=n;++i)if(d[i]!=inf)h[i]+=d[i];
			while(bfs(s,t))
			{
				for(int i=0;i<=n;++i)cur[i]=head[i];
				ll x=dfs(s,t,inf);fl+=x;vl+=x*h[t];
			}
		}
		return {fl,vl};
	}
};

struct dominator_tree
{
	vector<int>g[N],h[N],q[N];
	int cnt,f[N],fa[N],in[N],fm[N],dfn[N],sdm[N],idm[N];
	void init(int n){cnt=0;for(int i=0;i<=n;++i){f[i]=in[i]=sdm[i]=i;dfn[i]=0;g[i].clear();h[i].clear();q[i].clear();}}
	void adde(int u,int v){g[u].push_back(v);h[v].push_back(u);}
	void find(int u)
	{
		if(u==f[u])return;
		find(f[u]);if(dfn[sdm[in[u]]]>dfn[sdm[in[f[u]]]])in[u]=in[f[u]];f[u]=f[f[u]];
	}
	void dfs(int u)
	{
		fm[dfn[u]=++cnt]=u;
		for(auto v:g[u])if(!dfn[v]){fa[v]=u;dfs(v);}
	}
	vector<int> sol(int n,int s)
	{
		dfs(s);
		for(int k=n;k>=2;--k)
		{
			int u=fm[k];
			for(auto v:q[u]){find(v);idm[v]=in[v];}
			for(auto v:h[u])
			{
				if(dfn[v]<dfn[sdm[u]]&&dfn[sdm[u]]>dfn[v])sdm[u]=v;
				if(dfn[v]>dfn[u])
				{
					find(v);
					if(dfn[sdm[u]]>dfn[sdm[in[v]]])sdm[u]=sdm[in[v]];
				}
			}
			q[sdm[u]].push_back(u);f[u]=fa[u];
		}
		for(auto v:q[1]){find(v);idm[v]=in[v];}
		for(int k=2;k<=n;++k)
		{
			int u=fm[k];
			if(sdm[u]!=sdm[idm[u]])idm[u]=idm[idm[u]];
			else idm[u]=sdm[u];
		}
		vector<int>ans;
		for(int i=0;i<=n;++i)ans.push_back(idm[i]);
		return ans;
	}
};




```

# 数论

```cpp
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


```

# 多项式

```cpp
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


```

# 数据结构

```cpp
#include <bits/stdc++.h>

#define N 100005

using namespace std;

struct treeintree
{
	#define M 100 //log^2
	#define lson l,m,ls[rt]
	#define rson m+1,r,rs[rt]
	#define lowbit(x) ((x)&-(x))

	int n,cnt,tr[N*M],ls[N*M],rs[N*M],rti[N];
	void init(int nn)
	{
		n=nn;cnt=0;
		for(int i=0;i<=n*M;++i)tr[i]=ls[i]=rs[i]=0;
		for(int i=0;i<=n;++i)rti[i]=0;
	}
	void up(int rt){tr[rt]=tr[ls[rt]]+tr[rs[rt]];}
	void add(int a,int b,int l,int r,int& rt)
	{
		if(!rt)rt=++cnt;
		if(l==r){tr[rt]+=b;return;}
		int m=(l+r)>>1;
		if(a<=m)add(a,b,lson);
		else add(a,b,rson);
		up(rt);
	}
	int que(int a,int b,int l,int r,int rt)
	{
		if(!rt)return 0;
		if(a<=l&&r<=b)return tr[rt];
		int m=(l+r)>>1,ans=0;
		if(a<=m)ans=que(a,b,lson);
		if(b>m)ans+=que(a,b,rson);
		return ans;
	}
	void add(int x,int y,int w)
	{for(int i=x;i<=n;i+=lowbit(i))add(y,w,1,n,rti[i]);}
	int que(int l1,int r1,int l2,int r2)
	{
		int ans=0;
		for(int i=r1;i;i-=lowbit(i))ans+=que(l2,r2,1,n,rti[i]);
		for(int i=l1-1;i;i-=lowbit(i))ans-=que(l2,r2,1,n,rti[i]);
		return ans;
	}

	#undef lson
	#undef rson
};
struct basis
{
	int sz,w[N+1];
	void add(int x)
	{
		for(int i=N;i>=0;--i)
			if(x&(1<<i))
			{
				if(!w[i]){w[i]=x;++sz;break;}
				else x^=w[i];
			}
	}
	int que(int x)
	{
		for(int i=N;i>=0;--i)
			if(x&(1<<i))
			{
				if(!w[i])return 0;
				x^=w[i];
			}
		return 1;
	}
	basis operator+(basis b)
	{
		for(int i=N;i>=0;--i)
			if(w[i])b.add(w[i]);
		return b;
	}
};
```

# 字符串

```cpp
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

```

# 对拍

```
g++ data.cpp -o data -O2
g++ my.cpp -o my -O2
g++ std.cpp -o std -O2

((cnt=1))

while true
do
	./data > in
	./my < in > out
	./std < in > ans
	if diff out ans; then
		printf "# $((cnt++)) Accepted\n";
	else 
		notify-send "WA"
		break
	fi
done

```