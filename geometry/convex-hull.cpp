// 接在任一二维几何核后使用。
// fl = 0：严格凸包，不保留边上的共线点
// fl = 1：保留凸包边界上的共线点，要求凸包非退化
// 返回点按逆时针排列，首尾不重复
poly convex_hull(vector<vec> s,int fl=0)
{
	sort(s.begin(),s.end());
	s.erase(unique(s.begin(),s.end(),
		[](vec a, vec b){return a.x==b.x&&a.y==b.y;}),s.end());
	vector<vec>d,u;
	auto add=[&](vector<vec>& q,vec t)
	{
		while(q.size()>1&&ori(q[q.size()-2],q[q.size()-1],t)<=-fl)
			q.pop_back();
		q.push_back(t);
	};
	for(auto t:s)add(d,t);
	for(int i=(int)s.size()-1;i>=0;--i)add(u,s[i]);
	for(int i=1;i<(int)u.size()-1;++i)
		d.push_back(u[i]);
	return {d};
}
