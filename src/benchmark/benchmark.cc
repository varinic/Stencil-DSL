#include <iostream>
#include <vector>
#include "../ir.h"
#include <cmath>

//config
const int cores = 32;
const int cores_sunway = 64;
const std::string input_file = "../data/rand.data";
//set datatype in here 
//#define DT f64
#define DT f32
//

// 2d9pt star
/* 
  --|--|--|--|--
    |  |* |  | 
  --|--|--|--|--
    |  |* |  | 
  --|--|--|--|--
  * |* |* |* |*
  --|--|--|--|--
    |  |* |  | 
  --|--|--|--|--
    |  |* |  | 
  --|--|--|--|--
*/

/*
  double a[M][N];
  double b[M][N];
  double c0;double c1;double c2;
  double c3;double c4;double c5;
  double c6;double c7;double c8;
  for(long k=2; k < M-2; ++k){
    for(long j=2; j < N-2; ++j){
      b[k][j] = c0 * a[k][j]
        + c1 * a[k][j-1] + c2 * a[k][j-2]
        + c3 * a[k][j+1] + c4 * a[k][j+2]
        + c5 * a[k-1][j] + c6 * a[k-2][j]
        + c7 * a[k+1][j] + c8 * a[k+2][j] ;
    }
  }
*/
void bench_2d9pt_star_sunway_bench(std::string target, int num_threads, int D1, int D2,
        int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048, int use_schedule=1, int is_assigned=1){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, DT, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, DT, 0.1);
  DefVar_Value(c1, DT, 0.2);
  DefVar_Value(c2, DT, 0.3);
  DefVar_Value(c3, DT, 0.4);
  DefVar_Value(c4, DT, 0.5);
  DefVar_Value(c5, DT, 0.4);
  DefVar_Value(c6, DT, 0.3);
  DefVar_Value(c7, DT, 0.2);
  DefVar_Value(c8, DT, 0.1);
  //
  if(use_schedule){
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k,j-2] + c3* B[k,j+1] + c4* B[k,j+2] + c5* B[k-1,j] + c6* B[k-2,j] + c7* B[k+1,j] + c8* B[k+2,j] , schedule );
    //
    Axis xo, yo, xi, yi;
    A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
    A.reorder(xo, xi, yo, yi);
    CacheRead  buffer_read;
    CacheWrite buffer_write;
    A.cache_read  (B, buffer_read,  "global");
    A.cache_write (buffer_write, "global");
    A.compute_at(buffer_read,  xo);
    A.compute_at(buffer_write, xo);
    A.parallel(xo, num_threads);
    A.build( target );
    //
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] ,0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_2d9pt_star_sunway_bench.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_star", target, use_timing);
  }
  else{
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k,j-2] + c3* B[k,j+1] + c4* B[k,j+2] + c5* B[k-1,j] + c6* B[k-2,j] + c7* B[k+1,j] + c8* B[k+2,j] );
    //
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] ,0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_2d9pt_star_sunway_bench_seq.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_star", target, use_timing);
 
  }
}


void bench_2d9pt_star_sunway(std::string target, int num_threads, int D1, int D2,
        int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k,j-2] + c3* B[k,j+1] + c4* B[k,j+2] + c5* B[k-1,j] + c6* B[k-2,j] + c7* B[k+1,j] + c8* B[k+2,j] , schedule );
  //
  Axis xo, yo, xi, yi;
  A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  A.reorder(xo, xi, yo, yi);
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j) , B[k,j] );
  //
  DefShapeMPI2D(shape_mpi, D1, D2);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] );

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("2d9pt_star", target, use_timing);
}

void bench_2d9pt_star_SharedMem_mpi_bench_schedule(std::string target, int num_threads, int D1, int D2,
        int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048, 
        int use_schedule=1, int use_tile=1, int use_reorder=1, int use_threads=0, int is_assigned=0){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  if(use_schedule){
    std::cout<<"use schedule"<<std::endl;
    assert(use_tile>0);
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k,j-2] + c3* B[k,j+1] + c4* B[k,j+2] + c5* B[k-1,j] + c6* B[k-2,j] + c7* B[k+1,j] + c8* B[k+2,j] , schedule );
    //
    if(use_tile){
    std::cout<<"use tile"<<std::endl;
    Axis xo, yo, xi, yi;
    A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
    if(use_reorder){
    std::cout<<"use reorder"<<std::endl;
    A.reorder(xo, yo, xi, yi);
    }
    A.parallel(xo, num_threads);
    A.build( target );
    }
    //
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,1000);
    const int time_step10 = 1000;
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_star", target, use_timing);
  }
  else{
    std::cout<<"not use schedule"<<std::endl;
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k,j-2] + c3* B[k,j+1] + c4* B[k,j+2] + c5* B[k-1,j] + c6* B[k-2,j] + c7* B[k+1,j] + c8* B[k+2,j], num_threads );
    //
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,1000);
    const int time_step10 = 1000;
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_star", target, use_timing);
  }
}

void bench_2d9pt_star_SharedMem_mpi(std::string target, int num_threads, int D1, int D2,
        int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
/*
  const int M = 4096;
  const int N = 4096;
*/
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k,j-2] + c3* B[k,j+1] + c4* B[k,j+2] + c5* B[k-1,j] + c6* B[k-2,j] + c7* B[k+1,j] + c8* B[k+2,j] , schedule );
/*
  const int tile_size_x = 2;
  const int tile_size_y = 2048;
*/
  Axis xo, yo, xi, yi;
  A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  A.reorder(xo, yo, xi, yi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j) , B[k,j] );
  //
  DefShapeMPI2D(shape_mpi, D1, D2);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , num_threads);

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("2d9pt_star", target, use_timing);
}

