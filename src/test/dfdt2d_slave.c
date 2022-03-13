
//==========
//CodeGenSunwaySlave {{
#include "slave.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

__thread_local volatile unsigned long get_reply, put_reply;
__thread_local volatile int my_id;

typedef struct Argfloat {
  float * TeNode;
  float * SpNodeTemp;
} Arg_float;
typedef struct Argdouble {
  float * TeNode;
  float * SpNodeTemp;
} Arg_double;
typedef struct Argint {
  float * TeNode;
  float * SpNodeTemp;
} Arg_int;

#define Get(x, y, cnt) { \
  volatile unsigned long get_reply = 0 ; \
  athread_get(PE_MODE, x, y, cnt, &get_reply, 0, 0, 0); \
  while(get_reply != 1) ; \
}  
#define Put(x, y, cnt) { \
  volatile unsigned long put_reply = 0 ; \
  athread_put(PE_MODE, y, x, cnt, &put_reply, 0, 0); \
  while(put_reply != 1) ; \
}  

void func0( Arg_float * arg ) {
 my_id = athread_get_id(-1);
 int var0_outer ;
 int var0_inner ;
 int var1_outer ;
 int var1_inner ;
float cache_read [4][10];
float cache_write[2][8];
float (*SpNode)  [10]= ( float (*) [10]) arg->SpNodeTemp;
float (*TeNode) [8]= ( float (*) [8]) arg->TeNode;
for( var0_outer = my_id; var0_outer < 64; var0_outer += 64 ) {
//DMA_get
Get( &SpNode[var0_outer * 2 ][0], &cache_read[0][0], 40 * sizeof(float) );
for( var0_inner = 0; var0_inner < 2; var0_inner += 1 ) {
for( var1_outer = 0; var1_outer < 2; var1_outer += 1 ) {
for( var1_inner = 0; var1_inner < 4; var1_inner += 1 ) {
 int var0 = var0_inner ;
 int var1 = var1_outer * 4 + var1_inner ;
 cache_write[ (var0) ] [ (var1) ] = ( ( ( ( ( (-4) * cache_read[ (var0)  + 1] [ (var1)  + 1] ) + cache_read[ (var0)  + 1] [ ( (var1) - (1) )  + 1] ) + cache_read[ (var0)  + 1] [ ( (var1) + (1) )  + 1] ) + cache_read[ ( (var0) - (1) )  + 1] [ (var1)  + 1] ) + cache_read[ ( (var0) + (1) )  + 1] [ (var1)  + 1] ) ;
}
}
}
//DMA_put
 Put( &TeNode[var0_outer * 2 ][0], &cache_write[0][0], 256 * sizeof(float) );
}
}
//}}
//==========
