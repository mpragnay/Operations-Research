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
    int a[n][m+n];
    vector<ld> b(n);
    vector<ld> artificial(n,0);
    vector<ld> sign(n,0);
    int k=0;
    cout<<"Enter coefficients of variables"<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<"Equation "<<i+1<<":"<<endl;
        for(int j=0;j<m;j++)
        {
            cout<<"Enter coefficient of x"<<j+1<<endl;
            cin>>a[i][j];
        }
        for(int j=m;j<n+m;j++)
        {
            a[i][j]=0;
            if(j==k+m)
            a[i][j]=1;
        }
        k++;
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
    if(max==0)
    {
    	for(int i=0;i<m;i++)
    	z[i]*=-1;
    }
    int add_col=0;
    vi artif;
    for(int i=0;i<sign.size();i++)
    {
    	if(sign[i]==1)
    	add_col++;
    	if(sign[i]!=2)
    	artif.pb(i);
    }
    
    //SIMPLEX TABLEAU PHASE 1
    vector<vector<ld>> table(n+1,vector<ld> (m+1+add_col,0));
    vi ord(m+add_col+n);
    for(int i=0;i<m+add_col+n;i++)
    ord[i]=i;
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<m;j++)
    	table[i][j]=a[i][j];
    	table[i][m+add_col]=b[i];
    }
    if(add_col)
    {
    	int jj=m;
	    for(int i=0;i<n;i++)
	    {
	    	if(sign[i]==1)
	    	{
	    		table[i][jj]=-1;
	    		jj++;
	    	}
	    }
    }
    for(int j=0;j<m;j++)
    {
    	int sum=0;
    	for(int k=0;k<artif.size();k++)
    	sum+=a[artif[k]][j];
    	table[n][j]=-sum;
    }
    if(add_col)
    {
    	for(int j=m;j<m+add_col;j++)
    	table[n][j]=1;
    	for(int k=0;k<artif.size();k++)
    	table[n][m+add_col]-=b[artif[k]];
    }
    cout<<"Phase 1 Initial Table:"<<endl;
    for(int i=0;i<n+1;i++)
    {
    	for(int j=0;j<m+add_col+1;j++)
    	cout<<table[i][j]<<' ';
    	cout<<endl;
    }
    
    
    
    
    
    
    //for(int i=0;i<m+add_col+n;i++)
    //cout<<ord[i]<<' ';
    //cout<<endl;
    int r=n+1,col=m+1+add_col;
    int itr=0;
    while(1)
    {
    	itr++;
    	lli pivr,pivc,tmp=1e9;
    	ld piv;
    	for(int j=0;j<col-1;j++)
    	{
    		if(table[n][j]<0)
    		{
    			if(table[n][j]<tmp)
    			{
    				//cout<<j<<' ';
    				pivc=j;
    				tmp=table[n][j];
    				//cout<<tmp<<endl;
    			}
    		}
    	}
    	//cout<<tmp<<endl;
    	//cout<<endl<<endl;
    	if(tmp==1e9)
    	break;
    	ld minr=1e9;
    	for(int i=0;i<r-1;i++)
    	{
    		if(table[i][pivc]>0)
    		{
    			if(table[i][col-1]/table[i][pivc]<minr)
    			{
    				pivr=i;
    				minr=table[i][col-1]/table[i][pivc];
    			}
    		}
    	}
    	piv=table[pivr][pivc];
    	swap(ord[pivc],ord[col+pivr-1]);
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
    	//cout<<pivr<<' '<<pivc<<endl;
    	
    }
    //PHASE 1 CHECK
    int infeasible=0;
    for(int i=0;i<artif.size();i++)
    {
    	int j;
    	for(j=0;j<m+add_col;j++)
    	{
    		if(ord[j]==m+add_col+artif[i])
    		break;
    	}
    	if(j==m+add_col)
    	{
    		infeasible=1;
    		break;
    	}
    	else
    	{
    		for(int k=0;k<n+1;k++)
    		table[k][j]=0;
    	}
    }
    //PHASE TRANSITION
    int unbound=0;
    if(infeasible)
    cout<<"Infeasible Solution"<<endl;
    else
    {
    	for(int i=0;i<m+add_col+1;i++)
    	table[n][i]=0;
    	for(int i=0;i<m;i++)
    	{
    		int j;
    		for(j=0;j<m+add_col+n;j++)
	    	{
	    		if(ord[j]==i)
	    		{
	    			if(j<m+add_col)
	    			{
	    				table[n][j]-=z[ord[j]];
	    			}
	    			else
	    			{
	    				j-=m+add_col;
	    				int mult=z[i];
	    				for(int k=0;k<m+add_col+1;k++)
	    				table[n][k]+=mult*table[j][k];
	    			}
	    			break;
	    		}
	    	}
    	}
    	cout<<"Phase 2 Initial Table:"<<endl;
	    for(int i=0;i<n+1;i++)
	    {
	    	for(int j=0;j<m+add_col+1;j++)
	    	cout<<table[i][j]<<' ';
	    	cout<<endl;
	    }
	    //PHASE 2 SIMPLEX TABLEAU
	    
	    
	    
	    
    	itr=0;
	    while(1)
	    {
	    	itr++;
	    	lli pivr,pivc,tmp=1e9;
	    	ld piv;
	    	for(int j=0;j<col-1;j++)
	    	{
	    		if(table[n][j]<0)
	    		{
	    			if(table[n][j]<tmp)
	    			{
	    				//cout<<j<<' ';
	    				pivc=j;
	    				tmp=table[n][j];
	    				//cout<<tmp<<endl;
	    			}
	    		}
	    	}
	    	//cout<<tmp<<endl;
	    	//cout<<endl<<endl;
	    	if(tmp==1e9)
	    	break;
	    	ld minr=1e9;
	    	for(int i=0;i<r-1;i++)
	    	{
	    		if(table[i][pivc]>0)
	    		{
	    			if(table[i][col-1]/table[i][pivc]<minr)
	    			{
	    				pivr=i;
	    				minr=table[i][col-1]/table[i][pivc];
	    			}
	    		}
	    	}
	    	if(minr==1e9)
	    	{
	    		cout<<"Solution is Unbounded"<<endl;
	    		unbound=1;
	    		break;
	    	}
	    	piv=table[pivr][pivc];
	    	swap(ord[pivc],ord[col+pivr-1]);
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
	    	//cout<<pivr<<' '<<pivc<<endl;
	    }
    }
    if(unbound==0)
    {
    	cout<<"Optimum solution is:"<<table[n][m+add_col]<<endl;
	    vector<long double> res(m,0);
	    for(int i=0;i<m+add_col+n;i++)
	    {
	    	//cout<<ord[i]<<' ';
	    	if(ord[i]<m)
	    	{
	    		if(i<m+add_col)
	    		{
	    			res[ord[i]]=table[0][i];
	    		}
	    		else
	    		{
	    			res[ord[i]]=table[i-m-add_col][m+add_col];
	    		}
	    	}
	    }
	    cout<<endl;
	    for(int i=0;i<m;i++)
	    {
	    	if(res[i]<0)
	    	{
	    		cout<<"Infeasible solution"<<endl;
	    		break;
	    	}
	    	cout<<"x"<<i+1<<"="<<res[i]<<',';
	    }
	    cout<<endl;
	    int alt=-1;
	    for(int i=0;i<m+add_col;i++)
	    {
	    	int chk=ord[i];
	    	int cont=0;
	    	for(int j=0;j<artif.size();j++)
	    	{
	    		if(artif[j]==chk-m-add_col)
	    		{
	    			cont=1;
	    			break;
	    		}
	    	}
	    	if(cont)
	    	continue;
	    	if(table[n][i]==0)
	    	{
	    		alt=i;
	    		break;
	    	}
	    }
	    if(alt!=-1)
	    cout<<"There exists an Alternative Optimum solution"<<endl;
    }
}