void bench_2d9pt_star_SharedMem(std::string target, int num_threads){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  const int M = 4096;
  const int N = 4096;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k,j-2] + c3* B[k,j+1] + c4* B[k,j+2] + c5* B[k-1,j] + c6* B[k-2,j] + c7* B[k+1,j] + c8* B[k+2,j] , schedule );
  const int tile_size_x = 2;
  const int tile_size_y = 2048;
  Axis xo, yo, xi, yi;
  A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  A.reorder(xo, yo, xi, yi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j) , B[k,j] );
  //
  auto t = Stencil::t;
  const int use_threads = 0;
  bool is_assigned =false;
  Stencil stencil( (k,j) , Res[t] << A[t-1], use_threads, is_assigned );

  stencil.input( B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code("2d9pt_star", target, use_timing);
}

// 2d9pt box
/* 
  --|--|--
  * |* |*
  --|--|--
  * |* |*
  --|--|--
  * |* |*
  --|--|--
*/
/*
double a[M][N];
double b[M][N];
double c0;
double c1;
double c2;
double c3;
double c4;
double c5;
double c6;
double c7;
double c8;
for(long k=1; k < M-1; ++k){
for(long j=1; j < N-1; ++j){
b[k][j] = c0 * a[k][j]
+ c1 * a[k][j-1] + c2 * a[k-1][j]
+ c3 * a[k][j+1] + c4 * a[k+1][j]
+ c5 * a[k-1][j+1] + c6 * a[k+1][j-1]
+ c7 * a[k-1][j-1] + c8 * a[k+1][j+1]
;
}
}
*/
void bench_2d9pt_box_sunway_bench(std::string target, int num_threads, int D1, int D2,
  int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048, int use_schedule=1, int is_assigned=1){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, DT, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, DT, 0.1);
  DefVar_Value(c1, DT, 0.2);
  DefVar_Value(c2, DT, 0.3);
  DefVar_Value(c3, DT, 0.4);
  DefVar_Value(c4, DT, 0.5);
  DefVar_Value(c5, DT, 0.4);
  DefVar_Value(c6, DT, 0.3);
  DefVar_Value(c7, DT, 0.2);
  DefVar_Value(c8, DT, 0.1);
  //
  if(use_schedule){
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k-1,j] + c3* B[k,j+1] + c4* B[k+1,j] + c5* B[k-1,j+1] + c6* B[k+1,j-1] + c7* B[k-1,j-1] + c8* B[k+1,j+1] , schedule );
    Axis xo, yo, xi, yi;
    A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
    A.reorder(xo, xi, yo, yi);
    CacheRead  buffer_read;
    CacheWrite buffer_write;
    A.cache_read  (B, buffer_read,  "global");
    A.cache_write (buffer_write, "global");
    A.compute_at(buffer_read,  xo);
    A.compute_at(buffer_write, xo);
    assert(num_threads==64);
    A.parallel(xo, num_threads);
    A.build( target );
    //
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , 0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_2d9pt_box_sunway_bench.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_box", target, use_timing);
  }
  else{
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k-1,j] + c3* B[k,j+1] + c4* B[k+1,j] + c5* B[k-1,j+1] + c6* B[k+1,j-1] + c7* B[k-1,j-1] + c8* B[k+1,j+1] );
    //
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , 0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_2d9pt_box_sunway_bench_seq.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_box", target, use_timing);
 
  }
}

void bench_2d9pt_box_sunway(std::string target, int num_threads, int D1, int D2,
  int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k-1,j] + c3* B[k,j+1] + c4* B[k+1,j] + c5* B[k-1,j+1] + c6* B[k+1,j-1] + c7* B[k-1,j-1] + c8* B[k+1,j+1] , schedule );
  Axis xo, yo, xi, yi;
  A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  A.reorder(xo, xi, yo, yi);
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  assert(num_threads==64);
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j) , B[k,j] );
  //
  DefShapeMPI2D(shape_mpi, D1, D2);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] );

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("2d9pt_box", target, use_timing);
}

void bench_2d9pt_box_SharedMem_mpi_bench_schedule(std::string target, int num_threads, int D1, int D2,
          int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048, 
          int use_schedule=1, int use_tile=1, int use_reorder=1, int use_threads=0, int is_assigned=0){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  if(use_schedule){
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k-1,j] + c3* B[k,j+1] + c4* B[k+1,j] + c5* B[k-1,j+1] + c6* B[k+1,j-1] + c7* B[k-1,j-1] + c8* B[k+1,j+1] , schedule );
    //
    if(use_tile){
    Axis xo, yo, xi, yi;
    A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
    if(use_reorder){
    A.reorder(xo, yo, xi, yi);
    }
    A.parallel(xo, num_threads);
    A.build( target );
    }
    //
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_box", target, use_timing);
  }
  else{
    Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k-1,j] + c3* B[k,j+1] + c4* B[k+1,j] + c5* B[k-1,j+1] + c6* B[k+1,j-1] + c7* B[k-1,j-1] + c8* B[k+1,j+1] , num_threads );
    Result Res( (k,j) , B[k,j] );
    //
    DefShapeMPI2D(shape_mpi, D1, D2);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("2d9pt_box", target, use_timing);

  }
}

void bench_2d9pt_box_SharedMem_mpi(std::string target, int num_threads, int D1, int D2,
  int M=4096, int N=4096, int tile_size_x=2,  int tile_size_y=2048){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
/*
  const int M = 4096;
  const int N = 4096;
*/
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k-1,j] + c3* B[k,j+1] + c4* B[k+1,j] + c5* B[k-1,j+1] + c6* B[k+1,j-1] + c7* B[k-1,j-1] + c8* B[k+1,j+1] , schedule );
/*
  const int tile_size_x = 2;
  const int tile_size_y = 2048;
*/
  Axis xo, yo, xi, yi;
  A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  A.reorder(xo, yo, xi, yi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j) , B[k,j] );
  //
  DefShapeMPI2D(shape_mpi, D1, D2);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j) , Res[t] << A[t-1] , num_threads);

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("2d9pt_box", target, use_timing);
}

