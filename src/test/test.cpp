#include <iostream>
#include <vector>
#include "../ir.h"
#include <cmath>


void test_ir(){

  DefVar(x,f32);
  DefVar(y,f32);
  DefVar(z,f32);
  //Kernel func(x+(y*y)+x);
  //Kernel func(sin(x+(y*y)+x));
  //Kernel func(sin(x+(y*y)+x)%y);
  //Kernel func(sin(x+(y*y)+x)/y);
  //Kernel func(x+x+x+sin(x+(y*y)));
  //Kernel func(x+x+x+(x+(y*y)));
  //Kernel func( x+sin(x) );
  //Kernel func( sin(x)*x );
  //Kernel func(x+(x+y)*(y+x*y));
  //Kernel func(x+y+z);
  //Kernel func(x*y*z);
  //Kernel func(sin(x+y)*x+y*y/max(x,y));
  Kernel func(sin(x+y)*x+y*y%max(x,y));
  std::cout<<"is_spatial : "<<func.is_spatial()<<std::endl;
  func.print_ir();

  ExprGroup eg=(x,y);
  //ExprGroup eg=[x,y];
  max(x,y);
  (x,y);
  std::cout<<eg.exprs.size()<<std::endl;

}

void fill_array_inc(float * arr, int length, float value){
  for(int i=0;i<length;i++){
    *(arr+i)=value+i;
  }
}

void fill_array_const(float * arr, int length, float value){
  for(int i=0;i<length;i++){
    *(arr+i)=value;
  }
}

void print_array(float * arr, int length){
  for(int i=0;i<length;i++){
    std::cout<<*(arr+i)<<" ";
  }
  std::cout<<std::endl;
}

void check_array(float * a, float * b, int length){
  bool is_diff = false;
  for(int i=0;i<length;i++){
    if(a[i]!=b[i]){
      is_diff = true;
      break;
    }
  }
  if(is_diff)
    std::cout<<"a[i] is diff from b[i]!"<<std::endl;
  else
    std::cout<<"a[i] is totally same as b[i]!"<<std::endl;

}

void split_stencil(){
  const int n_grid=5;
  float a[n_grid][n_grid],b[n_grid][n_grid],c[n_grid][n_grid];
  float a_temp[n_grid][n_grid],b_temp[n_grid][n_grid],c_temp[n_grid][n_grid];
  fill_array_inc(a[0], n_grid*n_grid, 1);
  fill_array_inc(b[0], n_grid*n_grid, 2);
  fill_array_const(c[0], n_grid*n_grid, 0);
  fill_array_const(c_temp[0], n_grid*n_grid, 0);
  for(int i=0+1;i<n_grid-1;i++){
    for(int j=0+1;j<n_grid-1;j++){
      c[i][j]=(a[i][j]+a[i-1][j]+a[i+1][j]+a[i][j-1]+a[i][j+1])*(b[i][j]+b[i-1][j]+b[i+1][j]+b[i][j-1]+b[i][j+1]);
    }
  }
  for(int i=0+1;i<n_grid-1;i++){
    for(int j=0+1;j<n_grid-1;j++){
      a_temp[i][j]=(a[i][j]+a[i-1][j]+a[i+1][j]+a[i][j-1]+a[i][j+1]);
      b_temp[i][j]=(b[i][j]+b[i-1][j]+b[i+1][j]+b[i][j-1]+b[i][j+1]);
    }
  }
  for(int i=0+1;i<n_grid-1;i++){
    for(int j=0+1;j<n_grid-1;j++){
      c_temp[i][j]=a_temp[i][j]*b_temp[i][j];
    }
  }
  print_array(a[0], n_grid*n_grid);
  print_array(c[0], n_grid*n_grid);
  print_array(c_temp[0], n_grid*n_grid);
  check_array(c[0], c_temp[0], n_grid*n_grid);
}

void check_kernel(int t, float (TeNode0[3][4])){
  for(int i=0;i<3;i++){
    for(int j=0;j<4;j++){
      TeNode0[i][j]=233;
    }
  }
}

void test_SpNode(){
  const int halo_size = 1;
  DefTensor2D(A, halo_size, f32, 3, 4)  ;
  DefTensor2D(B, halo_size, f32, 3, 4)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  //Kernel func( A[i+1,j+2]+A[i,j] );
  //Kernel func( A[i+1,j+2]+A[j,i]*B[i,3+j] );
  //Kernel func( A[i,j] + A[i+1,j] + A[i-1,j] + A[i,j+1] + A[i,j-1]);
  Kernel func( A[i,j]*B[j,i] );
  func.print_ir();
}

