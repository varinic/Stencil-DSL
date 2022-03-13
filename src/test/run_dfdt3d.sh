#!/bin/bash
export OMP_NUM_THREADS=32
yhrun -N 512 -n 512 -c 32 -p TH_MT ./dfdt3d