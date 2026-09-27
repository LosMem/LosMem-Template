template <class T>
struct compress
{
	vector<T>q;
	void add(T x)
	{q.push_back(x);}
	void build()
	{
		sort(q.begin(),q.end());
		q.erase(unique(q.begin(),q.end()),q.end());
	}
	int get(T x)
	{
		int k=lower_bound(q.begin(),q.end(),x)-q.begin();
		assert(k<q.size()&&q[k]==x);
		return k+1;
	}
	T operator[](int id)
	{return q[id-1];}
};