/*
静态区间最值 ST 表：
  ST<T> mn(a);                    // 默认 less<T>，查询最小值
  ST<T,greater<T>> mx(a);         // 查询最大值
  mn.que(l,r);                    // 查询 0-based 闭区间 [l,r]

比较器约定：cmp(x,y)==true 表示 x 比 y 更优，合并时选择 x；否则选择 y。
自定义类型示例：
  auto cmp=[](const Node&x,const Node&y){return x.val>y.val;};
  ST<Node,decltype(cmp)> s(a,cmp); // 按 val 查询最大值

要求：a 非空，且 0<=l<=r<a.size()；cmp 应满足严格弱序。
复杂度：预处理 O(n log n)，单次查询 O(1)，空间 O(n log n)。
*/
template<class T,class C=less<T>>
struct ST
{
	C cmp;
	vector<vector<T>>st;
	T f(T a,T b){return cmp(a,b)?a:b;}
	ST(vector<T> a,C c={}):cmp(c)
	{
		int i,k,n=a.size(),m=__lg(n)+1;
		st.assign(m,a);
		for(k=1;k<m;++k)
			for(i=0;i+(1<<k)<=n;++i)
				st[k][i]=f(st[k-1][i],st[k-1][i+(1<<(k-1))]);
	}
	T que(int l,int r)
	{
		int k=__lg(r-l+1);
		return f(st[k][l],st[k][r-(1<<k)+1]);
	}
};