void test_lambda(){
  {
  auto stencil = [](int i,int j){return i+j;};
  int i=1,j=2,k=0;
  k=stencil(i,j);
  std::cout<<"k : "<<k<<std::endl;
  }
  // dsl
  {
  auto lap = [](Var i, Var j, Tensor A){return -4 * A[i,j] + A[i,j-1] + A[i,j+1]+ A[i-1,j]+ A[i+1,j];};
  const int halo_size = 1;
  DefTensor2D(A, halo_size, f32, 3, 4)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  Kernel laplac(lap(i, j, A));
  }
}

void test_dfdt3d(){
  init();
  const int halo_size = 1;
  DefTensor3D(B, halo_size, f32, 4, 4, 4)  ;
  // --> Input data
  //Res[0] = xxx;
  //Res[1] = xxx;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar(k,i32);
  DefVar(c,f32);
  DefVar(dt,f32);
  DefVar(alpha,f32);
  auto t = Stencil::t;
  Kernel laplaciant( (i,j,k) , -6 * B[i,j,k] + B[i,j-1,k] + B[i,j+1,k]+ B[i-1,j,k]+ B[i+1,j,k]+ B[i,j,k-1]+ B[i,j,k+1]);
  Kernel A( (i,j,k) , B[i,j,k] );
  Result Res( (i,j,k) , B[i,j,k] );
  Stencil stencil( (i,j,k) , Res[t] << 2* A[t-1] + ( c * c * dt * dt + c * alpha * dt) * laplaciant[t-1] 
      - A[t-2] - c * alpha *dt *dt * laplaciant[t-2]  );
  // A[t,i,j] = 2 * A[t-1,i,j] + (c * c * dt * dt + c * alpha * dt) * laplaciant[t-1,i,j] 
  // - A[t-2,i,j] -c * alpha *dt *dt * laplaciant[t-2,i,j]
  
  int start =2, end=100;
  stencil.run(2,100);
  // --> Output data
  //PrintIR();
  //stencil.compile();
  stencil.compile_to_source_code("dfdt3d");
}

void test_Sugar(){
  init();
  const int halo_size = 1;
  const int time_window_size = 3;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f32, 3, 4)  ;
  // --> Input data
  //Res[0] = xxx;
  //Res[1] = xxx;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar(c,f32);
  DefVar(dt,f32);
  DefVar(alpha,f32);
  auto t = Stencil::t;
  Kernel laplaciant( (i,j) , -4 * B[i,j] + B[i,j-1] + B[i,j+1]+ B[i-1,j]+ B[i+1,j] );
  Kernel A( (i,j) , B[i,j] );
  Result Res( (i,j) , B[i,j] );
  int start =2, end=100;
  Stencil stencil( (i,j) , Res[t] << 2* A[t-1] + ( c * c * dt * dt + c * alpha * dt) * laplaciant[t-1] 
      - A[t-2] - c * alpha *dt *dt * laplaciant[t-2]  );
  // A[t,i,j] = 2 * A[t-1,i,j] + (c * c * dt * dt + c * alpha * dt) * laplaciant[t-1,i,j] 
  // - A[t-2,i,j] -c * alpha *dt *dt * laplaciant[t-2,i,j]

  ////stencil.print_ir();
  
  // --> Calling Graph Function
  // --> Running on time step
  stencil.input(B, "../data/rand.data");
  stencil.run(2,10);
  const int time_step10 = 10;
  stencil.output(B, time_step10, "../data/out.data");
  // --> Output data
  //stencil.output(100);
  PrintIR();
  //stencil.compile();
  stencil.compile_to_source_code("dfdt2d");
  //stencil.compile();
  
}

