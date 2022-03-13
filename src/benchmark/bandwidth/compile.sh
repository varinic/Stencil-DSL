sw5cc -host -I/usr/sw-mpp/mpi2/include -std=c99 -c bandwidth.c
mpicc -hybrid -o bandwidth bandwidth.o
