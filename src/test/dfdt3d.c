
//==========
//CodeGenCPU {{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include "../IO.h"

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + 1e-6 * tv.tv_usec;
}
double time_start;
double time_end;

#include <omp.h>

#include "../mpi_lib/mpi_lib.h"
int mpi_rank ;
int mpi_size ;

float SpNode0[3][130][18][18];
int var0;
int var1;
int var2;
float var3 = 0.100000 ;
float var4 = 0.200000 ;
float var5 = 0.150000 ;
void func0(int (var13) , float TeNode0[128][16][16]) {
# pragma omp parallel for num_threads (16)
for( int var0_outer = 0; var0_outer < 64; var0_outer += 1 ) {
for( int var0_inner = 0; var0_inner < 2; var0_inner += 1 ) {
for( int var1_outer = 0; var1_outer < 4; var1_outer += 1 ) {
for( int var2_outer = 0; var2_outer < 2; var2_outer += 1 ) {
for( int var1_inner = 0; var1_inner < 4; var1_inner += 1 ) {
for( int var2_inner = 0; var2_inner < 8; var2_inner += 1 ) {
 int var0 = var0_outer * 2 + var0_inner ;
 int var1 = var1_outer * 4 + var1_inner ;
 int var2 = var2_outer * 8 + var2_inner ;
 TeNode0[ (var0) ] [ (var1) ] [ (var2) ] = ( ( ( ( ( ( ( (-6) * SpNode0[ ( (var13) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] [ (var2)  + 1] ) + SpNode0[ ( (var13) % 3 ) ] [ ( (var0) - (1) )  + 1] [ (var1)  + 1] [ (var2)  + 1] ) + SpNode0[ ( (var13) % 3 ) ] [ ( (var0) + (1) )  + 1] [ (var1)  + 1] [ (var2)  + 1] ) + SpNode0[ ( (var13) % 3 ) ] [ (var0)  + 1] [ ( (var1) - (1) )  + 1] [ (var2)  + 1] ) + SpNode0[ ( (var13) % 3 ) ] [ (var0)  + 1] [ ( (var1) + (1) )  + 1] [ (var2)  + 1] ) + SpNode0[ ( (var13) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] [ ( (var2) - (1) )  + 1] ) + SpNode0[ ( (var13) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] [ ( (var2) + (1) )  + 1] ) ;
}
}
}
}
}
}
}
void func1(int (var14) , float TeNode1[128][16][16]) {
for( int  (var0) = 0;  (var0) < 128;  (var0) +=1 ) {
for( int  (var1) = 0;  (var1) < 16;  (var1) +=1 ) {
for( int  (var2) = 0;  (var2) < 16;  (var2) +=1 ) {
 TeNode1[ (var0) ] [ (var1) ] [ (var2) ] = SpNode0[ ( (var14) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] [ (var2)  + 1] ;
}
}
}
}
float TeNode2[128][16][16];
float TeNode3[128][16][16];
float TeNode4[128][16][16];
float TeNode5[128][16][16];
void func2(int (var22) , int (var23) ) {
for( int  (var21) =  (var22) ;  (var21) <=  (var23) ;  (var21) +=1 ) {
 func1( ( (var21) + (-1) ) , TeNode2 );
 func0( ( (var21) + (-1) ) , TeNode3 );
 func1( ( (var21) + (-2) ) , TeNode4 );
 func0( ( (var21) + (-2) ) , TeNode5 );
for( int  (var0) = 0;  (var0) < 128;  (var0) +=1 ) {
for( int  (var1) = 0;  (var1) < 16;  (var1) +=1 ) {
for( int  (var2) = 0;  (var2) < 16;  (var2) +=1 ) {
 SpNode0[ ( ( (var21) + (0) ) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] [ (var2)  + 1] = ( ( ( ( (2) * TeNode2[ (var0) ] [ (var1) ] [ (var2) ] ) + ( ( ( ( ( (0.100000) * (0.100000) ) * (0.200000) ) * (0.200000) ) + ( ( (0.100000) * (0.150000) ) * (0.200000) ) ) * TeNode3[ (var0) ] [ (var1) ] [ (var2) ] ) ) - TeNode4[ (var0) ] [ (var1) ] [ (var2) ] ) - ( ( ( ( (0.100000) * (0.150000) ) * (0.200000) ) * (0.200000) ) * TeNode5[ (var0) ] [ (var1) ] [ (var2) ] ) ) ;
}
}
}
 exchange_halo_3D_float (  &SpNode0[ (var21) % 3 ][0][0][0], 0, 128, 16, 16, 1, mpi_rank, 2, 16, 16, mpi_size ) ;
}
}
void func3() {
 InputData_float (  &SpNode0[0][0][0][0], 126360, "../data/input.data" );
 exchange_halo_3D_float (  &SpNode0[ (0) % 3 ][0][0][0], 0, 128, 16, 16, 1, mpi_rank, 2, 16, 16, mpi_size ) ;
 exchange_halo_3D_float (  &SpNode0[ (1) % 3 ][0][0][0], 0, 128, 16, 16, 1, mpi_rank, 2, 16, 16, mpi_size ) ;
 exchange_halo_3D_float (  &SpNode0[ (2) % 3 ][0][0][0], 0, 128, 16, 16, 1, mpi_rank, 2, 16, 16, mpi_size ) ;
}
void func4() {
 func2( (2) , (100) );
}
int main(){
 MPI_Init(NULL, NULL);
 MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
 MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
 func3();
 func4();
 MPI_Finalize();
 return 0;
}
//}}
//==========
