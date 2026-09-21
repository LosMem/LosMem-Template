// 接在任一二维几何核后使用。
// ins 只支持插点，chk 判断点是否在当前凸包内（含边界）。
// 重复点会被忽略；插入均摊 O(log n)，查询 O(log n)，不支持删除。
struct dynamic_convex
{
	set<vec>u,d,all;
	static bool inside(const set<vec>&h,vec p,int z)
	{
		if(h.empty())return 0;
		auto r=h.lower_bound(p);
		if(r==h.begin())return *r==p;
		if(r==h.end())return 0;
		return z*(~seg{*prev(r),*r}|p)<=0;
	}
	static void add(set<vec>&h,vec p,int z)
	{
		auto r=h.lower_bound(p);
		if(r!=h.begin()&&r!=h.end()&&z*(~seg{*prev(r),*r}|p)<=0)
			return;
		auto t=h.insert(p).first;
		while(r!=h.end()&&next(r)!=h.end())
		{
			auto nr=next(r);
			if(z*(~seg{p,*nr}|*r)>0)break;
			h.erase(r);r=nr;
		}
		while(t!=h.begin())
		{
			auto l=prev(t);
			if(l==h.begin())break;
			auto pl=prev(l);
			if(z*(~seg{*pl,p}|*l)>0)break;
			h.erase(l);
		}
	}
	void ins(vec p)
	{
		if(!all.insert(p).second)return;
		add(u,p,1);add(d,p,-1);
	}
	bool chk(vec p)const
	{
		return inside(u,p,1)&&inside(d,p,-1);
	}
};
