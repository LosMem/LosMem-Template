// 接在二维浮点几何核后使用。有向直线保留左侧。
// 交集应为有界、非退化的凸多边形；空集、无界或退化情况返回空多边形。
poly half_plane(vector<line> s)
{
	if(s.empty())return {};
	auto hf=[](vec v){return v.y<0||(v.y==0&&v.x<0);};
	auto par=[](line a,line b){return dir(a.v,b.v)==0;};
	sort(s.begin(),s.end(),[&](line a,line b)
	{
		int x=hf(a.v),y=hf(b.v);
		if(x!=y)return x<y;
		db c=a.v*b.v;
		return c?c>0:b.v*(a.s-b.s)>0;
	});
	deque<line>q;
	auto trim=[&](line t,bool ps)
	{
		while(q.size()>1)
		{
			line a=ps?q.back():q.front();
			line b=ps?q[q.size()-2]:q[1];
			if(par(a,b))return 0;
			if((t|(a*b))>0)break;
			if(ps)q.pop_back();
			else q.pop_front();
		}
		return 1;
	};
	for(auto t:s)
	{
		if(!q.empty()&&par(q.back(),t)&&(q.back().v^t.v)>0)
		{
			if((t|q.back().s)>=0)continue;
			q.pop_back();
		}
		if(!trim(t,1)||!trim(t,0))return {};
		q.push_back(t);
	}
	if(q.size()<3
	||!trim(q.front(),1)
	||!trim(q.back(),0)
	||q.size()<3
	||dir(q.back().v,q[0].v)<=0)
		return {};
	vector<vec>a;
	for(int i=0,n=q.size();i<n;++i)
		a.push_back(q[i]*q[(i+1)%n]);
	return {a};
}
