
//==========
//CodeGenCPU {{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#include "IO.h"

float SpNode0[3][5][6];
int var0;
int var1;
float var2;
float var3;
float var4;
void func0(int (var10) , float TeNode0[3][4]) {
# pragma omp parallel for num_threads(8)
for( int  (var0) = 0 ;  (var0) < 3;  (var0) +=1 ) {
for( int  (var1) = 0 ;  (var1) < 4;  (var1) +=1 ) {
 TeNode0[ (var0) ] [ (var1) ] = ( ( ( ( ( (-4) * SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ ( (var1) - (1) )  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ ( (var1) + (1) )  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ ( (var0) - (1) )  + 1] [ (var1)  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ ( (var0) + (1) )  + 1] [ (var1)  + 1] ) ;
}
}
}
void func1(int (var11) , float TeNode1[3][4]) {
# pragma omp parallel for num_threads(8)
for( int  (var0) = 0 ;  (var0) < 3;  (var0) +=1 ) {
for( int  (var1) = 0 ;  (var1) < 4;  (var1) +=1 ) {
 TeNode1[ (var0) ] [ (var1) ] = SpNode0[ ( (var11) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] ;
}
}
}
void func2(int (var19) , int (var20) ) {
for( int  (var18) =  (var19) ;  (var18) <=  (var20) ;  (var18) +=1 ) {
float TeNode2[3][4];
 func1( ( (var18) + (-1) ) , TeNode2 );
float TeNode3[3][4];
 func0( ( (var18) + (-1) ) , TeNode3 );
float TeNode4[3][4];
 func1( ( (var18) + (-2) ) , TeNode4 );
float TeNode5[3][4];
 func0( ( (var18) + (-2) ) , TeNode5 );

 //exchange halo start
 exchange_halo_2D_float_start(&SpNode0[ ((var18) - (1) +(3)) % 3 ][0][0], 3, d1, d2 , halo_size, /*pid*/ mpi_rank, D1, D2, mpi_size);
 //intra area
 const int halo_size =1;
# pragma omp parallel for num_threads(8)
for( int  (var0) = 0+halo_size ;  (var0) < 3-halo_size;  (var0) +=1 ) {
  for( int  (var1) = 0+halo_size ;  (var1) < 4-halo_size;  (var1) +=1 ) {
    SpNode0[ ( ( (var18) + (0) ) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] = ( ( ( ( (2) * TeNode2[ (var0) ] [ (var1) ] ) + ( ( ( ( ( (var2) * (var2) ) * (var3) ) * (var3) ) + ( ( (var2) * (var4) ) * (var3) ) ) * TeNode3[ (var0) ] [ (var1) ] ) ) - TeNode4[ (var0) ] [ (var1) ] ) - ( ( ( ( (var2) * (var4) ) * (var3) ) * (var3) ) * TeNode5[ (var0) ] [ (var1) ] ) ) ;
  }
}
 //exchange halo end
 exchange_halo_2D_float_end(&SpNode0[ ((var18) - (1) +(3)) % 3 ][0][0], 3, d1, d2 , halo_size, /*pid*/ mpi_rank, D1, D2, mpi_size);
 //outer area
 # pragma omp parallel for num_threads(8)
for( int  (var0) = 0 ;  (var0) < 3;  (var0) +=1 ) {
  for( int  (var1) = 0 ;  (var1) < 4;  (var1) +=1 ) {
    if( var0<0+halo_size ||var0 >= 3-halo_size || var1<0+halo_size ||var1 >= 4-halo_size ){
      SpNode0[ ( ( (var18) + (0) ) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] = ( ( ( ( (2) * TeNode2[ (var0) ] [ (var1) ] ) + ( ( ( ( ( (var2) * (var2) ) * (var3) ) * (var3) ) + ( ( (var2) * (var4) ) * (var3) ) ) * TeNode3[ (var0) ] [ (var1) ] ) ) - TeNode4[ (var0) ] [ (var1) ] ) - ( ( ( ( (var2) * (var4) ) * (var3) ) * (var3) ) * TeNode5[ (var0) ] [ (var1) ] ) ) ;
    }
  }
}
  // 
}
}
void func3() {
 InputData_float (  &SpNode0[ (1) % 3 ][0][0], 30, "./data/rand.data" );
 exchange_halo_2D_float(&SpNode0[ (1) % 3 ][0][0], 3, d1, d2 , halo_size, /*pid*/ mpi_rank, D1, D2, mpi_size);
}
void func4() {
 func2( (2) , (10) );
}
void func5() {
 OutputData_float (  &SpNode0[ (10) % 3 ][0][0], 30, "./data/out.data" );
}
int main(){
   func3();
   func4();
   func5();
 return 0;
}
//}}
//==========
