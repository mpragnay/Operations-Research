#include <bits/stdc++.h>
using namespace std;

//Here in checkInt function instead of checking all x (whether they are int or not),  we only check x2

int findMin(vector<double> arr)
{
    double min = 0 ;
    for(int i=1 ; i<arr.size()-1 ; i++) //We skip the last element
    {
        if(arr[i] < arr[min])
            min = i;
    }
    if(arr[min] < 0)
        return min; //Return the most negative element
    return -1; //Return if the row is completely positive
}

vector<int> findPivot(vector<vector<double>> arr, int min) //return a tuple of indices of pivotal element
{
    vector<double> ratios;
    for(int i=0 ; i<arr.size()-1 ; i++)
    {
        if(arr[i][min] > 0)
            ratios.push_back(arr[i][arr[0].size()-1]/arr[i][min]); //Stores the ratio
        else
            ratios.push_back(999999);
    }
    int minInd = 0;
    for(int i=1 ; i<ratios.size() ; i++) //Finding minimum ratio
    {
        if(ratios[minInd] > ratios[i])
            minInd = i;
    }
    if(ratios[minInd] != 999999) //If ratio is possible
        return {minInd,min};
    return {-1,-1}; //Problem is unbound
}

double pqrs(double p, double q, double r, double s)
{
    return (p*s - q*r)/p ;
}


double dotProduct(vector<double> p , vector<double> q)
{
    if(p.size() != q.size())
    {
        cout << p.size() << "  " << q.size() << endl ;
        cout << "Length mismatch " << endl;
        return 0;
    }
    double val = 0;

    for(int i=0 ; i<p.size() ; i++)
        val += (p[i]*q[i]) ;
    return val;
}

vector<double> removeLastRow(vector<vector<double>> arr, int k)
{
    vector<double> out; 
    for(int i=0 ; i<arr.size()-1 ; i++)
        out.push_back(arr[i][k]);
    return out;
}

void printTableExperiment(vector<vector<double>> arr, vector<string> var, vector<string> bv)
{
    cout << endl;
    for(int i=0 ; i<var.size() ; i++)
        cout << var[i] << '\t' ;
    cout << endl;
    cout << endl;
    for(int i=0 ; i<arr.size() ; i++)
    {
        
        for(int j=0 ; j<arr[i].size() ; j++)
        {
            cout << arr[i][j] << '\t';
        }
        // cout << '\t' ;
        cout  << bv[i] << endl;
    }
}


void printMat(vector<vector<double>> arr)
{
    for(int i=0 ; i<arr.size(); i++)
    {
        for(int j=0 ; j<arr[i].size(); j++)
            cout << arr[i][j] << " " ;
        cout << endl;
    }
}

double fraction(double a)
{
    double f;
    if(a > 0)
        f =  a - floor(a) ;
    else
        f = -1* (abs(a) - floor(abs(a))); 
    if(abs(f) < 0.00001 || abs(f) > 0.999)
        f = 0;
    return f;
}

int findMaxFrac(vector<vector<double>> arr)
{
    int max = 0;
    for(int i=1 ; i<arr.size() ; i++)
    {
        if(fraction(arr[i][arr[i].size()-1]) > fraction(arr[max][arr[max].size()-1]))
            max = i;
    }
    return max ;
}

int findMinDual(vector<double> arr)
{
    double min = 0 ;

    for(int i=1 ; i<arr.size()-1 ; i++) //We skip the last element
    {
        if(arr[i] < arr[min])
            min = i;
    }
    if(arr[min] < 0)
        return min; //Return the most negative element

    return -1; //Return if the row is completely positive
}

vector<int> findPivotDual(vector<vector<double>> arr, int min) //return a tuple of indices of pivotal element
{
    vector<double> ratios;

    for(int i=0 ; i<arr[0].size()-1 ; i++)
    {
        if(arr[min][i] < 0)
            ratios.push_back(abs(arr[arr.size()-1][i]/arr[min][i])); //Stores the ratio
        else
            ratios.push_back(999999);
    }

    int minInd = 0;
    for(int i=1 ; i<ratios.size() ; i++) //Finding minimum ratio
    {
        if(ratios[minInd] >= ratios[i])
            minInd = i;
    }


    if(ratios[minInd] != 999999) //If ratio is possible
        return {min,minInd};

    return {-1,-1}; //Problem is unbound
}

vector<double> getLastColumn(vector<vector<double>> arr)
{
    vector<double> out ;

    for(int i=0 ; i<arr.size() ; i++)
        out.push_back(arr[i][arr[0].size()-1]);
    return out;
}

int checkInt(vector<vector<double>> arr,vector<string> bv)
{
    for(int i=0 ; i<bv.size() ; i++)
    {
        if(bv[i] == "x2" )
        {
            if(fraction(arr[i][arr[0].size()-1]) != 0)
            {
                // cout << "No " << fraction(arr[i][arr[0].size()-1]) << endl;
                return 0;
            } 
        }
    }
    return 1;
}

