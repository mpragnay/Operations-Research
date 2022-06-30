#include <bits/stdc++.h>
#define pb push_back
#define ll long long
#define lli long long int
#define ld long double
#define vi vector<int>
#define vll vector<long long>
#define mp make_pair

using namespace std;

int main()
{
    int n,m;
    cout<<"Enter number of equations:"<<endl;
    cin>>n;
    cout<<"Enter number of variables:"<<endl;
    cin>>m;
    int a[n][m];
    vector<ld> b(n);
    vector<ld> sign(n,0);
    cout<<"Enter coefficients of variables"<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<"Equation "<<i+1<<":"<<endl;
        for(int j=0;j<m;j++)
        {
            cout<<"Enter coefficient of x"<<j+1<<endl;
            cin>>a[i][j];
        }
        cout<<"Enter sign(If greater 1 else if smaller then 2 else 0):"<<endl;
        cin>>sign[i];
        cout<<"Enter constant term:"<<endl;
        cin>>b[i];
    }
    int max;
    cout<<"Enter the equation to optimize:"<<endl;
    vi z(m);
    for(int i=0;i<m;i++)
    cin>>z[i];
    cout<<"Enter 0 to minimize and 1 to maximize"<<endl;
    cin>>max;
    int answer_multi=1;
    if(max==0)
    {
        answer_multi=-1;
    	for(int i=0;i<m;i++)
    	z[i]*=-1;
    }
    for(int i=0;i<sign.size();i++)
    {
    	if(sign[i]==1)
    	{
            sign[i]=2;
            b[i]*=-1;
            for(int j=0;j<m;j++)
            a[i][j]*=-1;
        }
    }
    
    //Dual Simplex Method
    vector<vector<ld>> table(n+1,vector<ld> (m+1,0));
    vi ord(m+n);
    for(int i=0;i<m+n;i++)
    ord[i]=i;
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<m;j++)
    	table[i][j]=a[i][j];
    	table[i][m]=b[i];
    }
    for(int j=0;j<m;j++)
    {
    	table[n][j]=-z[j];
    }
    cout<<"Initial Table:"<<endl;
    for(int i=0;i<n+1;i++)
    {
    	for(int j=0;j<m+1;j++)
    	cout<<table[i][j]<<' ';
    	cout<<endl;
    }
    
    
    
    
    
    
    //for(int i=0;i<m+add_col+n;i++)
    //cout<<ord[i]<<' ';
    cout<<endl;
    int r=n+1,col=m+1;
    int itr=0;
    int unbound=0;
    while(1)
    {
    	itr++;
    	lli pivr,pivc;
    	ld piv;
    	long double tmp=1e9;
    	for(int j=0;j<r-1;j++)
    	{
    		if(table[j][m]<0)
    		{
    			if(table[j][m]<tmp)
    			{
    				//cout<<j<<' ';
    				pivr=j;
    				tmp=table[j][m];
    				//cout<<tmp<<endl;
    			}
    		}
    	}
    	//cout<<tmp<<endl;
    	//cout<<endl<<endl;
    	if(tmp==1e9)
    	{
    		break;
    	}
    	ld minr=1e9;
    	for(int i=0;i<col-1;i++)
    	{
    		if(table[pivr][i]<0 && table[n][i]>=0)
    		{
    			if(abs(table[n][i]/table[pivr][i])<minr)
    			{
    				pivc=i;
    				minr=abs(table[n][i]/table[pivr][i]);
    			}
    		}
    	}
    	if(minr==1e9)
    	{
    		unbound=1;
    		break;
    	}
    	piv=table[pivr][pivc];
    	//cout<<"pivot:"<<piv<<endl;
    	if(ord[n+pivc]<n)
    	cout<<"s"<<ord[n+pivc]+1<<" is entering variable for iteration "<<itr<<endl;
    	else
    	cout<<"x"<<ord[n+pivc]-n+1<<" is entering variable for iteration "<<itr<<endl;
    	if(ord[pivr]<n)
    	cout<<"s"<<ord[pivr]+1<<" is departing variable for iteration "<<itr<<endl;
    	else
    	cout<<"x"<<ord[pivr]-n+1<<" is departing variable for iteration "<<itr<<endl;
    	swap(ord[pivr],ord[n+pivc]);
    	//cout<<pivr<<' '<<pivc<<endl;
    	//cout<<"minimum ratio:"<<minr<<endl;
    	for(int i=0;i<r;i++)
    	{
    		if(i==pivr)
    		continue;
    		for(int j=0;j<col;j++)
    		{
    			if(j==pivc)
    			continue;
    			table[i][j]=(table[i][j]*piv-table[pivr][j]*table[i][pivc])/piv;
    		}
    	}
    	for(int i=0;i<col;i++)
    	{
    		if(i==pivc)
    		continue;
    		table[pivr][i]/=piv;
    	}
    	for(int i=0;i<r;i++)
    	{
    		if(i==pivr)
    		continue;
    		table[i][pivc]/=piv;
    		table[i][pivc]=-table[i][pivc];
    	}
    	table[pivr][pivc]=1/table[pivr][pivc];
        cout<<"Objective value for iteration "<<itr<<" is:"<<-table[n][m]<<endl;
    	//cout<<pivr<<' '<<pivc<<endl;
    	//cout<<"iteration "<<itr<<endl;
	    //for(int i=0;i<n+1;i++)
	    //{
	    //	for(int j=0;j<m+1;j++)
	    //	cout<<table[i][j]<<' ';
	    //	cout<<endl;
	    //}
    }
    if(unbound)
    cout<<"Solution is unbounded"<<endl;
    vector<ld> x(m,0);
    vector<ld> s(n,0);
    for(int i=0;i<n;i++)
    {
    	if(ord[i]<n)
    	s[ord[i]]=table[i][m];
    	else
    	x[ord[i]-n]=table[i][m];
    }
    cout<<"Final Table:"<<endl;
    for(int i=0;i<n+1;i++)
    {
    	for(int j=0;j<m+1;j++)
    	cout<<table[i][j]<<' ';
    	cout<<endl;
    }
    int infeasible=0;
    for(int i=0;i<m;i++)
    {
    	if(x[i]<0)
    	{
    		infeasible=1;
    		break;
    	}
    }
    if(!infeasible)
    {
    	cout<<"Optimum Solution is:"<<-table[n][m]<<endl;
    	for(int i=0;i<m;i++)
    	cout<<"x"<<i+1<<'='<<x[i]<<endl;
    }
    else
    cout<<"Infeasible solution"<<endl;
}