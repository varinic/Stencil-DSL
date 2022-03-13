#coding:utf-8
import numpy as np
import json
import math
from util import *
from args import *

def simul():
    # Get arguments
    args = parse_args()


    # Params Initial

    markov_step = args.markov_coefficient
    T_0, T, T_MIN = args.init_temperature, args.init_temperature, 1
    T_NUM_CYCLE = 1
    #
    #print(args.markov_coefficient)
    #print(args.init_temperature)
    #print(args.halt)
    #append2file("./out/best_sol.data", [1,2,3,4,5,6])
    #append2file("./out/best_sol.data", [1,2,3,4,5,6])

    # Build distance matrix to accelerate cost computing
    #distmat = get_distmat(coordinates)

    #initial
    grid = [8192,128,128]
    #p1~p6
    init_param = [4096,8,2,2,4,16]
    #init_param = [1,8,8,16,4,2]
    #init_param = [1,8,8,16,8,2]
    #init_param = [1,8,8,16,8,8]
    #init_param = [1,64,64,64,64,64]
    #kinit_param = [1,4,4,4,4,4]
    #init_param = [1,4,4,8,4,4]
    #num_mpi
    num_mpi = 128
    log_num_mpi=7
    # States: New, Current and Best
    sol_new  = init_param.copy()
    sol_current = init_param.copy()
    sol_best = init_param.copy()
    sol_mid = init_param.copy()
    #print(sol_new)
    #print(sol_current)
    #print(sol_best)
    cost_new, cost_current, cost_best , cost_mid = (float('inf'), ) * 4
    #print(cost_new)
    #print(cost_current)
    #print(cost_best)
    #for i in range(1,100):
    #    print(random.randint(1,2))
    #
    #print(change_sol(grid,log_num_mpi, sol_new))

    # Record costs during the process
    costs = []

    # previous cost_best
    prev_cost_best = cost_best

    # counter for detecting how stable the cost_best currently is
    cost_best_counter = 0

    # Simulated Annealing
    while T > T_MIN and cost_best_counter < args.halt:
        for i in np.arange(markov_step):
            # Use three different methods to generate new solution
            #print(sol_new)
            sol_new = change_sol(grid, log_num_mpi, sol_new)
            #print(sol_new)
            if(check(grid, num_mpi, sol_new)==False):
                continue
            #print(sol_new)
            #print('check true')
            # Get the total distance of new route
            #cost_new = sum_distmat(sol_new, distmat)
            cost_new = performance_model(grid, num_mpi, sol_new)
            cost_mid = cost_new
            sol_mid = sol_new.copy()

            if accept(cost_new, cost_current, T):
                # Update sol_current
                sol_current = sol_new.copy()
                cost_current = cost_new

                if cost_new < cost_best:
                    sol_best = sol_new.copy()
                    #print(sol_best)
                    cost_best = cost_new
            else:
                sol_new = sol_current.copy()

        # Lower the temperature
        alpha = 1 + math.log(1 + T_NUM_CYCLE)
        T = T_0 / alpha
        costs.append(cost_best)

        # Increment T_NUM_CYCLE
        T_NUM_CYCLE += 1

        # Detect stability of cost_best
        if isclose(cost_best, prev_cost_best, abs_tol=1e-12):
          cost_best_counter += 1
        else:
          # Not stable yet, reset
          cost_best_counter = 0

        #
        #Flag = isclose(cost_best, prev_cost_best, abs_tol=1e-12)
        #if Flag == True:
            #print('T_NUM_CYCLE, ', T_NUM_CYCLE, ', cost_mid, ', cost_mid*1000 ,  ', cost_current, ', cost_current*1000, ', cost_best, ', cost_best*1000)
            #Monitor the temperature & cost
            #print("Temperature:", "%.2f°C" % round(T, 2),
            #      " Time:", "%.2f ms" % round(cost_best*1000, 2),
            #      " Optimization Threshold:", "%d" % cost_best_counter)
            #print('T_NUM_CYCLE, ', T_NUM_CYCLE, "sol_mid",sol_mid, 'sol_current', sol_current,'cost_mid', cost_mid)
            #print('T_NUM_CYCLE, ', T_NUM_CYCLE, ', cost_mid, ', cost_mid)

        # Update prev_cost_best
        prev_cost_best = cost_best

        print('T_NUM_CYCLE, ', T_NUM_CYCLE, ', cost_mid, ', cost_mid*1000 ,  ', cost_current, ', cost_current*1000, ', cost_best, ', cost_best*1000)

        # Monitor the temperature & cost
        #print("Temperature:", "%.2f°C" % round(T, 2),
        #      " Time:", "%.2f second" % round(cost_best, 2),
        #      " Optimization Threshold:", "%d" % cost_best_counter)
        #print('T_NUM_CYCLE, ', T_NUM_CYCLE, "sol_mid",sol_mid, 'sol_current', sol_current,'cost_mid', cost_mid)
        #print('T_NUM_CYCLE, ', T_NUM_CYCLE, ', cost_mid, ', cost_mid)

    # Show final cost & route
    #print("Final Time: ", round(costs[-1], 2))
    #print("Best Solution", sol_best)

if __name__ == "__main__":
    simul()
