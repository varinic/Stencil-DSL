import numpy as np
import matplotlib.pyplot as plt
import sqlite3
import json
from matplotlib.ticker import FormatStrFormatter
import random

def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

def isclose1(a, b, rel_tol=1e-01, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

def export2json(filename, sol_best):
    coord = []
    for line in open(filename,"r").readlines():
        x=line.strip("\r\n").split(",")
        coord.append({'lat':x[0],'lng':x[1]})

    export_data = []
    for i in range(len(sol_best)):
        export_data.append(coord[ int(sol_best[i]) ])

    file = open("path.json", 'w')
    file.write(json.dumps(export_data))
    file.close()

def list2str(list1):
    res=''
    for i in range(len(list1)):
        res=res+str(list1[i])
        if(i!=(len(list1)-1)):
            res=res+", "
    return res+'\n'

def append2file(filename, sol):
    file = open(filename, 'a')
    file.write(list2str(sol))
    file.close()

def change_sol4(grid, exp_num_mpi, sol_new):
    #for i in range(1,6,1):
    # 3,4,5
    for i in range(3,6,1):
        flag = random.randint(1,3)
        #ad = random.randint(1,101,2)
        ad = random.randrange(0, 101, 2)
        if(flag==1):
            if(sol_new[i]+ad<=grid[i-3]):
                sol_new[i]=sol_new[i]+ad
        elif(flag==2):
            if(sol_new[i]-ad>0):
                sol_new[i]=sol_new[i]-ad
    # 1,2
    for i in range(1,3,1):
        flag = random.randint(1,3)
        ad = random.randrange(0, 101, 2)
        if(flag==1):
            if(sol_new[i]+ad<=grid[i]/sol_new[i+3]):
                sol_new[i]=sol_new[i]+ad
        elif(flag==2):
            if(sol_new[i]-ad>0):
                sol_new[i]=sol_new[i]-ad
    return sol_new

def change_sol7(grid, exp_num_mpi, sol_new):
    index = random.randint(1,5)
    sol_new2=sol_new.copy()
    sol_new2[index]=(sol_new[index]+2)%(2**exp_num_mpi)+2
    return sol_new2

def change_sol(grid, exp_num_mpi, sol_new):
    index = random.randint(1,5)
    ad = random.randrange(2, 5, 2)
    ad = random.randrange(2, 9, 2)
    #ad =2
    sol_new2=sol_new.copy()
    sol_new2[index]=(sol_new[index]+ad)%(2**exp_num_mpi)+2
    #swap
    n1 = random.randint(1,5)
    n2 = random.randint(1,5)
    if n1 != n2:
        sol_new2[n1], sol_new2[n2] = sol_new2[n2], sol_new2[n1]
    ##swap
    #n1 = random.randint(1,5)
    #n2 = random.randint(1,5)
    #if n1 != n2:
    #    sol_new2[n1], sol_new2[n2] = sol_new2[n2], sol_new2[n1]
    ##swap
    #n1 = random.randint(1,5)
    #n2 = random.randint(1,5)
    #if n1 != n2:
    #    sol_new2[n1], sol_new2[n2] = sol_new2[n2], sol_new2[n1]
    #
    x1=grid[0]
    x2=x1//sol_new2[3]
    sol_new2[0]=x2
    return sol_new2


def change_sol5(grid, exp_num_mpi, sol_new):
    index = random.randint(1,5)
    ad =2
    flag = random.randint(1,3)
    if(flag==1):
        ad=-2
    sol_new[index]=(sol_new[index]+ad+32)%(2**exp_num_mpi)+2
    return sol_new

def change_sol3(grid, exp_num_mpi, sol_new):
    #ep6 = [i for i in range(1,7)]
    exp_num_mpi=13
    ep4 = random.randint(0,exp_num_mpi)
    ep1 = random.randint(0,ep4)
    ep5 = random.randint(0,exp_num_mpi)
    ep2 = random.randint(0,ep5)
    ep6 = random.randint(0,exp_num_mpi)
    ep3 = random.randint(0,ep6)
    sol_change = [sol_new[0],2**ep2,2**ep3,2**ep4,2**ep5,2**ep6]
    return sol_change


def change_sol1(grid, exp_num_mpi, sol_new):
    #ep6 = [i for i in range(1,7)]
    ep4 = random.randint(0,exp_num_mpi)
    ep1 = random.randint(0,ep4)
    ep5 = random.randint(0,exp_num_mpi-ep4)
    ep2 = random.randint(0,ep5)
    #ep6 = random.randint(0,exp_num_mpi-ep4-ep5)
    ep6 = exp_num_mpi-ep4-ep5
    ep3 = random.randint(0,ep6)
    sol_change = [sol_new[0],2**ep2,2**ep3,2**ep4,2**ep5,2**ep6]
    return sol_change

def check(grid, num_mpi, sol):
    x1=grid[0]
    y1=grid[1]
    z1=grid[2]
    if(x1%sol[3]!=0 or y1%sol[4]!=0 or z1%sol[5]!=0):
        #print('false 1')
        return False
    x2=x1/sol[3]
    y2=y1/sol[4]
    z2=z1/sol[5]
    assert(x2==sol[0])
    if(x2%sol[0]!=0 or y2%sol[1]!=0 or z2%sol[2]!=0):
        #print('false 2')
        return False
    sub_grid = sol[3]*sol[4]*sol[5]
    space = (2*1+2)*(y2+2)*(z2+2)
    #print(x1,y1,z1,x2,y2,z2,sub_grid, space)
    if(sub_grid==num_mpi and space<=6096):
        return True
    #print(x1,y1,z1,x2,y2,z2,sub_grid, space, 'false 3')
    return False


def performance_model(grid, num_mpi, sol):
    x1=grid[0]
    y1=grid[1]
    z1=grid[2]
    x2=x1/sol[3]
    y2=y1/sol[4]
    z2=z1/sol[5]
    #
    vBi1 =509044590.02
    vBi0 =305813733.40
    vBi2 =864294330.78
    vBo1 =538201936.22
    vBo0 =256921718.89
    vBo2 =854365679.86
    vBmpi=2243147165.10
    #
    space = (2*1+2)*(2*y2+2)*(2*z2+2)
    Tcomp = x2*y2*z2*1.77/1000/(1024*32*32)*(1+space/6096)
    #
    halo=1
    sizeof_double=8
    #left, front, end
    Tpack   = 2*halo*(x2*y2/vBi0+  x2*z2/vBi1+  y2*z2/vBi2)*sizeof_double
    Tunpack = 2*halo*(x2*y2/vBo0+  x2*z2/vBo1+  y2*z2/vBo2)*sizeof_double
    #
    Tmpi = 2*halo*( (x2*y2+ x2*z2+ y2*z2)/vBmpi)*sizeof_double
    #
    #print(Tcomp)
    #print(Tpack)
    #print(Tunpack)
    #print(Tmpi)
    #
    NT=100
    #
    Ttotal = NT*(Tmpi+Tpack+Tunpack+Tcomp)
    return Ttotal
    

#get the total dist
def sum_distmat(p, distmat):
    dist = 0
    num_location = p.shape[0]
    for i in range(num_location-1):
        dist += distmat[p[i]][p[i+1]]
    dist += distmat[p[0]][p[num_location-1]]
    return dist

#initial dist_matrix
def get_distmat(p):
    num_location = p.shape[0]
    # 1 degree of lat/lon ~ 111km = 111000m in Taiwan
    p *= 111000
    distmat = np.zeros((num_location, num_location))
    for i in range(num_location):
        for j in range(i, num_location):
            distmat[i][j] = distmat[j][i] = np.linalg.norm(p[i] - p[j])
    return distmat

def swap(sol_new):
    while True:
        n1 = np.int(np.floor(np.random.uniform(0, sol_new.shape[0])))
        n2 = np.int(np.floor(np.random.uniform(0, sol_new.shape[0])))
        if n1 != n2:
            break
    sol_new[n1], sol_new[n2] = sol_new[n2], sol_new[n1]
    return sol_new

def reverse(sol_new):
    while True:
        n1 = np.int(np.floor(np.random.uniform(0, sol_new.shape[0])))
        n2 = np.int(np.floor(np.random.uniform(0, sol_new.shape[0])))
        if n1 != n2:
            break
    sol_new[n1:n2] = sol_new[n1:n2][::-1]

    return sol_new

def transpose(sol_new):
    while True:
        n1 = np.int(np.floor(np.random.uniform(0, sol_new.shape[0])))
        n2 = np.int(np.floor(np.random.uniform(0, sol_new.shape[0])))
        n3 = np.int(np.floor(np.random.uniform(0, sol_new.shape[0])))
        if n1 != n2 != n3 != n1:
            break
    #Let n1 < n2 < n3
    n1, n2, n3 = sorted([n1, n2, n3])

    #Insert data between [n1,n2) after n3
    tmplist = sol_new[n1:n2].copy()
    sol_new[n1 : n1+n3-n2+1] = sol_new[n2 : n3+1].copy()
    sol_new[n3-n2+1+n1 : n3+1] = tmplist.copy()
    return sol_new

def accept(cost_new, cost_current, T):
    # If new cost better than current, accept it
    # If new cost not better than current, accept it by probability P(dE)
    # P(dE) = exp(dE/(kT)), defined by Metropolis
    return ( cost_new < cost_current or
             np.random.rand() < np.exp(-(cost_new - cost_current) / T) )

def save_sqlite(payloads):
    conn = sqlite3.connect('data/tsp.db')
    c = conn.cursor()
    c.execute("CREATE TABLE IF NOT EXISTS TSP (costs REAL, route TEXT, markov_step INTEGER) ")
    c.execute('INSERT INTO TSP VALUES (?,?,?)' , payloads)
    conn.commit()
    conn.close()



def plot(path, points, costs):
    '''
    path: List of the different orders in which the nodes are visited
    points: coordinates for the different nodes
    costs: Cost of each iteration
    '''

    # Change figure size
    plt.figure(figsize=(15,6))

    '''
    Plot Cost Function
    '''
    plt.subplot(121)
    curve, = plt.plot(np.array(costs), label='Distance(m)')
    plt.ylabel("Distance")
    plt.xlabel("Iteration")
    plt.grid(True)
    plt.legend()
    cost =  str("%.2f" % round(costs[-1], 2))
    plt.title("Final Distance: " + cost)

    '''
    Plot TSP Route
    '''
    plt.subplot(122)
    # Transform back to longitude/latitude
    points = (points / 111000).tolist()

    # Unpack the primary path and transform it into a list of ordered coordinates
    x = []; y = []
    for i in path:
        x.append(points[i][1])
        y.append(points[i][0])
    x.append(points[path[0]][1])
    y.append(points[path[0]][0])

    # Plot line
    plt.plot(x, y, 'c-', label='Route')

    # Plot dot
    plt.plot(x, y, 'bo', label='Location')

    # Avoid scientific notation
    ax = plt.gca()
    ax.xaxis.set_major_formatter(FormatStrFormatter('%.3f'))
    ax.yaxis.set_major_formatter(FormatStrFormatter('%.3f'))

    # Set axis too slightly larger than the set of x and y
    plt.xlim(min(x)*0.99999, max(x)*1.00001)
    plt.ylim(min(y)*0.99999, max(y)*1.00001)
    plt.xlabel("Longitude")
    plt.ylabel("Latitude")
    plt.title("TSP Route Visualization")
    plt.grid(True)
    plt.show()

