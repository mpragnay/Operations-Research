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
	//	cout<<"YES"<<endl;
    int n=b.size();
    vector<ld> x(n,0);
    //cout<<n<<' ';
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
	int n;
    cout<<"Enter number of equations"<<endl;
    cin>>n;
    vector<vector<ld>> a(n);
    vector<ld> b(n);
    for(int i=0;i<n;i++)
    {
        cout<<"Equation "<<i+1<<":"<<endl;
        vector<ld> c(n);
		for(int j=0;j<n;j++)
        {
            cout<<"Enter coefficient of x"<<j+1<<endl;
            cin>>c[j];
        }
		cout<<"Enter constant term:"<<endl;
		cin>>b[i];
		a[i]=c;
    }
    vector<ld> x=gs_algo(a,b);
    cout<<"Answer is: ";
    for(int i=0;i<3;i++)
	cout<<x[i]<<' ';
    
}