void bench_2d9pt_box_SharedMem(std::string target, int num_threads){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  const int M = 4096;
  const int N = 4096;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f64, M, N)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.5);
  DefVar_Value(c5, f64, 0.4);
  DefVar_Value(c6, f64, 0.3);
  DefVar_Value(c7, f64, 0.2);
  DefVar_Value(c8, f64, 0.1);
  //
  Kernel A( (k,j) , c0* B[k,j] + c1* B[k,j-1] + c2* B[k-1,j] + c3* B[k,j+1] + c4* B[k+1,j] + c5* B[k-1,j+1] + c6* B[k+1,j-1] + c7* B[k-1,j-1] + c8* B[k+1,j+1] , schedule );
  const int tile_size_x = 2;
  const int tile_size_y = 2048;
  Axis xo, yo, xi, yi;
  A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  A.reorder(xo, yo, xi, yi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j) , B[k,j] );
  //
  auto t = Stencil::t;
  const int use_threads = 0;
  bool is_assigned =false;
  Stencil stencil( (k,j) , Res[t] << A[t-1], use_threads, is_assigned  );

  stencil.input( B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code("2d9pt_box", target, use_timing);
}

// 3d7pt star
/*
double a[M][N][P];
double b[M][N][P];
double c0;
double c1;
double c2;
double c3;
double c4;
double c5;
double c6;
for(long k=1; k < M-1; ++k){
for(long j=1; j < N-1; ++j){
for(long i=1; i < P-1; ++i){
b[k][j][i] = c0 * a[k][j][i]
+ c1 * a[k][j][i-1] + c2 * a[k][j][i+1]
+ c3 * a[k-1][j][i] + c4 * a[k+1][j][i]
+ c5 * a[k][j-1][i] + c6 * a[k][j+1][i]
;
}
}
}
*/
void bench_3d7pt_star_SharedMem(std::string target, int num_threads){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  const int M = 256;
  const int N = 256;
  const int P = 256;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.3);
  DefVar_Value(c5, f64, 0.2);
  DefVar_Value(c6, f64, 0.1);
  //
  Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] , schedule );
  const int tile_size_x = 2;
  const int tile_size_y = 8;
  const int tile_size_z = 256;
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, yo, zo, xi, yi, zi);
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , B[k,j,i] );
  //
  auto t = Stencil::t;
  const int use_threads = 0;
  bool is_assigned =false;
  Stencil stencil( (k,j,i) , Res[t] << A[t-1], use_threads, is_assigned  );

  stencil.input( B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code("3d7pt_star", target, use_timing);
}
void bench_3d7pt_star_sunway_bench(std::string target, int num_threads, int D1, int D2, int D3,
  int M=256, int N=256, int P=256, int tile_size_x=2,  int tile_size_y=8, int tile_size_z=256, int use_schedule=1, int is_assigned=1){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, DT, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, DT, 0.1);
  DefVar_Value(c1, DT, 0.2);
  DefVar_Value(c2, DT, 0.3);
  DefVar_Value(c3, DT, 0.4);
  DefVar_Value(c4, DT, 0.3);
  DefVar_Value(c5, DT, 0.2);
  DefVar_Value(c6, DT, 0.1);
  //
  if(use_schedule){
    Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] , schedule );
    Axis xo, yo, xi, yi, zo, zi;
    A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
    A.reorder(xo, xi, yo, zo, yi, zi);
    CacheRead  buffer_read;
    CacheWrite buffer_write;
    A.cache_read  (B, buffer_read,  "global");
    A.cache_write (buffer_write, "global");
    A.compute_at(buffer_read,  xo);
    A.compute_at(buffer_write, xo);
    assert(num_threads==64);
    A.parallel(xo, num_threads);
    A.build( target );
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] ,0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_3d7pt_star_sunway_bench.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d7pt_star", target, use_timing);
  }
  else{
    Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] );
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] ,0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_3d7pt_star_sunway_bench_seq.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d7pt_star", target, use_timing);
 
  }
}


void bench_3d7pt_star_sunway(std::string target, int num_threads, int D1, int D2, int D3,
  int M=256, int N=256, int P=256, int tile_size_x=2,  int tile_size_y=8, int tile_size_z=256){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.3);
  DefVar_Value(c5, f64, 0.2);
  DefVar_Value(c6, f64, 0.1);
  //
  Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] , schedule );
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, xi, yo, zo, yi, zi);
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  assert(num_threads==64);
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , B[k,j,i] );
  //
  DefShapeMPI3D(shape_mpi, D1, D2, D3);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] );

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("3d7pt_star", target, use_timing);
}

void bench_3d7pt_star_SharedMem_mpi_bench_schedule(std::string target, int num_threads, int D1, int D2, int D3,
  int M=256, int N=256, int P=256, int tile_size_x=2,  int tile_size_y=8, int tile_size_z=256, int use_schedule=1, int use_tile=1, int use_reorder=1, int use_threads=0, int is_assigned=0){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.3);
  DefVar_Value(c5, f64, 0.2);
  DefVar_Value(c6, f64, 0.1);
  //
  if(use_schedule){
    Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] , schedule );
    //
    if(use_tile){
    Axis xo, yo, xi, yi, zo, zi;
    A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
    if(use_reorder){
    A.reorder(xo, yo, zo, xi, yi, zi);
    }
    A.parallel(xo, num_threads);
    A.build( target );
    }
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    //stencil.output(B, time_step10, "../data/out.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d7pt_star", target, use_timing);
  }
  else{
    Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] , num_threads );
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    //stencil.output(B, time_step10, "../data/out.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d7pt_star", target, use_timing);
  }
}

void bench_3d7pt_star_SharedMem_mpi(std::string target, int num_threads, int D1, int D2, int D3,
  int M=256, int N=256, int P=256, int tile_size_x=2,  int tile_size_y=8, int tile_size_z=256){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
/*
  const int M = 256;
  const int N = 256;
  const int P = 256;
*/
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.3);
  DefVar_Value(c5, f64, 0.2);
  DefVar_Value(c6, f64, 0.1);
  //
  Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] , schedule );
/*
  const int tile_size_x = 2;
  const int tile_size_y = 8;
  const int tile_size_z = 256;
*/
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, yo, zo, xi, yi, zi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , B[k,j,i] );
  //
  DefShapeMPI3D(shape_mpi, D1, D2, D3);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , num_threads);

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("3d7pt_star", target, use_timing);
}


