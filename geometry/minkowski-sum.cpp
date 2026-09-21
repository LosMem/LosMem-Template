// 接在任一二维几何核后使用。
// 要求 P、Q 均为非退化凸多边形，顶点按逆时针排列且首尾不重复。
poly minkowski(poly P,poly Q)
{
	if(P.a.empty()||Q.a.empty())return {};
	auto fix=[](vector<vec>& p)
	{
		int k=min_element(p.begin(),p.end())-p.begin();
		rotate(p.begin(),p.begin()+k,p.end());
	};
	fix(P.a);fix(Q.a);
	int i=0,j=0,n=P.a.size(),m=Q.a.size();
	vec u=P.a[0]+Q.a[0];
	vector<vec>c;
	c.push_back(u);
	while(i<n||j<m)
	{
		vec ea=i<n?P.a[(i+1)%n]-P.a[i]:vec{0,0};
		vec eb=j<m?Q.a[(j+1)%m]-Q.a[j]:vec{0,0};
		if(j==m||(i<n&&ea*eb>=0)){u=u+ea;++i;}
		else{u=u+eb;++j;}
		c.push_back(u);
	}
	c.pop_back();
	return {c};
}
