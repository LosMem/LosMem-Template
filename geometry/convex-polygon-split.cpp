// 接在二维浮点几何核后使用。
// 用直线分割凸多边形，顶点按边界顺序排列；返回值分别为 (l|p)>=0 与 <=0 两侧。
// 切线或共边时零面积部分为空，退化或非凸输入不保证正确。
pair<poly,poly> split(const poly& P,line l)
{
	int i,n=P.a.size(),fl=0,fr=0;
	vector<int> b(n);for(i=0;i<n;++i){b[i]=l|P.a[i];fl|=b[i]==1;fr|=b[i]==-1;}
	if(!fr)return {P,{}};
	if(!fl)return {{},P};
	poly A,B;
	for(i=0;b[i]==1;++i);
	for(;b[i]!=1;i=(i+1)%n);
	A.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	for(;b[i]==1;i=(i+1)%n)A.a.push_back(P.a[i]);
	A.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	if(b[i]==0)i=(i+1)%n;
	B.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	for(;b[i]==-1;i=(i+1)%n)B.a.push_back(P.a[i]);
	B.a.push_back(~seg{P.a[(i+n-1)%n],P.a[i]}*l);
	return {A,B};
}
