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
    ld a[n][m];
    vector<ld> b(n);

    cout<<"Enter coefficients of variables and constants"<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<"Equation "<<i+1<<":"<<endl;
        for(int j=0;j<m;j++)
        {
            cout<<"Enter coefficient of x"<<j+1<<endl;
            cin>>a[i][j];
        }
        cout<<"Enter constant term:"<<endl;
        cin>>b[i];
    }
    int k=m-n;
    vector<vector<ld>> res;
    string bitmask(k, 1); 
    bitmask.resize(m, 0); 
    do 
    {
        vector<vector<ld>> inp(n,vector<ld> (n,0));
        int l=0;
        for (int i = 0; i < m; ++i) 
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
        vector<ld> y(m,0);
        int j=0;
        for(int i=0;i<m;i++)
        {
            if(bitmask[i]==0)
            {
                y[i]=x[j];
                j++;
            }
        }   
        res.pb(y);   
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

    cout<<"Basic solutions are:"<<endl;
    for(int i=0;i<res.size();i++)
    {
        for(int j=0;j<m;j++)
        cout<<res[i][j]<<' ';
        cout<<endl;
    }
}