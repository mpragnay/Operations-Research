import numpy as np
import pandas as pd
import math

INFINITY = 10e9


def take_input():
    optimization = str(input())
    # print("Enter the number of variables")
    n = int(input())
    # print("Enter the number of equations")
    m = int(input())
    # print("Enter the coefficients of the objective Function")
    c = [float(i) for i in input().split(" ")]
    if optimization == 'min':
        c = [-z for z in c]
    # print("Enter the value of the constant in the objective function")
    d = float(input())
    # print("Enter the coefficient of the constraints row by row")
    a = []
    for i in range(m):
        a.append([float(j) for j in input().split(" ")])
    # print("Enter the constants for the equations")
    b = [float(i) for i in input().split(" ")]
    # print("Enter the equation or in-equation type for the variable")
    s = input().split(" ")
    for i in range(len(b)):
        if b[i] >= 0:
            continue
        b[i] *= -1
        if s[i] == '>=':
            s[i] = '<='
        elif s[i] == '<=':
            s[i] = '>='
        for j in range(n):
            a[i][j] *= -1
    return n, m, a, np.array(c), d, b, s, optimization


def print_standard_form(n, m, a, c, d, b, s, M, optimization):
    num_artificial_variables = 0
    num_surplus_variables = 0
    num_slack_variables = 0
    k = n + 1
    x = []
    add_col = []
    all_variables = []
    list_of_nonbasic_variables = []
    list_of_basic_variables = []
    list_of_slack_variables = []
    list_of_artificial_variables = []
    list_of_surplus_variables = []
    a = np.array(a)
    b = np.array(b)
    obj_func = ""
    for i, j in enumerate(c):
        obj_func += str(j) + "x" + str(i + 1)
        obj_func += " + "
    size = len(c)
    obj_func += str(d)
    """print("The objective function to maximize is: \n" + obj_func)"""
    print("\nThe standard form of the constraints using slack/surplus variables is:")
    k = n + 1
    x = []
    add_col = []
    for i in range(n):
        list_of_nonbasic_variables.append("x" + str(i + 1))
        x.append(0)
    
    for j, row in enumerate(a):
        constraint = ""
        for i, q in enumerate(row):
            constraint += str(q) + "x" + str(i + 1) + " + " * (i != len(row) - 1)
        if s[j] == "=":
            constraint += " + x" + str(k)
            list_of_basic_variables.append("x" + str(k))
            x.append(b[j])
            k += 1
            num_artificial_variables += 1
        elif s[j] == "<=":
            constraint += " + x" + str(k)
            list_of_basic_variables.append("x" + str(k))
            x.append(b[j])
            k += 1
            num_slack_variables += 1
        else:
            constraint += " - x" + str(k) + " + x" + str(k + 1)
            list_of_basic_variables.append("x" + str(k + 1))
            x.append(0)
            list_of_nonbasic_variables.append("x" + str(k))
            x.append(b[j])
            num_surplus_variables += 1
            temp_col = np.zeros((m,))
            temp_col[j] = -1
            add_col.append(temp_col)
            num_artificial_variables += 1
            k += 2
        constraint += " = " + str(b[j])
        print(constraint)

    if len(list_of_nonbasic_variables) - size > 0:
        additional = np.array(add_col).transpose()
        a = np.concatenate([a, additional], axis=1)

    all_variables = list_of_nonbasic_variables + list_of_basic_variables
    cn = np.zeros((len(list_of_nonbasic_variables)))
    cb = np.zeros((len(list_of_basic_variables)))
    for i in range(len(c)):
        cn[i] = c[i]
    for i, var in enumerate(list_of_basic_variables):
        if s[i] == '<=':
            cb[i] = 0
        else:
            cb[i] = -M
    # c = np.empty((len(all_variables)))
    zj_cj = np.empty((len(list_of_nonbasic_variables)))
    # a = np.concatenate([a, np.array(add_col)], axis=1)
    print(a)
    print(cb)
    print(cn)
    print(list_of_nonbasic_variables)
    print(list_of_basic_variables)
    for i in range(len(list_of_nonbasic_variables)):
        zj_cj[i] = 0
        zj_cj[i] += (np.dot(np.transpose(cb), a[:, i]) - cn[i])
    """print("The objective function to maximize is")
    obj_func = ""
    """
    for i, var in enumerate(list_of_nonbasic_variables):
        obj_func += "-" + str(-cn[i]) + str(var) + " "
    print(obj_func + " + ", d)
    return a, b, cb, cn, zj_cj, np.array(
        x), list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_artificial_variables, list_of_slack_variables, list_of_surplus_variables