void bench_3d7pt_star_sunway(){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  const int M = 1024;
  const int N = 32;
  const int P = 32;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.3);
  DefVar_Value(c5, f64, 0.2);
  DefVar_Value(c6, f64, 0.1);
  //
  Kernel A( (k,j,i) , c0* B[k,j,i] + c1* B[k,j,i-1] + c2* B[k,j,i+1] + c3* B[k-1,j,i] + c4* B[k+1,j,i] + c5* B[k,j-1,i] + c6* B[k,j+1,i] , schedule );
  const int tile_size_x = 1;
  const int tile_size_y = 8;
  const int tile_size_z = 8;
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, xi, yo, zo, yi, zi);
  
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  
  A.parallel(xo, cores_sunway);
  A.build( "sunway" );
  //
  Result Res( (k,j,i) , B[k,j,i] );
  //
  DefShapeMPI3D(shape_mpi, 1, 1, 1);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] );

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("3d7pt_star_sunway", "sunway", use_timing);
}

// 3d27pt box

/*
double a[M][N][P];
double b[M][N][P];
double c0;
double c1;
double c2;
double c3;
double c4;
double c5;
double c6;
double c7;
double c8;
double c9;
double c10;
double c11;
double c12;
double c13;
double c14;
double c15;
double c16;
double c17;
double c18;
double c19;
double c20;
double c21;
double c22;
double c23;
double c24;
double c25;
double c26;

for(long k=1; k < M-1; ++k){
for(long j=1; j < N-1; ++j){
for(long i=1; i < P-1; ++i){
b[k][j][i] = c0 * a[k][j][i]
+ c1 * a[k-1][j-1][i-1]
+ c2 * a[k][j-1][i-1]
+ c3 * a[k+1][j-1][i-1]
+ c4 * a[k-1][j][i-1]
+ c5 * a[k][j][i-1]
+ c6 * a[k+1][j][i-1]
+ c7 * a[k-1][j+1][i-1]
+ c8 * a[k][j+1][i-1]
+ c9 * a[k+1][j+1][i-1]
+ c10 * a[k-1][j-1][i]
+ c11 * a[k][j-1][i]
+ c12 * a[k+1][j-1][i]
+ c13 * a[k-1][j][i]
+ c14 * a[k+1][j][i]
+ c15 * a[k-1][j+1][i]
+ c16 * a[k][j+1][i]
+ c17 * a[k+1][j+1][i]
+ c18 * a[k-1][j-1][i+1]
+ c19 * a[k][j-1][i+1]
+ c20 * a[k+1][j-1][i+1]
+ c21 * a[k-1][j][i+1]
+ c22 * a[k][j][i+1]
+ c23 * a[k+1][j][i+1]
+ c24 * a[k-1][j+1][i+1]
+ c25 * a[k][j+1][i+1]
+ c26 * a[k+1][j+1][i+1]
;
}
}
}
*/
void bench_3d27pt_box_SharedMem_mpi(std::string target, int num_threads){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  const int M = 256;
  const int N = 256;
  const int P = 256;
  DefTensor3D_TimeWin(a, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.3);
  DefVar_Value(c5, f64, 0.2);
  DefVar_Value(c6, f64, 0.1);
  DefVar_Value(c7, f64, 0.1);
  DefVar_Value(c8, f64, 0.2);
  DefVar_Value(c9, f64, 0.3);
  DefVar_Value(c10, f64, 0.4);
  DefVar_Value(c11, f64, 0.3);
  DefVar_Value(c12, f64, 0.2);
  DefVar_Value(c13, f64, 0.1);
  DefVar_Value(c14, f64, 0.1);
  DefVar_Value(c15, f64, 0.2);
  DefVar_Value(c16, f64, 0.3);
  DefVar_Value(c17, f64, 0.4);
  DefVar_Value(c18, f64, 0.3);
  DefVar_Value(c19, f64, 0.2);
  DefVar_Value(c20, f64, 0.1);
  DefVar_Value(c21, f64, 0.1);
  DefVar_Value(c22, f64, 0.2);
  DefVar_Value(c23, f64, 0.3);
  DefVar_Value(c24, f64, 0.4);
  DefVar_Value(c25, f64, 0.3);
  DefVar_Value(c26, f64, 0.2);

  Kernel A( (k,j,i) , c0 * a[k,j,i]
      + c1 *  a[k-1, j-1, i-1]
      + c2 *  a[k  , j-1, i-1]
      + c3 *  a[k+1, j-1, i-1]
      + c4 *  a[k-1, j  , i-1]
      + c5 *  a[k  , j  , i-1]
      + c6 *  a[k+1, j  , i-1]
      + c7 *  a[k-1, j+1, i-1]
      + c8 *  a[k  , j+1, i-1]
      + c9 *  a[k+1, j+1, i-1]
      + c10 * a[k-1, j-1, i  ]
      + c11 * a[k  , j-1, i  ]
      + c12 * a[k+1, j-1, i  ]
      + c13 * a[k-1, j  , i  ]
      + c14 * a[k+1, j  , i  ]
      + c15 * a[k-1, j+1, i  ]
      + c16 * a[k  , j+1, i  ]
      + c17 * a[k+1, j+1, i  ]
      + c18 * a[k-1, j-1, i+1]
      + c19 * a[k  , j-1, i+1]
      + c20 * a[k+1, j-1, i+1]
      + c21 * a[k-1, j  , i+1]
      + c22 * a[k  , j  , i+1]
      + c23 * a[k+1, j  , i+1]
      + c24 * a[k-1, j+1, i+1]
      + c25 * a[k  , j+1, i+1]
      + c26 * a[k+1, j+1, i+1]
      , schedule );

  const int tile_size_x = 2;
  const int tile_size_y = 8;
  const int tile_size_z = 256;
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, yo, zo, xi, yi, zi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (a, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , a[k,j,i] );
  //
  DefShapeMPI3D(shape_mpi, 2, 2, 1);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, a, (k,j,i) , Res[t] << A[t-1] , num_threads);

  stencil.input( shape_mpi, a, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(a, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("3d27pt_box", target, use_timing);
}

void bench_3d27pt_box_SharedMem(std::string target, int num_threads){
  init();
  const int halo_size = 1;
  const int time_window_size = 2;
  const int M = 256;
  const int N = 256;
  const int P = 256;
  DefTensor3D_TimeWin(a, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(c0, f64, 0.1);
  DefVar_Value(c1, f64, 0.2);
  DefVar_Value(c2, f64, 0.3);
  DefVar_Value(c3, f64, 0.4);
  DefVar_Value(c4, f64, 0.3);
  DefVar_Value(c5, f64, 0.2);
  DefVar_Value(c6, f64, 0.1);
  DefVar_Value(c7, f64, 0.1);
  DefVar_Value(c8, f64, 0.2);
  DefVar_Value(c9, f64, 0.3);
  DefVar_Value(c10, f64, 0.4);
  DefVar_Value(c11, f64, 0.3);
  DefVar_Value(c12, f64, 0.2);
  DefVar_Value(c13, f64, 0.1);
  DefVar_Value(c14, f64, 0.1);
  DefVar_Value(c15, f64, 0.2);
  DefVar_Value(c16, f64, 0.3);
  DefVar_Value(c17, f64, 0.4);
  DefVar_Value(c18, f64, 0.3);
  DefVar_Value(c19, f64, 0.2);
  DefVar_Value(c20, f64, 0.1);
  DefVar_Value(c21, f64, 0.1);
  DefVar_Value(c22, f64, 0.2);
  DefVar_Value(c23, f64, 0.3);
  DefVar_Value(c24, f64, 0.4);
  DefVar_Value(c25, f64, 0.3);
  DefVar_Value(c26, f64, 0.2);

  Kernel A( (k,j,i) , c0 * a[k,j,i]
      + c1 *  a[k-1, j-1, i-1]
      + c2 *  a[k  , j-1, i-1]
      + c3 *  a[k+1, j-1, i-1]
      + c4 *  a[k-1, j  , i-1]
      + c5 *  a[k  , j  , i-1]
      + c6 *  a[k+1, j  , i-1]
      + c7 *  a[k-1, j+1, i-1]
      + c8 *  a[k  , j+1, i-1]
      + c9 *  a[k+1, j+1, i-1]
      + c10 * a[k-1, j-1, i  ]
      + c11 * a[k  , j-1, i  ]
      + c12 * a[k+1, j-1, i  ]
      + c13 * a[k-1, j  , i  ]
      + c14 * a[k+1, j  , i  ]
      + c15 * a[k-1, j+1, i  ]
      + c16 * a[k  , j+1, i  ]
      + c17 * a[k+1, j+1, i  ]
      + c18 * a[k-1, j-1, i+1]
      + c19 * a[k  , j-1, i+1]
      + c20 * a[k+1, j-1, i+1]
      + c21 * a[k-1, j  , i+1]
      + c22 * a[k  , j  , i+1]
      + c23 * a[k+1, j  , i+1]
      + c24 * a[k-1, j+1, i+1]
      + c25 * a[k  , j+1, i+1]
      + c26 * a[k+1, j+1, i+1]
      , schedule );
  //32k=4*32*32(double)
  const int tile_size_x = 2;
  const int tile_size_y = 8;
  const int tile_size_z = 256;
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, yo, zo, xi, yi, zi);
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , a[k,j,i] );
  //
  auto t = Stencil::t;
  const int use_threads = 0;
  bool is_assigned =false;
  Stencil stencil( (k,j,i) , Res[t] << A[t-1], use_threads, is_assigned  );

  stencil.input( a, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(a, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code("3d27pt_box", target, use_timing);
}


//3d13pt_star
/*
 int L, M, N;
 int k, j, i;
 double in[L][M][N], out[L][M][N];
 double a = 0.1
 double d = 0.2;
 double e = 0.3;
 double f = 0.4;

 for(long k=2; k < L-2; ++k)
 for(long j=2; j < M-2; ++j)
 for(long i=2; i < N-2; ++i){
  out[k][j][i] = a*in[k][j][i] + 
	   d*(in[k-2][j][i] + 
              in[k][j-2][i] + 
              in[k][j][i-2] + 
              in[k][j][i+2] + 
              in[k][j+2][i] + 
              in[k+2][j][i]) + 
       e*(in[k-1][j][i] + 
              in[k][j-1][i] + 
              in[k][j][i-1] + 
              in[k][j][i+1] + 
              in[k][j+1][i] + 
              in[k+1][j][i]) -
       f*in[k][j][i];
 }
*/
void bench_3d13pt_star_helmholtz_sunway_bench(std::string target, int num_threads, int D1, int D2, int D3, 
            int M, int N, int P, int tile_size_x,  int tile_size_y, int tile_size_z, int use_schedule=1, int is_assigned=1 ){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, DT, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(a, DT, 0.1);
  DefVar_Value(d, DT, 0.2);
  DefVar_Value(e, DT, 0.3);
  DefVar_Value(f, DT, 0.4);
  //
  if(use_schedule){
    Kernel A( (k,j,i) , a* B[k,j,i] + d* (B[k-2,j,i] + B[k,j-2,i] + B[k,j,i-2] + B[k,j,i+2] + B[k,j+2,i] + B[k+2,j,i] ) + e* (B[k-1,j,i] + B[k,j-1,i] + B[k,j,i-1] + B[k,j,i+1] + B[k,j+1,i] + B[k+1,j,i] ) - f * B[k,j,i] , schedule );
    //
    Axis xo, yo, xi, yi, zo, zi;
    A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
    A.reorder(xo, xi, yo, zo, yi, zi);
    //
    CacheRead  buffer_read;
    CacheWrite buffer_write;
    A.cache_read  (B, buffer_read,  "global");
    A.cache_write (buffer_write, "global");
    A.compute_at(buffer_read,  xo);
    A.compute_at(buffer_write, xo);
    //
    assert(num_threads==64);
    A.parallel(xo, num_threads);
    A.build( target );
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , 0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_3d13pt_star_sunway_bench.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d13pt_star_helmholtz", target, use_timing);
  }
  else{
    Kernel A( (k,j,i) , a* B[k,j,i] + d* (B[k-2,j,i] + B[k,j-2,i] + B[k,j,i-2] + B[k,j,i+2] + B[k,j+2,i] + B[k+2,j,i] ) + e* (B[k-1,j,i] + B[k,j-1,i] + B[k,j,i-1] + B[k,j,i+1] + B[k,j+1,i] + B[k+1,j,i] ) - f * B[k,j,i] );
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , 0, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    stencil.output(B, time_step10, "../data/out_3d13pt_star_sunway_bench_seq.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d13pt_star_helmholtz", target, use_timing);
  }
}


void bench_3d13pt_star_helmholtz_sunway(std::string target, int num_threads, int D1, int D2, int D3, 
            int M, int N, int P, int tile_size_x,  int tile_size_y, int tile_size_z){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(a, f64, 0.1);
  DefVar_Value(d, f64, 0.2);
  DefVar_Value(e, f64, 0.3);
  DefVar_Value(f, f64, 0.4);
  //
  Kernel A( (k,j,i) , a* B[k,j,i] + d* (B[k-2,j,i] + B[k,j-2,i] + B[k,j,i-2] + B[k,j,i+2] + B[k,j+2,i] + B[k+2,j,i] ) + e* (B[k-1,j,i] + B[k,j-1,i] + B[k,j,i-1] + B[k,j,i+1] + B[k,j+1,i] + B[k+1,j,i] ) - f * B[k,j,i] , schedule );
  //
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, xi, yo, zo, yi, zi);
  //
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  //
  assert(num_threads==64);
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , B[k,j,i] );
  //
  DefShapeMPI3D(shape_mpi, D1, D2, D3);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] );

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("3d13pt_star_helmholtz", target, use_timing);
}

