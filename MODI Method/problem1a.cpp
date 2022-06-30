#include <bits/stdc++.h>
#define mod 1000000007
#define IOS std::ios::sync_with_stdio(false); cin.tie(NULL);cout.tie(NULL);cout.precision(dbl::max_digits10);
#define pb push_back
#define mod 1000000007 //998244353
#define ll long long
#define lli long long int
#define ld long double
#define vi vector<int>
#define vll vector<long long>
#define mp make_pair
#define F first
#define S second
typedef std::numeric_limits< double > dbl;
using namespace std;
long long int binpow(long long int a, long long int b, long long int m) {a %= m;long long int res = 1; while (b > 0) {if (b & 1) res = res * a % m;a = a * a % m;b >>= 1;}return res;}

vector<vi> nwcr(vector<vi> table,vi supply,vi demand,int m,int n)
{
	vector<vi> bfs(m,vi (n,0));
	int i=0,j=0,cnt=0;
	while(1)
	{
		if(supply[i]<demand[j])
		{
			bfs[i][j]=supply[i];
			cnt++;
			supply[i]=0;
			demand[j]-=bfs[i][j];
			i++;
		}
		else
		{
			bfs[i][j]=demand[j];
			cnt++;
			demand[j]=0;
			supply[i]-=bfs[i][j];
			j++;
		}
		if(cnt==m+n-1)
		break;
	}
	return bfs;
}

vector<vi> lcm(vector<vi> table,vi supply,vi demand,int m,int n)
{
	vector<vi> bfs(m,vi (n,0));
	int i=0,j=0,cnt=0;
	while(1)
	{
		if(cnt==m+n-1)
		break;
		int i_min=0,j_min=0,mini=1e6,brk=1;
		for(int i=0;i<m;i++)
		{
			if(supply[i]==0)
			continue;
			for(int j=0;j<n;j++)
			{
				if(demand[j]==0)
				continue;
				if(table[i][j]<mini)
				{
					i_min=i;j_min=j;mini=table[i][j];
					brk=0;
				}
			}
		}
		if(brk)
		break;
		//cout<<i_min<<' '<<j_min<<endl;
		if(supply[i_min]<demand[j_min])
		{
			bfs[i_min][j_min]=supply[i_min];
			cnt++;
			demand[j_min]-=supply[i_min];
			supply[i_min]-=supply[i_min];
			if(demand[j_min])
			{
				vi col(m);
				for(int i=0;i<m;i++)
				col[i]=table[i][j_min];
				sort(col.begin(),col.end());
				//int itr=1;
				for(int itr=1;itr<m;itr++)
				{
					if(demand[j_min]==0)
					break;
					for(int i=0;i<m;i++)
					{
						if(supply[i]==0)
						continue;
						if(table[i][j_min]==col[itr])
						{
							int subtract=min(supply[i],demand[j_min]);
							bfs[i][j_min]=subtract;
							cnt++;
							supply[i]-=subtract;
							demand[j_min]-=subtract;
							break;
						}
					}
				}
			}
		}
		
		else
		{
			bfs[i_min][j_min]=demand[j_min];
			cnt++;
			//cout<<demand[j_min]<<' '<<supply[i_min]<<endl;
			supply[i_min]-=demand[j_min];
			demand[j_min]-=demand[j_min];
			
			if(supply[i_min])
			{
				vi row(n);
				for(int j=0;j<n;j++)
				row[j]=table[i_min][j];
				sort(row.begin(),row.end());
				//int itr=1;
				for(int itr=1;itr<n;itr++)
				{
					if(supply[i_min]==0)
					break;
					for(int i=0;i<n;i++)
					{
						if(demand[i]==0)
						continue;
						if(table[i_min][i]==row[itr])
						{
							int subtract=min(supply[i_min],demand[i]);
							bfs[i_min][i]=subtract;
							cnt++;
							supply[i_min]-=subtract;
							demand[i]-=subtract;
							break;
						}
					}
				}
			}
		}
	}
	return bfs;
}
 
int32_t main() 
{
	int n,m;
	cout<<"Enter number of sources:"<<endl;
	cin>>m;
	cout<<"Enter number of destinations:"<<endl;
	cin>>n;
    vector<vi> table(m,vi (n,0));
    vi supply(m);
    vi demand(n);
    for(int i=0;i<m;i++)
    {
    	for(int j=0;j<n;j++)
    	{
    		cout<<"Enter cost from "<<i+1<<" to "<<j+1<<":"<<endl;
    		cin>>table[i][j];
    	}
    }
    for(int i=0;i<m;i++)
    {
    	cout<<"Enter supply for "<<i+1<<" :"<<endl;
    	cin>>supply[i];
    }
    for(int i=0;i<n;i++)
    {
    	cout<<"Enter demand for "<<i+1<<" :"<<endl;
    	cin>>demand[i];
    }
    vector<vi> bfs=lcm(table,supply,demand,m,n);
    cout<<"The BFS using Least Cost Method is:"<<endl;
    for(int i=0;i<m;i++)
    {
    	for(int j=0;j<n;j++)
    	{
    		cout<<bfs[i][j]<<' ';
    	}
    	cout<<endl;
    }
    IOS;
}