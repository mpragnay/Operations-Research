#include <bits/stdc++.h>
using namespace std;

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

vector<vector<double>> bigM(vector<vector<double>> arr, vector<string> var, vector<string> bv, 
            vector<double> cn, vector<double> cb, int type, double constant)
{
    // cout << "Im here 0.01" << endl;
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
            return infes;
        }
        // cout << "Im here 0.3" << endl;
        double pivot = arr[pivIdx[0]][pivIdx[1]] ;
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

    vector<vector<double>> ans; 
    ans.push_back({type * arr[arr.size()-1][arr[0].size()-1]  + constant});

    vector<double> val;
    for(int i=0 ; i<var.size()-1; i++)
        val.push_back(0);

    for(int i=0 ; i<bv.size() ; i++)
    {
        if(bv[i].substr(0,1) == "x" && arr[i][arr[0].size()-1] >= 0)
        {
            // cout << bv[i] << " = " << arr[i][arr[0].size()-1] << " ";
            char c = bv[i][1];
            val[int(c) - 49] = arr[i][arr[0].size()-1]; 
        }
        else if(bv[i].substr(0,1) == "x" && arr[i][arr[0].size()-1] < 0)
        {
            // cout << bv[i] << " = " << 0 << " "; 
            char c = bv[i][1];
            val[int(c) - 49] = 0; 
        }
    }
    for(int i=0 ; i<var.size() ; i++)
    {
        if(var[i].substr(0,1) == "x")
        {
            // cout << var[i] << " = " << 0 << " "; 
            char c = var[i][1];
            val[int(c) - 49] = 0; 
        }
    }  

    ans.push_back(val);
    return ans;

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