void bench_3d13pt_star_helmholtz_SharedMem_mpi_bench_schedule(std::string target, int num_threads, int D1, int D2, int D3, 
            int M=256, int N=256, int P=256, int tile_size_x=2,  int tile_size_y=8, int tile_size_z=256,
        int use_schedule=1, int use_tile=1, int use_reorder=1, int use_threads=0, int is_assigned=0){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(a, f64, 0.1);
  DefVar_Value(d, f64, 0.2);
  DefVar_Value(e, f64, 0.3);
  DefVar_Value(f, f64, 0.4);
  //
  if(use_schedule){
    Kernel A( (k,j,i) , a* B[k,j,i] + d* (B[k-2,j,i] + B[k,j-2,i] + B[k,j,i-2] + B[k,j,i+2] + B[k,j+2,i] + B[k+2,j,i] ) + e* (B[k-1,j,i] + B[k,j-1,i] + B[k,j,i-1] + B[k,j,i+1] + B[k,j+1,i] + B[k+1,j,i] ) - f * B[k,j,i] , schedule );
    //
    if(use_tile){
    Axis xo, yo, xi, yi, zo, zi;
    A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
    if(use_reorder){
    A.reorder(xo, yo, zo, xi, yi, zi);
    }
    A.parallel(xo, num_threads);
    A.build( target );
    }
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    //stencil.output(B, time_step10, "../data/out.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d13pt_star_helmholtz", target, use_timing);
  }
  else{
    Kernel A( (k,j,i) , a* B[k,j,i] + d* (B[k-2,j,i] + B[k,j-2,i] + B[k,j,i-2] + B[k,j,i+2] + B[k,j+2,i] + B[k+2,j,i] ) + e* (B[k-1,j,i] + B[k,j-1,i] + B[k,j,i-1] + B[k,j,i+1] + B[k,j+1,i] + B[k+1,j,i] ) - f * B[k,j,i] , num_threads );
    //
    Result Res( (k,j,i) , B[k,j,i] );
    //
    DefShapeMPI3D(shape_mpi, D1, D2, D3);
    auto t = Stencil::t;
    Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , use_threads, is_assigned);

    stencil.input( shape_mpi, B, input_file);
    stencil.run(1,100);
    const int time_step10 = 100;
    //stencil.output(B, time_step10, "../data/out.data");
    PrintIR();
    const bool use_timing = true;
    stencil.compile_to_source_code_mpi("3d13pt_star_helmholtz", target, use_timing);
  }
}