def val_objective_function(cb, b, d, optimization):
    if optimization == 'max':
        return np.dot(np.transpose(cb), b) + d
    else:
        return -np.dot(np.transpose(cb), b) + d

def simplex(a, c, cn, cb, d, b, list_of_nonbasic_variables, list_of_basic_variables, all_variables,
            x, optimization, num_surplus_variables, num_slack_variables, num_artificial_variables):
    ite = 0
    n_prime = a.shape[1]
    m_prime = a.shape[0]
    while True:
        ite += 1
        print("-" * 60)
        print("Iteration ", ite)
        print("The value of Objective function in this iteration is ",
              round(val_objective_function(cb, b, d, optimization), 5))
        print("The values for x are:")
        l_ = []
        for i in range(len(x)):
            l_.append("x" + str(i + 1) + '=' + str(x[i]))
        print(l_)
        print("This is the list of all the basic variables are ", list_of_basic_variables)
        print("This is the list of all non-basic variables are ", list_of_nonbasic_variables)
        print("The matrix form of A is")
        print(a)
        print("The value of Zj-Cj are")
        print(c)
        print("The values of basic solutions X_b are")
        print(b)
        v = np.argmin(c)
        cv = c[v]
        if cv >= 0:
            if ite > num_artificial_variables:
                print("-" * 60)
                print("The iterations have ended")
                print("This is the list of all the basic variables are ", list_of_nonbasic_variables)
                print("This is the list of all non-basic variables are ", list_of_basic_variables)
                print("The values for x are:")
                li = []
                for i in range(len(x)):
                    li.append("x" + str(i + 1) + '=' + str(round(x[i], 5)))
                print(li)
                print("So the Final value of objective function is:",
                      round(val_objective_function(cb, b, d, optimization), 5))
                return a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables
            else:
                print("-" * 60)
                print("The Solution is infeasible because all artificial variables are not zero")
                print("This is the list of all the basic variables are ", list_of_nonbasic_variables)
                print("This is the list of all non-basic variables are ", list_of_basic_variables)
                print("The values for x are:")
                li = []
                for i in range(len(x)):
                    li.append("x" + str(i + 1) + '=' + str(round(x[i], 5)))
                print(li)
                print("As you can see the the value of objective function is :",
                      round(val_objective_function(cb, b, d, optimization), 5), "which is huge because all "
                                                                                "artificial variables are not "
                                                                                "zero")
                return a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables
        else:
            print("The value of most negative c is", cv, " Corresponding to column", v + 1)
            ratios = np.zeros((m_prime,))
            u = 0
            pivot = -1
            min_ratio = INFINITY
            for i in range(m_prime):
                if a[i][v] == 0:
                    continue
                ratios[i] = b[i] / a[i][v]
                if min_ratio > ratios[i] >= 0:
                    u = i
                    min_ratio = ratios[i]
                    pivot = a[u][v]
            print("The ratios are for corresponding column", ratios)
            print("The minimum ratio is:", min_ratio)
            print("The pivot element is ", pivot, " and corresponding coordinates(1 based indexing) is", u + 1, " ",
                  v + 1)
            if min_ratio == INFINITY:
                print("-" * 60)
                print("The problem is unbounded")
                print("The value of objective function is:", round(val_objective_function(cb, b, d, optimization), 5))
                print("The values for x are:", x)
                return a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables
            a_new = np.empty((m_prime, n_prime))
            for i in range(m_prime):
                for j in range(n_prime):
                    if i == u and j == v:
                        a_new[i][j] = 1 / a[i][j]
                    elif i == u:
                        a_new[i][j] = a[i][j] / pivot
                    elif j == v:
                        a_new[i][j] = -a[i][j] / pivot
                    else:
                        a_new[i][j] = (pivot * a[i][j] - a[i][v] * a[u][j]) / pivot
            c_new = np.copy(c)
            for j in range(n_prime):
                if j == v:
                    c_new[j] = round(-c[j] / pivot, 6)
                else:
                    c_new[j] = round((pivot * c[j] - c[v] * a[u][j]) / pivot, 6)
            b_new = np.copy(b)
            for j in range(m_prime):
                if j == u:
                    b_new[j] = b[j] / pivot
                else:
                    b_new[j] = (pivot * b[j] - a[j][v] * b[u]) / pivot
            a = np.copy(a_new)
            c = np.copy(c_new)
            b = np.copy(b_new)
            temp2 = cb[u]
            cb[u] = cn[v]
            cn[v] = temp2
            temp1 = list_of_nonbasic_variables[v]
            list_of_nonbasic_variables[v] = list_of_basic_variables[u]
            list_of_basic_variables[u] = temp1
            for i in range(len(list_of_basic_variables)):
                s = list_of_basic_variables[i]
                x[int(s[1:]) - 1] = b[i]
            for i in range(len(list_of_nonbasic_variables)):
                s = list_of_nonbasic_variables[i]
                x[int(s[1:]) - 1] = 0