void test_TeNode(){
  // --> definition
  init();
  const int halo_size = 1;
  DefTensor2D(B, halo_size, f32, 3, 4)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  // --> FrontendForStmt
  // --> FunctionStmt
  //Kernel laplaciant( -4 * A[i,j] + A[i,j-1] + A[i,j+1]+ A[i-1,j]+ A[i+1,j]);
  //Kernel laplaciant( (3,4), lambda(i,j) , -4 * A[i,j] + A[i,j-1] + A[i,j+1]+ A[i-1,j]+ A[i+1,j]);
  //Kernel laplaciant( spatial(i,j) , -4 * A[i,j] + A[i,j-1] + A[i,j+1]+ A[i-1,j]+ A[i+1,j] );
  Kernel laplaciant( (i,j) , -4 * B[i,j] + B[i,j-1] + B[i,j+1]+ B[i-1,j]+ B[i+1,j] );
  std::cout<<"is_spatial : "<<laplaciant.is_spatial()<<std::endl;
  laplaciant.print_ir();
  auto t = Stencil::t;
  // --> CallFunction
  // --> time step
  auto lap_1 = laplaciant.temporal( t -1 );
  auto lap_2 = laplaciant.temporal( t -2 );
  Kernel data( (i,j) , B[i,j] );
  std::cout<<"is_spatial : "<<data.is_spatial()<<std::endl;
  //data.print_ir();
  auto ten_0 = data.temporal( t    );
  auto ten_1 = data.temporal( t -1 );
  auto ten_2 = data.temporal( t -2 );
  // --> definition
  DefVar(c,f32);
  DefVar(dt,f32);
  DefVar(alpha,f32);
  // --> Calling Kernel Function
  // --> Building Graph to Analysis Temporal Dependency
  // --> Building Graph Function
  Stencil stencil( ten_0 EQUALS 2* ten_1 + ( c * c * dt * dt + c * alpha * dt) * lap_1 - ten_2 - c * alpha *dt *dt * lap_2 );
  // A[t,i,j] = 2 * A[t-1,i,j] + (c * c * dt * dt + c * alpha * dt) * laplaciant[t-1,i,j] 
  // - A[t-2,i,j] -c * alpha *dt *dt * laplaciant[t-2,i,j]
  // stencil.print_ir();
  
  // --> Input data
  //A.temporal(0) = xxx;
  //A.temporal(-1) = xxx;
  // --> Calling Graph Function
  // --> Running on time step
  //stencil.run(0,100);
  // --> Output data
  //stencil.output(100);
  
}

void test_IO(){
  float input[100];
  int size=100;
  std::string name_file="../data/rand.data";
  //InputData_float (input, size, const_cast<char *>(name_file.c_str()));
  InputData_float (input, size, "../data/rand.data");
  fmt::print<float>(input, size);
  std::string name_out_file="../data/out.data";
  OutputData_float (input, size, const_cast<char *>(name_out_file.c_str()));
  int array[3][3]={1,2,3,4,5,6,7,8,9};
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      std::cout<<array[i][j]<<" ";
    }
  }
  std::cout<<std::endl;
  int * ptr=&array[0][0];
  for(int i=0;i<9;i++){
    std::cout<<ptr[i]<<" ";
  }
}
//double sin(double x){
//  return x+1;
//}

void test_mpi(){
  init();
  const int halo_size = 1;
  const int time_window_size = 3;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f32, 3, 4)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar_Value(c, f32, 0.1);
  DefVar_Value(dt, f32, 0.2);
  DefVar_Value(alpha, f32, 0.3);
  auto t = Stencil::t;
  Kernel laplaciant( (i,j) , -4 * B[i,j] + B[i,j-1] + B[i,j+1]+ B[i-1,j]+ B[i+1,j] );
  Kernel A( (i,j) , B[i,j] );
  Result Res( (i,j) , B[i,j] );
  DefShapeMPI2D(shape_mpi, 2, 2);
  Stencil stencil( shape_mpi, B, (i,j) , Res[t] << 2* A[t-1] + ( c * c * dt * dt + c * alpha * dt) * laplaciant[t-1] 
      - A[t-2] - c * alpha *dt *dt * laplaciant[t-2]  );
  // A[t,i,j] = 2 * A[t-1,i,j] + (c * c * dt * dt + c * alpha * dt) * laplaciant[t-1,i,j] 
  // - A[t-2,i,j] -c * alpha *dt *dt * laplaciant[t-2,i,j]

  stencil.input( shape_mpi, B, "../data/rand.data");
  stencil.run(2,200);
  const int time_step10 = 10;
  //stencil.output(shape_mpi, B, time_step10, "../data/out.data");
  PrintIR();
  //stencil.compile();
  //target hardware: x86, feiteng, sunway, macos
  stencil.compile_to_source_code_mpi("dfdt2d", "sunway");
}

