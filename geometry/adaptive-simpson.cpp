// 接在二维浮点几何核后使用。
db simpson_rec(db l,db r,db e,int d,db fl,db fm,db fr,db S,db (*f)(db))
{
	db m=(l+r)/2,x=(l+m)/2,y=(m+r)/2,fx=f(x),fy=f(y);
	db L=(fl+4*fx+fm)*(m-l)/6,R=(fm+4*fy+fr)*(r-m)/6,t=L+R-S;
	if(abs(t)<15*e&&d<=0)return L+R+t/15;
	return simpson_rec(l,m,e/2,d-1,fl,fx,fm,L,f)
			 +simpson_rec(m,r,e/2,d-1,fm,fy,fr,R,f);
}
// 自适应辛普森积分：f 应在 [l,r] 上连续且足够光滑，eps 为误差阈值。
// step 为初始强制递归层数而非最大深度，奇异或不连续函数可能递归过深。
db simpson(db l,db r,db eps,int step,db (*f)(db x))
{
	db m=(l+r)/2,fl=f(l),fm=f(m),fr=f(r),S=(fl+4*fm+fr)*(r-l)/6;
	return simpson_rec(l,r,eps,step,fl,fm,fr,S,f);
}
