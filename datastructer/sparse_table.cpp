/*
静态区间最值 ST 表，使用 0-based 闭区间 [l,r]：
  ST<T> mn;mn.init(a);            // 可全局声明，默认 less<T> 查询最小值
  ST<T,greater<T>> mx(a);         // 也可传数组构造，查询最大值
  mn.pos(l,r);                   // 返回最优元素下标，同值取最左下标
  mn.que(l,r);                   // 返回最优元素的值

比较器 cmp(x,y)==true 表示 x 比 y 更优，须满足严格弱序。
自定义类型示例：
  auto cmp=[](const Node&x,const Node&y){return x.val>y.val;};
  ST<Node,decltype(cmp)> s(cmp);   // 可全局声明，之后调用 s.init(a)
  // 也可 ST<Node,decltype(cmp)> s(a,cmp); 按 val 查询最大值

init 复制并保存原数组，可重复调用重建；建表后不要直接修改内部 a 或改变比较规则。
要求：初始化数组非空，初始化后才能查询，且 0<=l<=r<a.size()。
复杂度：预处理 O(n log n)，单次查询 O(1)，空间 O(n log n)。
*/
template<class T,class C=less<T>>
struct ST
{
	C cmp;
	vector<T>a;
	vector<vector<int>>st;

	int f(int x,int y)const
	{
		return cmp(a[y],a[x])?y:x;
	}
	ST(C c={}):cmp(c){}
	ST(const vector<T>& b,C c={}):cmp(c){init(b);}
	void init(const vector<T>& b)
	{
		a=b;
		int n=a.size(),m=__lg(n)+1;
		st.assign(m,vector<int>(n));
		iota(st[0].begin(),st[0].end(),0);
		for(int k=1;k<m;++k)
			for(int i=0;i+(1<<k)<=n;++i)
				st[k][i]=f(st[k-1][i],st[k-1][i+(1<<(k-1))]);
	}
	int pos(int l,int r)const
	{
		int k=__lg(r-l+1);
		return f(st[k][l],st[k][r-(1<<k)+1]);
	}
	T que(int l,int r)const
	{
		return a[pos(l,r)];
	}
};