void bench_3d13pt_star_helmholtz_SharedMem_mpi(std::string target, int num_threads, int D1, int D2, int D3, 
            int M=256, int N=256, int P=256, int tile_size_x=2,  int tile_size_y=8, int tile_size_z=256){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  /*
  const int M = 256;
  const int N = 256;
  const int P = 256;
  */
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(a, f64, 0.1);
  DefVar_Value(d, f64, 0.2);
  DefVar_Value(e, f64, 0.3);
  DefVar_Value(f, f64, 0.4);
  //
  Kernel A( (k,j,i) , a* B[k,j,i] + d* (B[k-2,j,i] + B[k,j-2,i] + B[k,j,i-2] + B[k,j,i+2] + B[k,j+2,i] + B[k+2,j,i] ) + e* (B[k-1,j,i] + B[k,j-1,i] + B[k,j,i-1] + B[k,j,i+1] + B[k,j+1,i] + B[k+1,j,i] ) - f * B[k,j,i] , schedule );
  /*
  const int tile_size_x = 2;
  const int tile_size_y = 8;
  const int tile_size_z = 256;
  */
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, yo, zo, xi, yi, zi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , B[k,j,i] );
  //
  DefShapeMPI3D(shape_mpi, D1, D2, D3);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (k,j,i) , Res[t] << A[t-1] , num_threads);

  stencil.input( shape_mpi, B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code_mpi("3d13pt_star_helmholtz", target, use_timing);
}

