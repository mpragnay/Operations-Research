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
    int k=0;
    cout<<"Enter coefficients of variables and constants"<<endl;
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
        cout<<"Enter constant term:"<<endl;
        cin>>b[i];
    }
    cout<<"Enter coefficients of equation to maximize:"<<endl;
    vi z(m);
    for(int i=0;i<m;i++)
    cin>>z[i];
    

    vector<vector<ld>> res;
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

    
    for(int i=0;i<m;i++)
    {
    	for(int j=0;j<n+m;j++)
    	cout<<res[i][j]<<' ';
    	cout<<endl;
    }
}