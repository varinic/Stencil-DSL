
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

float SpNode0[3][5][6];
int var0;
int var1;
float var2 = 0.100000 ;
float var3 = 0.200000 ;
float var4 = 0.150000 ;
void func0(int (var10) , float TeNode0[3][4]) {
# pragma omp parallel for num_threads (3)
for( int var0_outer = 0; var0_outer < 1; var0_outer += 1 ) {
for( int var1_outer = 0; var1_outer < 2; var1_outer += 1 ) {
for( int var0_inner = 0; var0_inner < 3; var0_inner += 1 ) {
for( int var1_inner = 0; var1_inner < 2; var1_inner += 1 ) {
 int var0 = var0_outer * 3 + var0_inner ;
 int var1 = var1_outer * 2 + var1_inner ;
 TeNode0[ (var0) ] [ (var1) ] = ( ( ( ( ( (-4) * SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ ( (var1) - (1) )  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ (var0)  + 1] [ ( (var1) + (1) )  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ ( (var0) - (1) )  + 1] [ (var1)  + 1] ) + SpNode0[ ( (var10) % 3 ) ] [ ( (var0) + (1) )  + 1] [ (var1)  + 1] ) ;
}
}
}
}
}
void func1(int (var11) , float TeNode1[3][4]) {
for( int  (var0) = 0;  (var0) < 3;  (var0) +=1 ) {
for( int  (var1) = 0;  (var1) < 4;  (var1) +=1 ) {
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
for( int  (var0) = 0;  (var0) < 3;  (var0) +=1 ) {
for( int  (var1) = 0;  (var1) < 4;  (var1) +=1 ) {
 SpNode0[ ( ( (var18) + (0) ) % 3 ) ] [ (var0)  + 1] [ (var1)  + 1] = ( ( ( ( (2) * TeNode2[ (var0) ] [ (var1) ] ) + ( ( ( ( ( (0.100000) * (0.100000) ) * (0.200000) ) * (0.200000) ) + ( ( (0.100000) * (0.150000) ) * (0.200000) ) ) * TeNode3[ (var0) ] [ (var1) ] ) ) - TeNode4[ (var0) ] [ (var1) ] ) - ( ( ( ( (0.100000) * (0.150000) ) * (0.200000) ) * (0.200000) ) * TeNode5[ (var0) ] [ (var1) ] ) ) ;
}
}
}
}
void func3() {
 InputData_float (  &SpNode0[0][0][0], 90, "../data/rand.data" );
}
void func4() {
 func2( (2) , (10) );
}
void func5() {
 OutputData_float (  &SpNode0[ (10) % 3 ][0][0], 30, "../data/out.data" );
}
int main(){
 func3();
 func4();
 func5();
 return 0;
}
//}}
//==========