void bench_3d13pt_star_helmholtz_SharedMem(std::string target, int num_threads){
  init();
  const int halo_size = 2;
  const int time_window_size = 2;
  const int M = 256;
  const int N = 256;
  const int P = 256;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f64, M, N, P)  ;
  DefVar(k,i32);
  DefVar(j,i32);
  DefVar(i,i32);
  DefVar_Value(a, f64, 0.1);
  DefVar_Value(d, f64, 0.2);
  DefVar_Value(e, f64, 0.3);
  DefVar_Value(f, f64, 0.4);
  //
  Kernel A( (k,j,i) , a* B[k,j,i] + d* (B[k-2,j,i] + B[k,j-2,i] + B[k,j,i-2] + B[k,j,i+2] + B[k,j+2,i] + B[k+2,j,i] ) + e* (B[k-1,j,i] + B[k,j-1,i] + B[k,j,i-1] + B[k,j,i+1] + B[k,j+1,i] + B[k+1,j,i] ) - f * B[k,j,i] , schedule );
  const int tile_size_x = 2;
  const int tile_size_y = 8;
  const int tile_size_z = 256;
  Axis xo, yo, xi, yi, zo, zi;
  A.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  A.reorder(xo, yo, zo, xi, yi, zi);
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  A.parallel(xo, num_threads);
  A.build( target );
  //
  Result Res( (k,j,i) , B[k,j,i] );
  //
  auto t = Stencil::t;
  const int use_threads = 0;
  bool is_assigned =false;
  Stencil stencil( (k,j,i) , Res[t] << A[t-1], use_threads, is_assigned );

  stencil.input( B, input_file);
  stencil.run(1,100);
  const int time_step10 = 100;
  //stencil.output(B, time_step10, "../data/out.data");
  PrintIR();
  const bool use_timing = true;
  stencil.compile_to_source_code("3d13pt_star_helmholtz", target, use_timing);
}

void test_scalability(std::string target, int num_threads, std::vector<int> a)
{
  if(a.size()==7){
    int cases=a[0]; 
    int D1=a[1]; 
    int D2=a[2]; 
    int M =a[3]; 
    int N =a[4]; 
    int tile_size_x =a[5];  
    int tile_size_y =a[6]; 
    // 
    if(target=="feiteng" || target=="x86"){
      if(cases==1)
        bench_2d9pt_star_SharedMem_mpi(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y);
      if(cases==2)
        bench_2d9pt_box_SharedMem_mpi(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y);
    }
    else if(target=="sunway"){//
      if(cases==1)
        bench_2d9pt_star_sunway(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y);
      if(cases==2)
        bench_2d9pt_box_sunway(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y);
    }
  }
  else if(a.size()==10){
    int cases=a[0]; 
    int D1=a[1]; 
    int D2=a[2]; 
    int D3=a[3]; 
    int M =a[4]; 
    int N =a[5]; 
    int P =a[6]; 
    int tile_size_x =a[7];  
    int tile_size_y =a[8]; 
    int tile_size_z =a[9]; 
    //
    if(target=="feiteng" || target=="x86"){
      if(cases==3)
        bench_3d7pt_star_SharedMem_mpi(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z);
      if(cases==4)
        bench_3d13pt_star_helmholtz_SharedMem_mpi(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z);
    }
    else if(target=="sunway"){//
      if(cases==3)
        bench_3d7pt_star_sunway(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z);
      if(cases==4)
        bench_3d13pt_star_helmholtz_sunway(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z);
    }
  }
  else{
    std::cout<<"the input parameters are wrong!"<<std::endl;
    exit(-1);
  }
}
void bench_schedule(std::string target, int num_threads, std::vector<int> a){
  if(a.size()==12){
    int cases=a[0]; 
    int D1=a[1]; 
    int D2=a[2]; 
    int M =a[3]; 
    int N =a[4]; 
    int tile_size_x =a[5];  
    int tile_size_y =a[6]; 
    //
    int use_schedule=a[7]; 
    int use_tile=a[8];
    int use_reorder=a[9]; 
    int use_threads=a[10]; 
    int is_assigned=a[11];
    // 
    if(target=="feiteng" || target=="x86" || target=="macos"){
      if(cases==1)
        bench_2d9pt_star_SharedMem_mpi_bench_schedule(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y,
        use_schedule, use_tile, use_reorder, use_threads, is_assigned );
      if(cases==2)
        bench_2d9pt_box_SharedMem_mpi_bench_schedule(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y, use_schedule, use_tile, use_reorder, use_threads, is_assigned );
    }
  }
  else if(a.size()==15){
    int cases=a[0]; 
    int D1=a[1]; 
    int D2=a[2]; 
    int D3=a[3]; 
    int M =a[4]; 
    int N =a[5]; 
    int P =a[6]; 
    int tile_size_x =a[7];  
    int tile_size_y =a[8]; 
    int tile_size_z =a[9]; 
    //
    int use_schedule=a[10]; 
    int use_tile=a[11];
    int use_reorder=a[12]; 
    int use_threads=a[13]; 
    int is_assigned=a[14];
    //
    if(target=="feiteng" || target=="x86" || target=="macos"){
      if(cases==3)
        bench_3d7pt_star_SharedMem_mpi_bench_schedule(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z, use_schedule, use_tile, use_reorder, use_threads, is_assigned );
      if(cases==4)
        bench_3d13pt_star_helmholtz_SharedMem_mpi_bench_schedule(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z,
            use_schedule, use_tile, use_reorder, use_threads, is_assigned );
    }
  }
  else{
    std::cout<<"the input parameters are wrong!"<<std::endl;
    exit(-1);
  }
}

void bench_sunway(std::string target, int num_threads, std::vector<int> a){
  if(a.size()==9){
    int cases=a[0]; 
    int D1=a[1]; 
    int D2=a[2]; 
    int M =a[3]; 
    int N =a[4]; 
    int tile_size_x =a[5];  
    int tile_size_y =a[6]; 
    //
    int use_schedule=a[7]; 
    int is_assigned=a[8];
    // 
    if(target=="sunway"){//
      if(cases==1)
        bench_2d9pt_star_sunway_bench(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y, use_schedule, is_assigned);
      if(cases==2)
        bench_2d9pt_box_sunway_bench(target, num_threads, D1, D2, M, N, tile_size_x, tile_size_y, use_schedule, is_assigned);
    }
  }
  else if(a.size()==12){
    int cases=a[0]; 
    int D1=a[1]; 
    int D2=a[2]; 
    int D3=a[3]; 
    int M =a[4]; 
    int N =a[5]; 
    int P =a[6]; 
    int tile_size_x =a[7];  
    int tile_size_y =a[8]; 
    int tile_size_z =a[9]; 
    //
    int use_schedule=a[10]; 
    int is_assigned=a[11];
    //
    if(target=="sunway"){//
      if(cases==3)
        bench_3d7pt_star_sunway_bench(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z, use_schedule, is_assigned);
      if(cases==4)
        bench_3d13pt_star_helmholtz_sunway_bench(target, num_threads, D1, D2, D3, M, N, P, tile_size_x, tile_size_y, tile_size_z, use_schedule, is_assigned);
    }
  }
  else{
    std::cout<<"the input parameters are wrong!"<<std::endl;
    exit(-1);
  }
}