void check_mpi(){
  init();
  const int halo_size = 1;
  const int time_window_size = 3;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f32, 32, 32)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar_Value(c, f32, 0.1);
  DefVar_Value(dt, f32, 0.2);
  DefVar_Value(alpha, f32, 0.5);
  auto t = Stencil::t;
  const int num_threads = 0;
  //Kernel A( (i,j) , B[i,j] );
  //Kernel A( (i,j) , B[i,j] , num_threads );
  Kernel A( (i,j) , -4 * B[i,j] + B[i,j-1] + B[i,j+1]+ B[i-1,j]+ B[i+1,j] , num_threads );
  Result Res( (i,j) , B[i,j] );
  DefShapeMPI2D(shape_mpi, 2, 2);
  Stencil stencil( shape_mpi, B, (i,j) , Res[t] << (A[t-1] + A[t-2]) * alpha , num_threads );

  stencil.input( shape_mpi, B, "../data/rand.data");
  stencil.run(2,100);
  const int time_step10 = 10;
  //stencil.output(shape_mpi, B, time_step10, "../data/out.data");
  PrintIR();
  //stencil.compile();
  //target hardware: x86, feiteng, sunway, macos
  stencil.compile_to_source_code_mpi("dfdt2d", "sunway");
}

void test_time_step(){
  init();
  const int halo_size = 1;
  const int time_window_size = 3;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f32, 3, 4)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar_Value(c, f32, 0.1);
  DefVar_Value(dt, f32, 0.2);
  DefVar_Value(alpha, f32, 0.15);
  Kernel laplaciant( (i,j) , -4 * B[i,j] + B[i,j-1] + B[i,j+1]+ B[i-1,j]+ B[i+1,j] , schedule );
  const int tile_size_x = 3;
  const int tile_size_y = 2;
  Axis xo, yo, xi, yi;
  laplaciant.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  laplaciant.reorder(xo, yo, xi, yi);
  laplaciant.parallel(xo, 3);
  laplaciant.build( "feiteng" );
  Kernel A( (i,j) , B[i,j] );
  Result Res( (i,j) , B[i,j] );
  auto t = Stencil::t;
  Stencil stencil((i,j) , Res[t] << 2* A[t-1] + ( c * c * dt * dt + c * alpha * dt) * laplaciant[t-1] - A[t-2] - c * alpha *dt *dt * laplaciant[t-2]);
  //
  stencil.input(B, "../data/rand.data");
  stencil.run(2,10);
  const int time_step10 = 10;
  stencil.output(B, time_step10, "../data/out.data");
  stencil.compile_to_source_code("dfdt2d");
}

void test_schedule_2D(){
  init();
  const int halo_size = 1;
  const int time_window_size = 3;
  DefTensor2D_TimeWin(B, time_window_size, halo_size, f32, 8, 8)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar_Value(c, f32, 0.1);
  DefVar_Value(dt, f32, 0.2);
  DefVar_Value(alpha, f32, 0.15);
  auto t = Stencil::t;
  //
  Kernel A( (i,j) , -4 * B[i,j] + B[i,j-1] + B[i,j+1]+ B[i-1,j]+ B[i+1,j] , schedule );
  const int tile_size_x = 2;
  const int tile_size_y = 4;
  Axis xo, yo, xi, yi;
  A.tile(tile_size_x, tile_size_y, xo, xi, yo, yi);
  A.reorder(xo, yo, xi, yi);
  //A.reorder(xo, xi, yo, yi);
  
  /*
  CacheRead  buffer_read;
  CacheWrite buffer_write;
  A.cache_read  (B, buffer_read,  "global");
  A.cache_write (buffer_write, "global");
  A.compute_at(buffer_read,  xo);
  A.compute_at(buffer_write, xo);
  */
  
  A.parallel(xo, 4);
  A.build( "macos" );
  //
  Result Res( (i,j) , B[i,j] );
  //
  DefShapeMPI2D(shape_mpi, 1, 1);
  const int num_threads = 0;
  Stencil stencil( shape_mpi, B, (i,j) , Res[t] << (A[t-1] + A[t-2]) * alpha , num_threads );

  stencil.input( shape_mpi, B, "../data/rand8x8.data");
  stencil.run(2,100);
  const int time_step10 = 10;
  //stencil.output(shape_mpi, B, time_step10, "../data/out.data");
  PrintIR();
  //stencil.compile();
  //target hardware: x86, feiteng, sunway, macos
  stencil.compile_to_source_code_mpi("dfdt2d", "macos");
}

