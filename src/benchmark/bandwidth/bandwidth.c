
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <mpi.h>


double grid [1024][32][32];
double time_start;
double time_end;
int timesteps;
MPI_Status status_s, status_r;
MPI_Request handler_s, handler_r;

int mpi_rank ;
int mpi_size ;

void test_bandwidth_in(){
    int halo_size=1;
    double *buffer_send = (double *) malloc ( 1024*32*2*halo_size*sizeof(double) );       
    double *buffer_recv = (double *) malloc ( 1024*32*2*halo_size*sizeof(double) );       
    int cnt=0;
    timesteps = 100;
    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(mpi_rank==0){
      time_start = MPI_Wtime();
    }
    //front
    for (int ca=0;ca<timesteps;ca++){
    for (int i=0;i<1024;i++){
        for(int j=0;j<32;j++){
            buffer_send[cnt++]=grid[i][1][j];
        }
    }
    }
    //
    if(mpi_rank==0){
      time_end = MPI_Wtime();
      printf("\n mpi rank: %d\n total millisecond: %.2f\n timestep: %d\n avg millesecond: %.2f, in: bandwidth(B/s) %.4f\n",
              mpi_rank,(time_end-time_start)*1000,timesteps,(time_end-time_start)/(timesteps)*1000,
              1024*32*1*sizeof(double)/((time_end-time_start)/(timesteps)));
    }
    //
    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(mpi_rank==0){
      time_start = MPI_Wtime();
    }
    //left
    cnt=0;
    for (int ca=0;ca<timesteps;ca++){
    for (int i=0;i<1024;i++){
        for(int j=0;j<32;j++){
            buffer_send[cnt++]=grid[i][j][1];
        }
    }
    }
    //
    if(mpi_rank==0){
      time_end = MPI_Wtime();
      printf("\n mpi rank: %d\n total millisecond: %.2f\n timestep: %d\n avg millesecond: %.2f, in: bandwidth(B/s) %.4f\n",
              mpi_rank,(time_end-time_start)*1000,timesteps,(time_end-time_start)/(timesteps)*1000,
              1024*32*1*sizeof(double)/((time_end-time_start)/(timesteps)));
    }
    //
    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(mpi_rank==0){
      time_start = MPI_Wtime();
    }
    //down
    cnt=0;
    for (int ca=0;ca<timesteps;ca++){
    for (int i=0;i<1;i++){
        for(int j=0;j<32;j++){
            for (int k=0;k<32;k++)
            buffer_send[cnt++]=grid[i][j][k];
        }
    }
    }
    //
    if(mpi_rank==0){
      time_end = MPI_Wtime();
      printf("\n mpi rank: %d\n total millisecond: %.2f\n timestep: %d\n avg millesecond: %.2f, in: bandwidth(B/s) %.4f\n",
              mpi_rank,(time_end-time_start)*1000,timesteps,(time_end-time_start)/(timesteps)*1000,
              1*32*32*sizeof(double)/((time_end-time_start)/(timesteps)));
    }
    //
 
    free(buffer_send);
    free(buffer_recv);
}
void test_bandwidth_out(){
    int halo_size=1;
    double *buffer_send = (double *) malloc ( 1024*32*2*halo_size*sizeof(double) );       
    double *buffer_recv = (double *) malloc ( 1024*32*2*halo_size*sizeof(double) );       
    int cnt=0;
    timesteps = 100;
    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(mpi_rank==0){
      time_start = MPI_Wtime();
    }
    //front
    for (int ca=0;ca<timesteps;ca++){
    for (int i=0;i<1024;i++){
        for(int j=0;j<32;j++){
            grid[i][1][j]=buffer_send[cnt++];
        }
    }
    }
    //
    if(mpi_rank==0){
      time_end = MPI_Wtime();
      printf("\n mpi rank: %d\n total millisecond: %.2f\n timestep: %d\n avg millesecond: %.2f, out: bandwidth(B/s) %.4f\n",
              mpi_rank,(time_end-time_start)*1000,timesteps,(time_end-time_start)/(timesteps)*1000,
              1024*32*1*sizeof(double)/((time_end-time_start)/(timesteps)));
    }
    //
    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(mpi_rank==0){
      time_start = MPI_Wtime();
    }
    //left
    cnt=0;
    for (int ca=0;ca<timesteps;ca++){
    for (int i=0;i<1024;i++){
        for(int j=0;j<32;j++){
            grid[i][j][1]=buffer_send[cnt++];
        }
    }
    }
    //
    if(mpi_rank==0){
      time_end = MPI_Wtime();
      printf("\n mpi rank: %d\n total millisecond: %.2f\n timestep: %d\n avg millesecond: %.2f, out: bandwidth(B/s) %.4f\n",
              mpi_rank,(time_end-time_start)*1000,timesteps,(time_end-time_start)/(timesteps)*1000,
              1024*32*1*sizeof(double)/((time_end-time_start)/(timesteps)));
    }
    //
    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(mpi_rank==0){
      time_start = MPI_Wtime();
    }
    //down
    cnt=0;
    for (int ca=0;ca<timesteps;ca++){
    for (int i=0;i<1;i++){
        for(int j=0;j<32;j++){
            for (int k=0;k<32;k++)
            grid[i][j][k]=buffer_send[cnt++];
        }
    }
    }
    //
    if(mpi_rank==0){
      time_end = MPI_Wtime();
      printf("\n mpi rank: %d\n total millisecond: %.2f\n timestep: %d\n avg millesecond: %.2f, out: bandwidth(B/s) %.4f\n",
              mpi_rank,(time_end-time_start)*1000,timesteps,(time_end-time_start)/(timesteps)*1000,
              1*32*32*sizeof(double)/((time_end-time_start)/(timesteps)));
    }
    //
 
    free(buffer_send);
    free(buffer_recv);
}

void test_bandwidth_mpi(){
    int halo_size=1;
    double *buffer_send = (double *) malloc ( 1024*32*2*halo_size*sizeof(double) );       
    double *buffer_recv = (double *) malloc ( 1024*32*2*halo_size*sizeof(double) );       
    int cnt=0;
    timesteps = 100;
    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(mpi_rank==0){
      time_start = MPI_Wtime();
    }
    //
    for(int ca=0;ca<timesteps;ca++){
    cnt=1024*32;
    MPI_Isend (buffer_send, cnt, MPI_DOUBLE, (mpi_rank+1)%2, 0, MPI_COMM_WORLD, &handler_s);   
    MPI_Irecv (buffer_recv, cnt, MPI_DOUBLE, (mpi_rank+1)%2, 0, MPI_COMM_WORLD, &handler_r);   
    /*wait*/   
    MPI_Wait(&handler_s, &status_s);   
    MPI_Wait(&handler_r, &status_r);  
    }
    //
    if(mpi_rank==0){
      time_end = MPI_Wtime();
      printf("\n mpi rank: %d\n total millisecond: %.2f\n timestep: %d\n avg millesecond: %.2f, mpi: bandwidth(B/s) %.4f\n",
              mpi_rank,(time_end-time_start)*1000,timesteps,(time_end-time_start)/(timesteps)*1000,
              1024*32*1*sizeof(double)/((time_end-time_start)/(timesteps)));
    }
    //
    free(buffer_send);
    free(buffer_recv);
}



int main(){
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    //test_bandwidth_in();
    //test_bandwidth_out();
    test_bandwidth_mpi();
    return 0;
}