void test_weak_scalability(){
  std::string target = "feiteng";
  int num_threads = 32;
  //
  //bench_2d9pt_star_SharedMem_mpi(target, num_threads, 4, 8);
  //bench_2d9pt_star_SharedMem_mpi(target, num_threads, 8, 8);
  //bench_2d9pt_star_SharedMem_mpi(target, num_threads, 8, 16);
  //bench_2d9pt_star_SharedMem_mpi(target, num_threads,16, 16);
  //
  //bench_2d9pt_box_SharedMem_mpi(target, num_threads, 4, 8);
  //bench_2d9pt_box_SharedMem_mpi(target, num_threads, 8, 8);
  //bench_2d9pt_box_SharedMem_mpi(target, num_threads, 8, 16);
  //bench_2d9pt_box_SharedMem_mpi(target, num_threads,16, 16);
  //
  //bench_3d7pt_star_SharedMem_mpi(target, num_threads, 2, 4, 4);
  //bench_3d7pt_star_SharedMem_mpi(target, num_threads, 4, 4, 4);
  //bench_3d7pt_star_SharedMem_mpi(target, num_threads, 4, 4, 8);
  //bench_3d7pt_star_SharedMem_mpi(target, num_threads, 4, 8, 8);
  //
  //bench_3d13pt_star_helmholtz_SharedMem_mpi(target, num_threads, 2, 4, 4);
  //bench_3d13pt_star_helmholtz_SharedMem_mpi(target, num_threads, 4, 4, 4);
  //bench_3d13pt_star_helmholtz_SharedMem_mpi(target, num_threads, 4, 4, 8);
  //bench_3d13pt_star_helmholtz_SharedMem_mpi(target, num_threads, 4, 8, 8);
}

void bench_x86_vs_patus(){ //vs patus on x86
  //std::string target = "x86";
  //int num_threads = 28;
  std::string target = "macos";
  int num_threads = 8;
  //bench_2d9pt_star_SharedMem(target, num_threads);
  //bench_2d9pt_box_SharedMem(target, num_threads);
  //bench_3d7pt_star_SharedMem(target, num_threads);
  //bench_3d27pt_box_SharedMem(target, num_threads);
  //bench_3d13pt_star_helmholtz_SharedMem(target, num_threads);
}

int main(int argc, char * argv[]){

  /*
  bench_x86_vs_patus();
  //
  std::vector<int> a;
  std::cout<<"argc = "<<argc<<std::endl;
  std::string bench_kind = "";
  std::string target = "";
  int num_threads = 0;
  if(argc>=4){
    bench_kind = std::string(argv[1]);
    target = std::string(argv[2]);
    num_threads = atoi(argv[3]);
    std::cout<<"test_kind: "<<bench_kind<<std::endl;
    std::cout<<"target: "<<target<<std::endl;
    std::cout<<"num_threads: "<<num_threads<<std::endl;
  }
  for(int i=4;i< argc; i++){
    std::cout<<atoi(argv[i])<<" ";
    a.push_back(atoi(argv[i]));
  }
  std::cout<<std::endl;
  if(bench_kind=="scalability"){
    test_scalability(target, num_threads, a);
  }
  else if(bench_kind=="bench_schedule"){
    std::cout<<"bench_schedule"<<std::endl;
    bench_schedule(target, num_threads, a);
  }
  else if(bench_kind=="bench_sunway"){
    std::cout<<"bench_sunway"<<std::endl;
    bench_sunway(target, num_threads, a);
  }
  else if(bench_kind==""){
  }
  else{
    std::cout<<"this kind of test is not setting!"<<std::endl;
    exit(-1);
  }
  */

  //bench_2d9pt_star_SharedMem_mpi("feiteng", 32, 1, 1, 4096, 4096, 8, 2048);
  bench_3d7pt_star_sunway("sunway", 64, 1, 1, 1, 1024, 32, 32, 1, 8, 8);
  //bench_3d7pt_star_sunway("sunway", 64, 8, 4, 4, 1024, 32, 32, 1, 8, 8);
  //bench_3d7pt_star_sunway("sunway", 64, 8, 8, 8, 4096, 64, 32, 128, 8, 8);
  //bench_3d13pt_star_helmholtz_sunway("sunway", 64, 1, 1, 1, 1024, 32, 32, 1, 8, 8);
  //
  //std::string target = "x86";
  //int num_threads = 28;
  //std::string target = "feiteng";
  //int num_threads = 32;
  //std::string target = "macos";
  //int num_threads = 8;
  //
  //bench_2d9pt_star_SharedMem(target, num_threads);
  //bench_2d9pt_star_SharedMem_mpi(target, num_threads, 1, 1);
  //
  //bench_2d9pt_box_SharedMem(target, num_threads);
  //bench_2d9pt_box_SharedMem_mpi(target, num_threads, 1, 1);
  //
  //bench_3d7pt_star_SharedMem(target, num_threads);
  //bench_3d7pt_star_SharedMem_mpi(target, num_threads, 1, 1, 1);
  //
  //bench_3d7pt_star_sunway();
  //
  //bench_3d27pt_box_SharedMem(target, num_threads);
  //bench_3d27pt_box_SharedMem_mpi(target, num_threads);
  //
  //bench_3d13pt_star_helmholtz_SharedMem(target, num_threads);
  //bench_3d13pt_star_helmholtz_SharedMem_mpi(target, num_threads, 1, 1, 1);
  return 0;
}