def get_frac_part(x):
    if x == 0:
        return 0
    elif x > 0:
        return x - int(x)
    else:
        return 1 + x - int(x)


def cutting_plane(a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables,
                  list_of_slack_variables):
    n = len(list_of_nonbasic_variables)
    m = len(list_of_basic_variables)
    list_of_slack_variables.append('x'+str(n+m+1))
    all_variables.append('x'+str(n+m+1))
    list_of_basic_variables.append('x'+str(n+m+1))
    i = -1
    beta_i = -INFINITY
    for j in range(len(b)):
        f = b[j] - int(b[j])
        if beta_i < f:
            beta_i = f
            i = j
    add_row = np.empty((1, n))
    for item in range(n):
        f_i = get_frac_part(a[i][item])
        add_row[0][item] = -f_i
    b_new = np.empty((len(list_of_basic_variables)))
    for item in range(len(b)):
        b_new[item] = b[item]
    b_new[len(b)] = -get_frac_part(b[i])
    x_new = list(x)
    x_new.append(-get_frac_part(b[i]))
    x = np.array(x_new)
    b = np.copy(b_new)
    cb_list = cb.tolist()
    cb_list.append(0)
    cb = np.array(cb_list)
    a = np.concatenate([a, add_row], axis=0)
    return a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables


def print_table(a, c, d, b, cb, x, list_of_nonbasic_variables, list_of_basic_variables, optimization):
    l_ = []
    for basic in list_of_nonbasic_variables:
        l_.append(basic)
    print("Non Basic Variables = ", l_)
    l_ = []
    for non_basic in list_of_basic_variables:
        l_.append(non_basic)
    print("Basic Variables = ", l_)
    print("This is A martix")
    print(a)
    print("This is Xb")
    print(b)
    print("This is zj-cj bottom row of table")
    print(c)
    print("This is Cb (coefficients of basic variable")
    print(cb)
    print("This is Cn (coefficients of non-basic variable")
    print(cn)
    print("So the optimal value of objective function is:", round(val_objective_function(cb, b, d, optimization), 5))


def print_pivot(cv, v, u, ratios, min_ratio, pivot):
    print("The value of c is", -cv, " corresponding to column", v + 1)
    print("The ratios are for corresponding column", ratios)
    print("The minimum ratio is:", min_ratio)
    print("The pivot element is ", pivot, " and corresponding coordinates(1 based indexing) is", u + 1, " ", v + 1)