void cuttingPlane(vector<vector<double>> arr, vector<string> var, vector<string> bv, 
            vector<double> cn, vector<double> cb, int type, double constant)
{
    for(int i=0 ; i<arr[0].size()-1 ; i++)
    {
        arr[arr.size()-1][i] = dotProduct(cb,removeLastRow(arr,i)) - cn[i];
        // cout << "Im here 0.01" << endl;
    }
    arr[arr.size()-1][arr[0].size()-1] = dotProduct(cb,removeLastRow(arr,arr[0].size()-1));
    // cout << "Im here 0.1" << endl;

    cout << endl << "Initial Table : " << endl;
    printTableExperiment(arr,var,bv);
    cout << endl;

    while(findMin(arr[arr.size()-1]) != -1) //We will go in a loop till all elements in last row is positive and give optimal solution or the problem is unbound
    {
        int min = findMin(arr[arr.size()-1]) ;
        vector<int> pivIdx = findPivot(arr,min);
        // cout << "Im here 0.2" << endl;
        if(pivIdx[0] == -1)
        {
            cout << "Problem is unbound and has no solution " << endl;
            vector<vector<double>> infes;
            infes.push_back({-1});
            // return infes;
        }
        // cout << "Im here 0.3" << endl;
        double pivot = arr[pivIdx[0]][pivIdx[1]] ;
        cout << "Pivot = " << pivot << endl;

        //Swapping bv and var 
        cout << "Out Going variable : " << bv[pivIdx[0]] << endl;
        cout << "Incoming variable  : " << var[pivIdx[1]] << endl; 
        swap(bv[pivIdx[0]],var[pivIdx[1]]);

        for(int i=0 ; i<arr.size(); i++)
        {
            for(int j=0 ; j<arr[i].size() ; j++)
            {
                if(i != pivIdx[0] && j != pivIdx[1])
                    arr[i][j] = pqrs(pivot,arr[pivIdx[0]][j],arr[i][pivIdx[1]],arr[i][j]) ;
            }
        }
        for(int i=0 ; i<arr.size(); i++)  //Dividing the column by -pivot
            arr[i][min]/= (-1*pivot) ;

        for(int i=0 ; i<arr[0].size(); i++) //Dividing the pivot row by pivot
            arr[pivIdx[0]][i]/= pivot ; 

        arr[pivIdx[0]][pivIdx[1]] = 1/pivot ;

        cout << endl << "Table : " << endl;
        printTableExperiment(arr,var,bv);
        cout << endl;
    }
    cout << endl << "Table : " << endl;
    printTableExperiment(arr,var,bv);
    cout << endl;
    int l=0;

    while(true)
    {
        if(checkInt(arr,bv) == 1)
            break;
        int maxFrac = findMaxFrac(arr) ;

        if(arr[maxFrac][arr[0].size()-1] == 0)
            break;

        vector<double> temp ;
        for(int i=0 ; i<arr[0].size() ; i++)
        {
            temp.push_back(arr[arr.size()-1][i]);
            arr[arr.size()-1][i] = -1*fraction(arr[maxFrac][i]) ;
        }
        arr.push_back(temp) ;

        l++;
        cb.push_back(0);
        bv.insert(bv.begin() + bv.size()-1,"z" + to_string(arr.size()+l));
        var.insert(var.begin() + var.size()-1 ,"x" + to_string(arr[0].size()));


        int iter = 0;
        
        cout << endl << "Initial Table : " << endl;
        printTableExperiment(arr,var,bv);
        cout << endl;
        while(findMinDual(getLastColumn(arr)) != -1 ) //We will go in a loop till all elements in last row is positive and give optimal solution or the problem is unbound
        {
            int min = findMinDual(getLastColumn(arr)) ;
            vector<int> pivIdx = findPivotDual(arr,min);
            if(pivIdx[0] == -1)
            {
                exit(1);
                cout << "Problem is unbound and has no solution " << endl;
                break;
            }
            double pivot = arr[pivIdx[0]][pivIdx[1]] ;
            cout << "Pivot = " << pivot << endl;

            //Swapping bv and var 
            cout << "Out Going variable : " << bv[pivIdx[0]] << endl;
            cout << "Incoming variable  : " << var[pivIdx[1]] << endl; 
            swap(bv[pivIdx[0]],var[pivIdx[1]]);



            for(int i=0 ; i<arr.size(); i++)
            {
                for(int j=0 ; j<arr[i].size() ; j++)
                {
                    if(i != pivIdx[0] && j != pivIdx[1])
                    {
                        double vr = pqrs(pivot,arr[pivIdx[0]][j],arr[i][pivIdx[1]],arr[i][j]) ;
                        arr[i][j] = vr;
                    }
                }
            }
            for(int i=0 ; i<arr.size(); i++)  //Dividing the column by -pivot
                arr[i][pivIdx[1]]/= (-1*pivot) ;

            for(int i=0 ; i<arr[0].size(); i++) //Dividing the pivot row by pivot
                arr[pivIdx[0]][i]/= pivot ; 

            arr[pivIdx[0]][pivIdx[1]] = 1/pivot ;


            cout << endl << "Table : " << endl;
            printTableExperiment(arr,var,bv);
            cout << endl;


            // cout << "Objective value = " << type * arr[arr.size()-1][arr[0].size()-1] << endl; 
            iter++;
            cout << endl;
            if(iter > 10)
                break;
        }
    }
    cout << "Final Table : " << endl;
    printTableExperiment(arr,var,bv);
    cout << endl ;
    for(int i=0 ; i<bv.size() ; i++)
    {
        if(bv[i].substr(0,1) == "x" &&arr[i][arr[0].size()-1] >= 0)
            cout << bv[i] << " = " << arr[i][arr[0].size()-1] << " "; 
        else if(bv[i].substr(0,1) == "x" && arr[i][arr[0].size()-1] < 0)
            cout << bv[i] << " = " << 0 << " "; 
    }
    for(int i=0 ; i<var.size() ; i++)
    {
        if(var[i].substr(0,1) == "x")
            cout << var[i] << " = " << 0 << " "; 
    }    
    cout << "Optimal solution = " << type * arr[arr.size()-1][arr[0].size()-1] << endl;

}


