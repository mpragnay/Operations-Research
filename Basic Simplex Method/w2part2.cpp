#include <bits/stdc++.h>
#define pb push_back
#define ll long long
#define lli long long int
#define ld long double
#define vi vector<int>
#define vll vector<long long>
#define mp make_pair

using namespace std;

vector<ld> gs_algo(vector<vector<ld>> a,vector<ld> b)
{
    int n=b.size();
    vector<ld> x(n,0);

    for(int i=0;i<100;i++)
    {
        for(int j=0;j<n;j++)
        {
            ld sum=0;
            for(int k=0;k<n;k++)
            {
                if(k==j)
                continue;
                sum+=a[j][k]*x[k];
            }
            x[j]=b[j]-sum;x[j]/=a[j][j];
        }
    }
    return x;
}

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
    cout<<"Enter the equation to optimize:"<<endl;
    vi z(m);
    for(int i=0;i<m;i++)
    cin>>z[i];
    
    int add_col=0;
    for(int i=0;i<sign.size();i++)
    {
    	if(sign[i]==1)
    	add_col++;
    }

    vector<vector<ld>> res;
    vector<vector<ld>> bfs;
    string bitmask(m, 1); 
    bitmask.resize(n+m, 0); 
    do 
    {
        vector<vector<ld>> inp(n,vector<ld> (n,0));
        int l=0;
        for (int i = 0; i < n+m; ++i) 
        {
            if (bitmask[i]) 
            {
                continue;
            }
            for(int j=0;j<n;j++)
            {
                inp[j][l]=a[j][i];
            }
            l++;
        }
        vector<ld> x=gs_algo(inp,b);
        vector<ld> y(n+m,0);
        int j=0;
        bool add=1;
        for(int i=0;i<n+m;i++)
        {
            if(bitmask[i]==0)
            {
                if(x[j]<0)
                {
                    add=0;
                    break;
                }
                y[i]=x[j];
                j++;
            }
        }  
        if(add) 
        res.pb(y);   
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

    //SIMPLEX TABLEAU
    vector<vector<ld>> table(n+1,vector<ld> (m+1+add_col,0));
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<m;j++)
    	table[i][j]=a[i][j];
    	if(add_col)
    	{
    		for(int j=m;j<m+add_col;j++)
    		{
    			if(sign[i]==1)
    			table[i][j]=1;
    		}
    	}
    	table[i][m+add_col]=b[i];
    }
    for(int i=0;i<m;i++)
    table[n][i]=-z[i];
    
    //simplex table:"<<endl;
    for(int i=0;i<n+1;i++)
    {
    	for(int j=0;j<m+add_col+1;j++)
    	cout<<"Initialout<<table[i][j]<<' ';
    	cout<<endl;
    }
    cout<<endl;
    
    int r=n+1,col=m+1+add_col;
    int itr=0;
    vi ord(r+col-2);
    for(int i=0;i<r+col-2;i++)
    {
    	ord[i]=i+1;
    }
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
    			}
    		}
    	}
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
    	swap(ord[pivc],ord[col-2+pivr+1]);
    	cout<<"minimum ratio:"<<minr<<endl;
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
    	cout<<"Simplex tableau after iteration "<<itr<<endl;
    	for(int i=0;i<n+1;i++)
	    {
	    	for(int j=0;j<m+add_col+1;j++)
	    	cout<<table[i][j]<<' ';
	    	cout<<endl;
	    }
	    cout<<endl;
    }
    cout<<"Basic solutions are:";
    for(int i=0;i<col-1;i++)
    cout<<0<<' ';
    cout<<endl;
    cout<<"Non-basic solutions are:";
    for(int i=0;i<r-1;i++)
    cout<<table[i][col-1]<<' ';
    cout<<endl;
    cout<<"Optimized output is:"<<table[r-1][col-1];
}