void branchAndBound(vector<vector<double>> arr, vector<string> var, vector<string> bv, vector<double> cn, 
                    vector<double> cb, int type, double constant, vector<vector<double>> ans)
{
    vector<double> frac;
    for(int i=0 ; i<ans[1].size(); i++)
        frac.push_back(ans[1][i] - floor(ans[1][i]));

    int max = 0;
    for(int i=1 ; i<frac.size() ; i++)
    {
        if(frac[i] > frac[max])
            max = i;
    }

    vector<double> solution ;
    vector<vector<double>> value;
    vector<vector<double>> ans1,ans2,ans3,ans4,ans5,ans6;

//////////////////////////////////////////////////////////////////////////////////////////////
    vector<vector<double>> arr1;
    for(int i=0 ; i<arr.size()-1 ; i++)
        arr1.push_back(arr[i]);
    vector<double> temp1;
    for(int i=0 ; i<arr[0].size()-1 ; i++)
    { 
        if(i == max)
            temp1.push_back(1);
        else
            temp1.push_back(0);
    }
    temp1.push_back(floor(ans[1][max]));
    arr1.push_back(temp1);
    arr1.push_back(arr[arr.size()-1]);

    vector<string> bv1;
    for(int i=0 ; i<bv.size()-1 ; i++)
        bv1.push_back(bv[i]);
    bv1.push_back("z" + to_string(bv.size()));
    bv1.push_back(bv[bv.size()-1]);

    vector<double> cb1 ;
    for(int i=0 ; i<cb.size() ; i++)
        cb1.push_back(cb[i]);
    cb1.push_back(0);

    printMat(arr1);

    ans1 = bigM(arr1,var,bv1,cn,cb1,type,constant);
    if(ans1[0][0] == -1)
    {

    }
    else
    {
        solution.push_back(ans1[0][0]);
        value.push_back(ans1[1]);
    }

//////////////////////////////////////////////////////////////////////////////////////////////
    vector<vector<double>> arr2;
    for(int i=0 ; i<arr.size()-1 ; i++)
    {
        vector<double> temp ;
        for(int j=0 ; j<arr[0].size() ; j++)
        {
            if(j == arr[0].size()-1)
                temp.push_back(0);
            temp.push_back(arr[i][j]);
        }
        arr2.push_back(temp);
    }

    vector<double> temp2;    
    for(int i=0 ; i<arr[0].size()-1; i++)
    { 
        if(i == max)
            temp2.push_back(1);
        else
            temp2.push_back(0);
    }
    temp2.push_back(-1);  
    temp2.push_back(ceil(ans[1][max]));  
    arr2.push_back(temp2);
    vector<double> temp3;
    for(int i=0 ; i<arr[0].size()+1 ; i++)
        temp3.push_back(0);
    arr2.push_back(temp3);

    vector<double> cb2 ;
    for(int i=0 ; i<cb.size() ; i++)
        cb2.push_back(cb[i]);
    cb2.push_back(-100000);

    vector<string> var2;
    for(int i=0 ; i<var.size()-1 ; i++)
        var2.push_back(var[i]);
    var2.push_back("x" + to_string(var.size()));
    var2.push_back(var[var.size()-1]);

    printMat(arr2);

    ans2 = bigM(arr2,var2,bv1,cn,cb2,type,constant);
    if(ans2[0][0] == -1)
    {

    }
    else
    {
        solution.push_back(ans2[0][0]);
        ans2[1].pop_back();
        value.push_back(ans2[1]);
    }

//////////////////////////////////////////////////////////////////////////////////////////////
    int k = 0;
    while(true)
    {

    	vector<double> frac1;
        if(solution[k] != -1)
        {
            
            for(int i=0 ; i<value[k].size(); i++)
                frac1.push_back(value[k][i] - floor(value[k][i]));
        }

        max = 0;
        for(int i=1 ; i<frac1.size() ; i++)
        {
            if(frac1[i] > frac1[max])
                max = i;
        }  

        if(frac1[max] != 0)
        {
            for(int i=0 ; i<arr1[0].size() ; i++)
                arr1[arr1.size()-2][i] = 0;
            arr1[arr1.size()-2][max] = 1;
            // cout << endl << "floor(value[k][max]) = " << floor(value[k][max]) << endl;
            arr1[arr1.size()-2][arr1[0].size()-1] = floor(value[k][max]);

            // printMat(arr1);
            vector<vector<double>> ansn = bigM(arr1,var,bv1,cn,cb1,type,constant);
            if(ansn[0][0] == -1)
            {

            }
            else
            {
                solution.push_back(ansn[0][0]);
                value.push_back(ansn[1]);
            }
           for(int i=0 ; i<arr2[0].size()-2 ; i++)
                arr2[arr2.size()-2][i] = 0;
            arr2[arr1.size()-2][max] = 1;
            // cout << endl << "ceil(value[k][max]) = " << ceil(value[k][max]) << endl;
            arr2[arr2.size()-2][arr2[0].size()-1] = ceil(value[k][max]);
            // printMat(arr2);
            vector<vector<double>> ansnp1 = bigM(arr2,var2,bv1,cn,cb2,type,constant);
            if(ansnp1[0][0] == -1)
            {

            }
            else
            {
                solution.push_back(ansnp1[0][0]);
                ansnp1[1].pop_back();
                value.push_back(ansnp1[1]);
            }
        }
        k++;  
        if(k > 10)
            break;
    }

    vector<double> intSol;
    vector<vector<double>> intVal;
    int flag = 1;
    for(int i=0 ; i<value.size() ; i++)
    {
        flag = 1;
        for(int j=0 ; j<value[i].size() ; j++)
        {
            if(value[i][j] - floor(value[i][j]) != 0)
            {
                flag = 0;
                break;
            }
        }
        if(flag == 1)
        {
            intSol.push_back(solution[i]);
            intVal.push_back(value[i]);
        }
    }

    for(int i=0 ; i<intSol.size(); i++)
    {
        cout << endl <<"Optimal solution = " << intSol[i] << endl ;
        for(int j=0 ; j<intVal[i].size() ; j++)
            cout << "x" << j+1 << " = " << intVal[i][j] << " ";
        cout << endl;
    }

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
    vector<vector<double>> ans = bigM(arr,var,bv,cn,cb,type,constant) ;
    branchAndBound(arr,var,bv,cn,cb,type,constant,ans);
    // cout << endl << "Ans = " << endl;
    // for(int i=0 ; i<ans.size() ; i++)
    // {
    //     for(int j=0 ; j<ans[i].size() ; j++)
    //         cout << ans[i][j]  << " " ;
    //     cout << endl;
    // }

    // for(int i=0 ; i<var.size(); i++)
    //     cout << var[i] << " ";
    // return 0;
}