int main()
{
    int m,n,type ;
    cout <<"Enter maximation type (1) or minimization type (-1) :" ;
    cin >> type;
    if(type != 1 && type != -1)
    {
        cout <<"Enter maximation type (1) or minimization type (-1) :" ;
        cin >> type;
    }
    cout << "Enter number of equations : " ;
    cin >> n ;
    cout << "Enter number of unknowns : " ;
    cin >> m ;

    int original_m = m;
    vector<vector<double>> a ;
    vector<double> b;
    vector<string> relation;

    for(int i=0 ; i<n ; i++)
    {
        vector<double> temp;
        cout << "Enter the coefficient of equation " << i+1 << " : " << endl;
        for(int j=0 ; j<m ; j++)
        {
            double t ;
            cin >> t ;
            temp.push_back(t) ;
        }
        a.push_back(temp);

        cout << "Enter the condition ('=' or '<' or '>') : " << endl;
        string w;
        cin >> w;
        relation.push_back(w);

        cout <<"Enter the constant term : " << endl;
        double u;
        cin >> u;
        b.push_back(u);
    }

    for(int i=0 ; i<relation.size() ; i++)
    {
        if(relation[i] == ">")
        {
            for(int j=0 ; j<a.size() ; j++)
            {
                if(j == i)
                    a[j].push_back(-1);
                else
                    a[j].push_back(0);
            }
            m++;
        }
    }

    for(int i=0 ; i<relation.size() ; i++)
    {
        for(int j=0 ; j<a.size() ; j++)
        {
            if(j == i)
                a[j].push_back(1);
            else
                a[j].push_back(0);
        }   
    }

    vector<vector<double>> arr ;

    for(int i=0 ; i<n ; i++)
    {
        vector<double> temp;
        for(int j=0 ; j<m ; j++)
            temp.push_back(a[i][j]);
        temp.push_back(b[i]);
        arr.push_back(temp);
    }

    vector<double> lastRow;
    for(int i=0 ; i<m+1 ; i++)
        lastRow.push_back(0);
    arr.push_back(lastRow) ;

    vector<string> var,bv ;

    for(int i=0 ; i<m ; i++)
    {
        string temp = "x" + to_string(i+1) ;
        var.push_back(temp);
    } 
    var.push_back("1") ;
    for(int i=0 ; i<n ; i++)
    {
        string temp = "z" + to_string(i+1) ;
        bv.push_back(temp);
    }
    if(type == 1) 
        bv.push_back("z") ;
    else 
        bv.push_back("-z") ;  
    vector<double> cn,cb ;

    for(int i=0 ; i<m ; i++)
        cn.push_back(0);
    for(int i=0 ; i<n ; i++)
        cb.push_back(0);

    cout << "Enter the coefficients of the Objective Function : " << endl;
    for(int i=0 ; i<original_m ; i++)
    {
        double q;
        cin >> q ;
        cn[i] = type * q;
    }
    cout << "Constant term in Objective (0 if no constant) :" <<endl;
    double constant ;
    cin >> constant ;
    for(int i=0 ; i<relation.size(); i++)
    {
        if(relation[i] == ">" || relation[i] == "=")
            cb[i] = -100000 ;
    }
    cuttingPlane(arr,var,bv,cn,cb,type,constant);

}