def dual_simplex(a, c, cn, cb, d, b, list_of_nonbasic_variables, list_of_basic_variables, all_variables,
                 x, optimization, num_surplus_variables, num_slack_variables, num_artificial_variables):
    n_prime = a.shape[1]
    m_prime = a.shape[0]
    ite = 0
    while True:
        ite += 1
        if ite > 10:
            print(
                "The table has repeated. Due to this there is infinite iterations. Hence cannot solve by dual simplex.")
            exit(0)
        print("-" * 60)
        print("Iteration ", ite)
        print_table(a, c, d, b, cb, x, list_of_nonbasic_variables, list_of_basic_variables,
                    optimization)
        print("The values for x are:")
        l_ = []
        for i in range(len(x)):
            l_.append("x" + str(i + 1) + '=' + str(x[i]))
        print(l_)
        u = np.argmin(b)
        cv = b[u]
        if cv >= 0:
            if ite > num_artificial_variables:
                print("-" * 60)
                print("The iterations have ended because no negative values of Xb are present")
                print("This is the list of all the non-basic variables are ", list_of_nonbasic_variables)
                print("This is the list of all basic variables are ", list_of_basic_variables)
                print("The values for x are:")
                l_ = []
                for i in range(len(x)):
                    l_.append("x" + str(i + 1) + '=' + str(x[i]))
                print(l_)
                print("So the Final value of objective function is:",
                      round(val_objective_function(cb, b, d, optimization), 5))
                return a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables
            else:
                print("-" * 60)
                print("The Solution is infeasible because all artificial variables are not zero")
                print("This is the list of all the non-basic variables are ", list_of_nonbasic_variables)
                print("This is the list of all basic variables are ", list_of_basic_variables)
                print("The values for x are:")
                li = []
                for i in range(len(x)):
                    li.append("x" + str(i + 1))
                print(li)
                print("As you can see the the value of objective function is :",
                      round(val_objective_function(cb, b, d, optimization), 5))
                return a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables
        else:
            print("The value of most negative Xb is", cv, " Corresponding to row", u + 1)
            ratios = np.empty((n_prime,))
            v = 0
            pivot = -1
            min_ratio = INFINITY
            for i in range(n_prime):
                if a[u][i] == 0:
                    continue
                ratios[i] = abs(c[i] / a[u][i])
                if min_ratio > ratios[i] >= 0:
                    v = i
                    min_ratio = ratios[i]
                    pivot = a[u][v]
            print("The ratios are for corresponding row", ratios)
            print("The minimum ratio is:", min_ratio)
            print("The pivot element is ", pivot, " and corresponding coordinates(1 based indexing) is", u + 1, " ",
                  v + 1)
            if min_ratio == INFINITY:
                print("-" * 60)
                print("The problem is unbounded")
                print("The value of objective function is:", val_objective_function(cb, b, d, optimization))
                print("The values for x are:", x)
                return a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables
            a_new = np.empty((m_prime, n_prime))
            for i in range(m_prime):
                for j in range(n_prime):
                    if i == u and j == v:
                        a_new[i][j] = 1
                    elif i == u:
                        a_new[i][j] = round(a[i][j] / pivot, 6)
                    elif j == v:
                        a_new[i][j] = 0
                    else:
                        a_new[i][j] = round((pivot * a[i][j] - a[i][v] * a[u][j]) / pivot, 6)
            c_new = np.copy(c)
            for j in range(n_prime):
                if j == v:
                    c_new[j] = 0
                else:
                    c_new[j] = round((pivot * c[j] - c[v] * a[u][j]) / pivot, 6)
            b_new = np.copy(b)
            for j in range(m_prime):
                if j == u:
                    b_new[j] = round(b[j] / pivot)
                else:
                    b_new[j] = round((pivot * b[j] - a[j][v] * b[u]) / pivot, 6)
            a = np.copy(a_new)
            c = np.copy(c_new)
            b = np.copy(b_new)
            temp2 = cb[u]
            cb[u] = cn[v]
            cn[v] = temp2
            temp1 = list_of_nonbasic_variables[v]
            list_of_nonbasic_variables[v] = list_of_basic_variables[u]
            list_of_basic_variables[u] = temp1
            for i in range(len(list_of_basic_variables)):
                s = list_of_basic_variables[i]
                x[int(s[1:]) - 1] = b[i]
            for i in range(len(list_of_nonbasic_variables)):
                s = list_of_nonbasic_variables[i]
                x[int(s[1:]) - 1] = 0


def frac(x, n):
    for i in range(n):
        if abs(math.ceil(x[i]) - x[i]) > 10e-4 and abs(math.floor(x[i]) - x[i]) > 10e-4:
            return False
    return True


if __name__ == '__main__':
    # t = int(input("Enter the number of test cases:"))
    M = 10000
    print("*" * 60)

    n, m, a, c, d, b, s, optimization = take_input()
    # print standard form
    a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_artificial_variables, list_of_slack_variables, list_of_surplus_variables = print_standard_form(
        n, m, a, c, d, b, s, M, optimization)
    # simplex method

    a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables = simplex(
        a, c, cn, cb, d, b, list_of_nonbasic_variables, list_of_basic_variables, all_variables, x, optimization,
        len(list_of_surplus_variables), len(list_of_slack_variables), len(list_of_artificial_variables))
    print_table(a, c, d, b, cb, x, list_of_nonbasic_variables, list_of_basic_variables,
                optimization)
    print("The values for x are:")
    l_ = []
    for i in range(len(x)):
        l_.append("x" + str(i + 1) + '=' + str(x[i]))
    print(l_)
    ite = 0
    while not frac(x,n) and ite < 4:
        ite+=1
        print("^*"*45)
        print("Dual Simplex Iteration: ", ite)
        a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables = cutting_plane(a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables)
        a, b, cb, cn, c, x, list_of_nonbasic_variables, list_of_basic_variables, all_variables, list_of_slack_variables = dual_simplex(a, c, cn, cb, d, b, list_of_nonbasic_variables, list_of_basic_variables, all_variables, x, optimization, len(list_of_surplus_variables), len(list_of_slack_variables), len(list_of_artificial_variables))