void test_schedule_3D_sunway(){
  init();
  const int halo_size = 1;
  const int time_window_size = 3;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f32, 128, 16, 16)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar(k,i32);
  DefVar_Value(c, f32, 0.1);
  DefVar_Value(dt, f32, 0.2);
  DefVar_Value(alpha, f32, 0.15);
  auto t = Stencil::t;
  //
  Kernel A( (i,j,k) , -6 * B[i,j,k] + B[i-1,j,k]+ B[i+1,j,k] + B[i,j-1,k] + B[i,j+1,k] + B[i,j,k-1]+ B[i,j,k+1] , schedule );
  const int tile_size_x = 2;
  const int tile_size_y = 4;
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
  
  A.parallel(xo, 64);
  A.build( "sunway" );
  //
  Result Res( (i,j,k) , B[i,j,k] );
  //
  DefShapeMPI3D(shape_mpi, 1, 1, 1);
  const int num_threads = 0;
  Stencil stencil( shape_mpi, B, (i,j,k) , Res[t] << (A[t-1] + A[t-2]) * alpha , num_threads );

  stencil.input( shape_mpi, B, "../data/rand8x8.data");
  stencil.run(2,100);
  const int time_step10 = 10;
  //stencil.output(shape_mpi, B, time_step10, "../data/out.data");
  PrintIR();
  //stencil.compile();
  //target hardware: x86, feiteng, sunway, macos
  stencil.compile_to_source_code_mpi("dfdt3d", "sunway");
}

void test_schedule_3D_feiteng(){
  init();
  const int halo_size = 1;
  const int time_window_size = 3;
  DefTensor3D_TimeWin(B, time_window_size, halo_size, f32, 128, 16, 16)  ;
  DefVar(i,i32);
  DefVar(j,i32);
  DefVar(k,i32);
  DefVar_Value(c, f32, 0.1);
  DefVar_Value(dt, f32, 0.2);
  DefVar_Value(alpha, f32, 0.15);
  //
  Kernel laplaciant( (i,j,k) , -6 * B[i,j,k] + B[i-1,j,k]+ B[i+1,j,k] + B[i,j-1,k] + B[i,j+1,k] + B[i,j,k-1]+ B[i,j,k+1] , schedule );
  const int tile_size_x = 2;
  const int tile_size_y = 4;
  const int tile_size_z = 8;
  Axis xo, yo, xi, yi, zo, zi;
  laplaciant.tile(tile_size_x, tile_size_y, tile_size_z, xo, xi, yo, yi, zo, zi);
  laplaciant.reorder(xo, xi, yo, zo, yi, zi);
  laplaciant.parallel(xo, 16);
  laplaciant.build( "feiteng" );
  //
  Kernel A( (i,j,k) , B[i,j,k] );
  Result Res( (i,j,k) , B[i,j,k] );
  // 256*256*256
  DefShapeMPI3D(shape_mpi, 2, 16, 16);
  auto t = Stencil::t;
  Stencil stencil( shape_mpi, B, (i,j,k) , Res[t] << 2* A[t-1] + ( c * c * dt * dt + c * alpha * dt) * laplaciant[t-1] - A[t-2] - c * alpha *dt *dt * laplaciant[t-2]);

  stencil.input( shape_mpi, B, "../data/input.data");
  stencil.run(2,100);
  const int time_step10 = 10;
  //stencil.output(shape_mpi, B, time_step10, "../data/out.data");
  PrintIR();
  //stencil.compile();
  //target hardware: x86, feiteng, sunway, macos
  stencil.compile_to_source_code_mpi("dfdt3d", "feiteng");
}

void test_type(int value){
  std::cout<<value<<std::endl;
}

int main(){
  int * ptr=new int[5];
  ptr[0]=233;
  printf("address of ptr is : %p\n", ptr);
  std::vector<int *> vec;
  vec.push_back(ptr);
  printf("ptr 0 is : %d\n", *vec[0]);
  std::cout<<sin(2)<<std::endl;
  std::cout<<std::sin(2)<<std::endl;
  
  /*
  split_stencil();
  test_ir();
  test_lambda();
  test_SpNode();
  */
  //test_TeNode();
  //test_Sugar();
  //test_dfdt3d();
  //test_IO();
  //test_mpi();
  //check_mpi();
  //test_time_step();
  //test_schedule_2D();
  //test_schedule_3D_sunway();
  test_schedule_3D_feiteng();
  //test_type(0.3);
  /*
  float TeNode0[3][4];
  check_kernel(2, TeNode0);
  */

  
  //assert(true && "err");


  return 0;
}
