
//==========
//CodeGenCPU {{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "../IO.h"

#include <omp.h>

#include "../mpi_lib/mpi_lib.h"
int mpi_rank ;
int mpi_size ;

float SpNode0[3][10][10];
int var0;
int var1;
float var2 = 0.100000 ;
float var3 = 0.200000 ;
float var4 = 0.150000 ;
void func0(int (var10) , float TeNode0[8][8]) {
  // allocate
  float cache_read  [2+2*1][8+2*1];
  float cache_write [2][8];
# pragma omp parallel for num_threads (2)
  for( int var0_outer = 0; var0_outer < 4; var0_outer += 1 ) {
    //DMA_Get
    Get( &SpNode0[ (var10) % 3 ] [var0_outer*2] [0] , &cache_read[0][0] , (2+2*1)*(8+2*1) );
    //
    for( int var0_inner = 0; var0_inner < 2; var0_inner += 1 ) {
      for( int var1_outer = 0; var1_outer < 8; var1_outer += 1 ) {
        for( int var1_inner = 0; var1_inner < 1; var1_inner += 1 ) {
          //int var0 = var0_outer * 2 + var0_inner ;
          int var0 = var0_inner ;
          int var1 = var1_outer * 1 + var1_inner ;
          //TeNode0[ (var0) ] [ (var1) ] = ( ( ( ( ( (-4) * SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ ( (var1) - (1) )  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ ( (var1) + (1) )  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ ( (var0) - (1) )  + 1] [ (var1)  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ ( (var0) + (1) )  + 1] [ (var1)  + 1] ) ;
          cache_write[ (var0) ] [ (var1) ] = ( ( ( ( ( (-4) * cache_read [ (var0)  + 1] [ (var1)  + 1] ) + cache_read [ (var0)  + 1] [ ( (var1) - (1) )  + 1] ) + cache_read [ (var0)  + 1] [ ( (var1) + (1) )  + 1] ) + cache_read [ ( (var0) - (1) )  + 1] [ (var1)  + 1] ) + cache_read [ ( (var0) + (1) )  + 1] [ (var1)  + 1] ) ;
        }
      }
    }
    //DMA_Put
    Put( &TeNode0 [var0_outer*2] [0] , &cache_write[0][0] , 2*8 );
    //
  }
}
void func1(int (var15) , int (var16) ) {
for( int  (var14) =  (var15) ;  (var14) <=  (var16) ;  (var14) +=1 ) {
float TeNode1[8][8];
 func0( ( (var14) + (-1) ) , TeNode1 );
float TeNode2[8][8];
 func0( ( (var14) + (-2) ) , TeNode2 );
for( int  (var0) = 0;  (var0) < 8;  (var0) +=1 ) {
for( int  (var1) = 0;  (var1) < 8;  (var1) +=1 ) {
 SpNode0[ ( ( (var14) + (0) ) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] = ( ( TeNode1[ (var0) ] [ (var1) ] + TeNode2[ (var0) ] [ (var1) ] ) * (var4) ) ;
}
}
 exchange_halo_2D_float (  &SpNode0[ (var14) % 3 ][0][0], 0, 8, 8, 1, mpi_rank, 1, 2, mpi_size ) ;
}
}
void func2() {
 InputData_float (  &SpNode0[0][0][0], 300, "../data/rand.data" );
 exchange_halo_2D_float (  &SpNode0[ (0) % 3 ][0][0], 0, 8, 8, 1, mpi_rank, 1, 2, mpi_size ) ;
 exchange_halo_2D_float (  &SpNode0[ (1) % 3 ][0][0], 0, 8, 8, 1, mpi_rank, 1, 2, mpi_size ) ;
 exchange_halo_2D_float (  &SpNode0[ (2) % 3 ][0][0], 0, 8, 8, 1, mpi_rank, 1, 2, mpi_size ) ;
}
void func3() {
 func1( (2) , (1000) );
}
int main(){
 MPI_Init(NULL, NULL);
 MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
 MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
 func2();
 func3();
 MPI_Finalize();
 return 0;
}
//}}
//==========
